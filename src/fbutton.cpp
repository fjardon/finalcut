/***********************************************************************
* fbutton.cpp - Widget FButton                                         *
*                                                                      *
* This file is part of the Final Cut widget toolkit                    *
*                                                                      *
* Copyright 2012-2019 Markus Gans                                      *
*                                                                      *
* The Final Cut is free software; you can redistribute it and/or       *
* modify it under the terms of the GNU Lesser General Public License   *
* as published by the Free Software Foundation; either version 3 of    *
* the License, or (at your option) any later version.                  *
*                                                                      *
* The Final Cut is distributed in the hope that it will be useful,     *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include "final/fapplication.h"
#include "final/fbutton.h"
#include "final/fstatusbar.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FButton
//----------------------------------------------------------------------

// constructors and destructor
//----------------------------------------------------------------------
FButton::FButton(FWidget* parent)
  : FWidget(parent)
{
  init();
}

//----------------------------------------------------------------------
FButton::FButton (const FString& txt, FWidget* parent)
  : FWidget(parent)
  , text{txt}
{
  init();
}

//----------------------------------------------------------------------
FButton::~FButton()  // destructor
{
  delAccelerator();
  delOwnTimer();
}

// FButton operator
//----------------------------------------------------------------------
FButton& FButton::operator = (const FString& s)
{
  setText(s);
  return *this;
}

// public methods of FButton
//----------------------------------------------------------------------
void FButton::setForegroundColor (FColor color)
{
  FWidget::setForegroundColor(color);
  updateButtonColor();
}

//----------------------------------------------------------------------
void FButton::setBackgroundColor (FColor color)
{
  FWidget::setBackgroundColor(color);
  updateButtonColor();
}

//----------------------------------------------------------------------
void FButton::setHotkeyForegroundColor (FColor color)
{
  // valid colors -1..254
  if ( color == fc::Default || color >> 8 == 0 )
    button_hotkey_fg = color;
}

//----------------------------------------------------------------------
void FButton::setFocusForegroundColor (FColor color)
{
  // valid colors -1..254
  if ( color == fc::Default || color >> 8 == 0 )
    button_focus_fg = color;

  updateButtonColor();
}

//----------------------------------------------------------------------
void FButton::setFocusBackgroundColor (FColor color)
{
  // valid colors -1..254
  if ( color == fc::Default || color >> 8 == 0 )
    button_focus_bg = color;

  updateButtonColor();
}

//----------------------------------------------------------------------
void FButton::setInactiveForegroundColor (FColor color)
{
  // valid colors -1..254
  if ( color == fc::Default || color >> 8 == 0 )
    button_inactive_fg = color;

  updateButtonColor();
}

//----------------------------------------------------------------------
void FButton::setInactiveBackgroundColor (FColor color)
{
  // valid colors -1..254
  if ( color == fc::Default || color >> 8 == 0 )
    button_inactive_bg = color;

  updateButtonColor();
}

//----------------------------------------------------------------------
bool FButton::setNoUnderline (bool enable)
{
  return (flags.no_underline = enable);
}

//----------------------------------------------------------------------
bool FButton::setEnable (bool enable)
{
  FWidget::setEnable(enable);

  if ( enable )
    setHotkeyAccelerator();
  else
    delAccelerator();

  updateButtonColor();
  return enable;
}

//----------------------------------------------------------------------
bool FButton::setFocus (bool enable)
{
  FWidget::setFocus(enable);

  if ( enable )
  {
    if ( isEnabled() )
    {
      if ( getStatusBar() )
      {
        const auto& msg = getStatusbarMessage();
        const auto& curMsg = getStatusBar()->getMessage();

        if ( curMsg != msg )
          getStatusBar()->setMessage(msg);
      }
    }
  }
  else
  {
    if ( isEnabled() && getStatusBar() )
      getStatusBar()->clearMessage();
  }

  updateButtonColor();
  return enable;
}

//----------------------------------------------------------------------
bool FButton::setFlat (bool enable)
{
  return (flags.flat = enable);
}

//----------------------------------------------------------------------
bool FButton::setShadow (bool enable)
{
  if ( enable
    && getEncoding() != fc::VT100
    && getEncoding() != fc::ASCII )
  {
    flags.shadow = true;
    setShadowSize(FSize(1, 1));
  }
  else
  {
    flags.shadow = false;
    setShadowSize(FSize(0, 0));
  }

  return flags.shadow;
}

//----------------------------------------------------------------------
bool FButton::setDown (bool enable)
{
  if ( button_down != enable )
  {
    button_down = enable;
    redraw();
  }

  return enable;
}

//----------------------------------------------------------------------
void FButton::setText (const FString& txt)
{
  if ( txt.isNull() )
    text = "";
  else
    text = txt;

  detectHotkey();
}

//----------------------------------------------------------------------
void FButton::hide()
{
  std::size_t s, f, size;
  FColor fg, bg;
  auto parent_widget = getParentWidget();
  FWidget::hide();

  if ( parent_widget )
  {
    fg = parent_widget->getForegroundColor();
    bg = parent_widget->getBackgroundColor();
  }
  else
  {
    fg = wc.dialog_fg;
    bg = wc.dialog_bg;
  }

  setColor (fg, bg);
  s = hasShadow() ? 1 : 0;
  f = isFlat() ? 1 : 0;
  size = getWidth() + s + (f << 1);

  if ( size == 0 )
    return;

  char* blank = createBlankArray(size + 1);

  for (std::size_t y = 0; y < getHeight() + s + (f << 1); y++)
  {
    print() << FPoint(1 - int(f), 1 + int(y - f)) << blank;
  }

  destroyBlankArray (blank);
}

//----------------------------------------------------------------------
void FButton::onKeyPress (FKeyEvent* ev)
{
  if ( ! isEnabled() )
    return;

  FKey key = ev->key();

  switch ( key )
  {
    case fc::Fkey_return:
    case fc::Fkey_enter:
    case fc::Fkey_space:
      if ( click_animation )
      {
        setDown();
        addTimer(click_time);
      }
      processClick();
      ev->accept();
      break;

    default:
      break;
  }
}

//----------------------------------------------------------------------
void FButton::onMouseDown (FMouseEvent* ev)
{
  if ( ev->getButton() != fc::LeftButton )
  {
    setUp();
    return;
  }

  if ( ! hasFocus() )
  {
    auto focused_widget = getFocusWidget();
    setFocus();

    if ( focused_widget )
      focused_widget->redraw();

    if ( getStatusBar() )
      getStatusBar()->drawMessage();
  }

  FPoint tPos = ev->getTermPos();

  if ( getTermGeometry().contains(tPos) )
    setDown();
}

//----------------------------------------------------------------------
void FButton::onMouseUp (FMouseEvent* ev)
{
  if ( ev->getButton() != fc::LeftButton )
    return;

  if ( button_down )
  {
    setUp();

    if ( getTermGeometry().contains(ev->getTermPos()) )
      processClick();
  }
}

//----------------------------------------------------------------------
void FButton::onMouseMove (FMouseEvent* ev)
{
  if ( ev->getButton() != fc::LeftButton )
    return;

  FPoint tPos = ev->getTermPos();

  if ( click_animation )
  {
    if ( getTermGeometry().contains(tPos) )
      setDown();
    else
      setUp();
  }
}

//----------------------------------------------------------------------
void FButton::onTimer (FTimerEvent* ev)
{
  delTimer(ev->getTimerId());
  setUp();
}

//----------------------------------------------------------------------
void FButton::onAccel (FAccelEvent* ev)
{
  if ( ! isEnabled() )
    return;

  if ( ! hasFocus() )
  {
    auto focused_widget = static_cast<FWidget*>(ev->focusedWidget());

    if ( focused_widget && focused_widget->isWidget() )
    {
      setFocus();
      focused_widget->redraw();

      if ( click_animation )
        setDown();
      else
        redraw();

      if ( getStatusBar() )
        getStatusBar()->drawMessage();
    }
  }
  else if ( click_animation )
    setDown();

  if ( click_animation )
    addTimer(click_time);

  processClick();
  ev->accept();
}

//----------------------------------------------------------------------
void FButton::onFocusIn (FFocusEvent*)
{
  if ( getStatusBar() )
    getStatusBar()->drawMessage();
}

//----------------------------------------------------------------------
void FButton::onFocusOut (FFocusEvent*)
{
  if ( getStatusBar() )
  {
    getStatusBar()->clearMessage();
    getStatusBar()->drawMessage();
  }
}


// private methods of FButton
//----------------------------------------------------------------------
void FButton::init()
{
  setForegroundColor (wc.button_active_fg);
  setBackgroundColor (wc.button_active_bg);
  setShadow();

  if ( ! text.isEmpty() )
    detectHotkey();
}

//----------------------------------------------------------------------
void FButton::setHotkeyAccelerator()
{
  FKey hotkey = getHotkey(text);

  if ( hotkey )
  {
    if ( std::isalpha(int(hotkey)) || std::isdigit(int(hotkey)) )
    {
      addAccelerator (FKey(std::tolower(int(hotkey))));
      addAccelerator (FKey(std::toupper(int(hotkey))));
      // Meta + hotkey
      addAccelerator (fc::Fmkey_meta + FKey(std::tolower(int(hotkey))));
    }
    else
      addAccelerator (hotkey);
  }
  else
    delAccelerator();
}

//----------------------------------------------------------------------
inline void FButton::detectHotkey()
{
  if ( isEnabled() )
  {
    delAccelerator();
    setHotkeyAccelerator();
  }
}

//----------------------------------------------------------------------
std::size_t FButton::getHotkeyPos ( wchar_t src[]
                                  , wchar_t dest[]
                                  , std::size_t length )
{
  // find hotkey position in string
  // + generate a new string without the '&'-sign
  wchar_t* txt = src;
  std::size_t pos = NOT_SET;

  for (std::size_t i = 0; i < length; i++)
  {
    if ( i < length && txt[i] == L'&' && pos == NOT_SET )
    {
      pos = i;
      i++;
      src++;
    }

    *dest++ = *src++;
  }

  return pos;
}

//----------------------------------------------------------------------
inline std::size_t FButton::clickAnimationIndent (FWidget* parent_widget)
{
  if ( ! button_down || ! click_animation )
    return 0;

  // noshadow + indent one character to the right
  if ( flags.flat )
    clearFlatBorder();
  else if ( hasShadow() )
    clearShadow();

  if ( parent_widget )
    setColor ( parent_widget->getForegroundColor()
             , parent_widget->getBackgroundColor() );

  for (std::size_t  y = 1; y <= getHeight(); y++)
  {
    print() << FPoint(1, int(y)) << ' ';  // clear one left █
  }

  return 1;
}

//----------------------------------------------------------------------
inline void FButton::clearRightMargin (FWidget* parent_widget)
{
  if ( button_down || isNewFont() )
    return;

  // Restore the right background after button down
  if ( parent_widget )
    setColor ( parent_widget->getForegroundColor()
             , parent_widget->getBackgroundColor() );

  for (int y = 1; y <= int(getHeight()); y++)
  {
    if ( isMonochron() )
      setReverse(true);  // Light background

    print() << FPoint(1 + int(getWidth()), y) << ' ';  // clear right

    if ( flags.active && isMonochron() )
      setReverse(false);  // Dark background
  }
}

//----------------------------------------------------------------------
inline void FButton::drawMarginLeft()
{
  // Print left margin

  setColor (getForegroundColor(), button_bg);

  for (std::size_t y = 0; y < getHeight(); y++)
  {
    print() << FPoint(1 + int(indent), 1 + int(y));

    if ( isMonochron() && active_focus && y == vcenter_offset )
      print (fc::BlackRightPointingPointer);  // ►
    else
      print (space_char);  // full block █
  }
}

//----------------------------------------------------------------------
inline void FButton::drawMarginRight()
{
  // Print right margin

  for (std::size_t y = 0; y < getHeight(); y++)
  {
    print() << FPoint(int(getWidth() + indent), 1 + int(y));

    if ( isMonochron() && active_focus && y == vcenter_offset )
      print (fc::BlackLeftPointingPointer);   // ◄
    else
      print (space_char);  // full block █
  }
}

//----------------------------------------------------------------------
inline void FButton::drawTopBottomBackground()
{
  // Print top and bottom button background

  if ( getHeight() < 2 )
    return;

  for (std::size_t y = 0; y < vcenter_offset; y++)
  {
    print() << FPoint(2 + int(indent), 1 + int(y));

    for (std::size_t x = 1; x < getWidth() - 1; x++)
      print (space_char);  // █
  }

  for (std::size_t y = vcenter_offset + 1; y < getHeight(); y++)
  {
    print() << FPoint(2 + int(indent), 1 + int(y));

    for (std::size_t x = 1; x < getWidth() - 1; x++)
      print (space_char);  // █
  }
}

//----------------------------------------------------------------------
inline void FButton::drawButtonTextLine (wchar_t button_text[])
{
  std::size_t pos;
  print() << FPoint(2 + int(indent), 1 + int(vcenter_offset))
          << FColorPair (button_fg, button_bg);

  if ( getWidth() < txtlength + 1 )
    center_offset = 0;
  else
    center_offset = (getWidth() - txtlength - 1) / 2;

  // Print button text line
  for (pos = 0; pos < center_offset; pos++)
    print (space_char);  // █

  if ( hotkeypos == NOT_SET )
    setCursorPos (FPoint ( 2 + int(center_offset)
                         , 1 + int(vcenter_offset) ));  // first character
  else
    setCursorPos (FPoint ( 2 + int(center_offset + hotkeypos)
                         , 1 + int(vcenter_offset) ));  // hotkey

  if ( ! flags.active && isMonochron() )
    setReverse(true);  // Light background

  if ( active_focus && (isMonochron() || getMaxColor() < 16) )
    setBold();

  for ( std::size_t z = 0
      ; pos < center_offset + txtlength && z < getWidth() - 2
      ; z++, pos++)
  {
    if ( z == hotkeypos && flags.active )
    {
      setColor (button_hotkey_fg, button_bg);

      if ( ! active_focus && getMaxColor() < 16 )
        setBold();

      if ( ! flags.no_underline )
        setUnderline();

      print (button_text[z]);

      if ( ! active_focus && getMaxColor() < 16 )
        unsetBold();

      if ( ! flags.no_underline )
        unsetUnderline();

      setColor (button_fg, button_bg);
    }
    else
    {
      print (button_text[z]);
    }
  }

  if ( txtlength >= getWidth() - 1 )
  {
    // Print ellipsis
    print() << FPoint(int(getWidth() + indent) - 2, 1) << "..";
  }

  if ( active_focus && (isMonochron() || getMaxColor() < 16) )
    unsetBold();

  for (pos = center_offset + txtlength; pos < getWidth() - 2; pos++)
    print (space_char);  // █
}

//----------------------------------------------------------------------
void FButton::draw()
{
  wchar_t* button_text;
  auto parent_widget = getParentWidget();
  txtlength = text.getLength();
  space_char = int(' ');
  active_focus = flags.active && flags.focus;

  try
  {
    button_text = new wchar_t[txtlength + 1]();
  }
  catch (const std::bad_alloc& ex)
  {
    std::cerr << bad_alloc_str << ex.what() << std::endl;
    return;
  }

  if ( isMonochron() )
    setReverse(true);  // Light background

  // Click animation preprocessing
  indent = clickAnimationIndent (parent_widget);

  // Clear right margin after animation
  clearRightMargin (parent_widget);

  if ( ! flags.active && isMonochron() )
    space_char = fc::MediumShade;  // ▒ simulates greyed out at Monochron

  if ( isMonochron() && (flags.active || flags.focus) )
    setReverse(false);  // Dark background

  if ( flags.flat && ! button_down )
    drawFlatBorder();

  hotkeypos = getHotkeyPos(text.wc_str(), button_text, uInt(txtlength));

  if ( hotkeypos != NOT_SET )
    txtlength--;

  if ( getHeight() >= 2 )
    vcenter_offset = (getHeight() - 1) / 2;
  else
    vcenter_offset = 0;

  // Print left margin
  drawMarginLeft();

  // Print button text line
  drawButtonTextLine(button_text);

  // Print right margin
  drawMarginRight();

  // Print top and bottom button background
  drawTopBottomBackground();

  // Draw button shadow
  if ( ! flags.flat && flags.shadow && ! button_down )
    drawShadow();

  if ( isMonochron() )
    setReverse(false);  // Dark background

  delete[] button_text;
  updateStatusBar();
}

//----------------------------------------------------------------------
void FButton::updateStatusBar()
{
  if ( ! flags.focus || ! getStatusBar() )
    return;

  const auto& msg = getStatusbarMessage();
  const auto& curMsg = getStatusBar()->getMessage();

  if ( curMsg != msg )
  {
    getStatusBar()->setMessage(msg);
    getStatusBar()->drawMessage();
  }
}

//----------------------------------------------------------------------
void FButton::updateButtonColor()
{
  if ( isEnabled() )
  {
    if ( hasFocus() )
    {
      button_fg = button_focus_fg;
      button_bg = button_focus_bg;
    }
    else
    {
      button_fg = getForegroundColor();
      button_bg = getBackgroundColor();
    }
  }
  else  // inactive
  {
    button_fg = button_inactive_fg;
    button_bg = button_inactive_bg;
  }
}

//----------------------------------------------------------------------
void FButton::processClick()
{
  emitCallback("clicked");
}

}  // namespace finalcut

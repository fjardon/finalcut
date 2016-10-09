// File: fwidget.h
// Provides: class FWidget
//
//  Inheritance diagram
//  ═══════════════════
//
// ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▏
// ▕ FObject ▏ ▕  FTerm  ▏
// ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▏
//      ▲           ▲
//      │           │
//      └─────┬─────┘
//            │
//       ▕▔▔▔▔▔▔▔▔▔▏ 1      1▕▔▔▔▔▔▔▔▔▔▔▔▔▏
//       ▕ FWidget ▏-┬- - - -▕ FStatusBar ▏
//       ▕▁▁▁▁▁▁▁▁▁▏ :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      1▕▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FMenuBar ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FString ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FKeyEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FMouseEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FWheelEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FFocusEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FAccelEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FResizeEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FShowEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FHideEvent ▏
//                   :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▔▔▔▔▔▏
//                   └- - - -▕ FCloseEvent ▏
//                           ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▏

#ifndef _FWIDGET_H
#define _FWIDGET_H

#include "fterm.h"


// Callback macros
#define _FUNCTION_CALLBACK(h) \
           reinterpret_cast<FWidget::FCallback>((h))

#define _METHOD_CALLBACK(i,h) \
           reinterpret_cast<FWidget*>((i)) \
         , reinterpret_cast<FWidget::FMemberCallback>((h))

// class forward declaration
class FStatusBar;
class FMenuBar;

//----------------------------------------------------------------------
// class FWidget
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FWidget : public FObject, public FTerm
{
 public:
   typedef std::vector<FWidget*> widgetList;
   static widgetList* window_list;
   static widgetList* dialog_list;
   static widgetList* always_on_top_list;
   static widgetList* close_widget;

   typedef void (*FCallback)(FWidget*, void*);
   typedef void (FWidget::*FMemberCallback)(FWidget*, void*);
   typedef void* data_ptr;

   struct callback_data
   {
     FString   cb_signal;
     FCallback cb_handler;
     data_ptr  data;
   };

   typedef std::vector<callback_data> CallbackObjects;
   CallbackObjects callback_objects;

   struct member_callback_data
   {
     FString         cb_signal;
     FWidget*        cb_instance;
     FMemberCallback cb_handler;
     data_ptr        data;
   };

   typedef std::vector<member_callback_data> MemberCallbackObjects;
   MemberCallbackObjects member_callback_objects;

   struct accelerator
   {
     int key;
     FWidget* object;
   };

   typedef std::vector<accelerator> Accelerators;
   Accelerators* accelerator_list;

 protected:
   static struct widget_colors
   {
     short term_fg;
     short term_bg;
     short list_fg;
     short list_bg;
     short selected_list_fg;
     short selected_list_bg;
     short current_element_focus_fg;
     short current_element_focus_bg;
     short current_element_fg;
     short current_element_bg;
     short current_inc_search_element_fg;
     short selected_current_element_focus_fg;
     short selected_current_element_focus_bg;
     short selected_current_element_fg;
     short selected_current_element_bg;
     short label_fg;
     short label_bg;
     short label_inactive_fg;
     short label_inactive_bg;
     short label_hotkey_fg;
     short label_hotkey_bg;
     short label_emphasis_fg;
     short label_ellipsis_fg;
     short inputfield_active_focus_fg;
     short inputfield_active_focus_bg;
     short inputfield_active_fg;
     short inputfield_active_bg;
     short inputfield_inactive_fg;
     short inputfield_inactive_bg;
     short dialog_fg;
     short dialog_resize_fg;
     short dialog_emphasis_fg;
     short dialog_bg;
     short error_box_fg;
     short error_box_emphasis_fg;
     short error_box_bg;
     short tooltip_fg;
     short tooltip_bg;
     short shadow_fg;
     short shadow_bg;
     short toggle_button_active_focus_fg;
     short toggle_button_active_focus_bg;
     short toggle_button_active_fg;
     short toggle_button_active_bg;
     short toggle_button_inactive_fg;
     short toggle_button_inactive_bg;
     short button_active_focus_fg;
     short button_active_focus_bg;
     short button_active_fg;
     short button_active_bg;
     short button_inactive_fg;
     short button_inactive_bg;
     short button_hotkey_fg;
     short titlebar_active_fg;
     short titlebar_active_bg;
     short titlebar_inactive_fg;
     short titlebar_inactive_bg;
     short titlebar_button_fg;
     short titlebar_button_bg;
     short titlebar_button_focus_fg;
     short titlebar_button_focus_bg;
     short menu_active_focus_fg;
     short menu_active_focus_bg;
     short menu_active_fg;
     short menu_active_bg;
     short menu_inactive_fg;
     short menu_inactive_bg;
     short menu_hotkey_fg;
     short menu_hotkey_bg;
     short statusbar_fg;
     short statusbar_bg;
     short statusbar_hotkey_fg;
     short statusbar_hotkey_bg;
     short statusbar_separator_fg;
     short statusbar_active_fg;
     short statusbar_active_bg;
     short statusbar_active_hotkey_fg;
     short statusbar_active_hotkey_bg;
     short scrollbar_fg;
     short scrollbar_bg;
     short scrollbar_button_fg;
     short scrollbar_button_bg;
     short progressbar_fg;
     short progressbar_bg;
   } wc;
   // widget_colors wc;

   int     flags;
   static  uInt modal_dialogs;

 private:
   bool    enable;
   bool    visible;
   bool    shown;
   bool    focus;
   bool    focusable;
   bool    visible_cursor;
   FPoint  widget_cursor_position;

   struct widget_size_hints
   {
     widget_size_hints()
     : min_width  (INT_MIN)
     , min_height (INT_MIN)
     , max_width  (INT_MAX)
     , max_height (INT_MAX)
     { }
    ~widget_size_hints()
     { }
     void setMinimum (int w, int h)
     {
       min_width = w;
       min_height = h;
     }
     void setMaximum (int w, int h)
     {
       max_width = w;
       max_height = h;
     }
     int min_width;
     int min_height;
     int max_width;
     int max_height;
   } size_hints;

   struct dbl_line_mask
   {
     dbl_line_mask() : top(), right(), bottom(), left()
     { }
    ~dbl_line_mask()
     { }
     std::vector<bool> top;
     std::vector<bool> right;
     std::vector<bool> bottom;
     std::vector<bool> left;
   } double_flatline_mask;

   struct widget_padding
   {
     widget_padding() : top(0), left(0), bottom(0), right(0)
     { }
    ~widget_padding()
     { }
     int top;
     int left;
     int bottom;
     int right;
   } padding;

   bool    ignore_padding;

   // widget size
   FRect   wsize;
   FRect   adjust_wsize;
   FRect   adjust_wsize_term;
   FRect   adjust_wsize_shadow;
   FRect   adjust_wsize_term_shadow;
   // widget offset
   FRect   offset;
   // offset of the widget client area
   FRect   client_offset;
   // widget shadow size (on the right and bottom side)
   FPoint  wshadow;

   // default widget foreground and background color
   short   foreground_color;
   short   background_color;

   term_area* print_area;
   FString    statusbar_message;

   static FStatusBar* statusbar;
   static FMenuBar*   menubar;
   static FWidget*    show_root_widget;
   static FWidget*    redraw_root_widget;

   friend class FTerm;
   friend class FApplication;
   friend class FToggleButton;

 private:
   // Disable copy constructor
   FWidget (const FWidget&);
   // Disable assignment operator (=)
   FWidget& operator = (const FWidget&);

   void             init();
   void             finish();
   void             processDestroy();
   virtual void     draw();
   static void      setColorTheme();
   term_area*       getPrintArea();
   void             setPrintArea (term_area*);

 protected:
   virtual void     adjustSize();
   void             adjustSizeGlobal();
   virtual void     setStatusBar (FStatusBar*);
   virtual void     setMenuBar (FMenuBar*);
   // Event handlers
   bool             event (FEvent*);
   virtual void     onKeyPress (FKeyEvent*);
   virtual void     onKeyUp (FKeyEvent*);
   virtual void     onKeyDown (FKeyEvent*);
   virtual void     onMouseDown (FMouseEvent*);
   virtual void     onMouseUp (FMouseEvent*);
   virtual void     onMouseDoubleClick (FMouseEvent*);
   virtual void     onWheel (FWheelEvent*);
   virtual void     onMouseMove (FMouseEvent*);
   virtual void     onFocusIn (FFocusEvent*);
   virtual void     onFocusOut (FFocusEvent*);
   virtual void     onAccel (FAccelEvent*);
   virtual void     onResize (FResizeEvent*);
   virtual void     onShow (FShowEvent*);
   virtual void     onHide (FHideEvent*);
   virtual void     onClose (FCloseEvent*);
   // Change child focus
   virtual bool     focusNextChild();
   virtual bool     focusPrevChild();

 public:
   // Constructor
   explicit FWidget (FWidget* = 0);
   // Destructor
  ~FWidget();

   const char*      getClassName() const;
   FWidget*         getRootWidget() const;
   static FWidget*  getMainWidget();
   static void      setMainWidget (FWidget*);
   static FWidget*  childWidgetAt (FWidget*, const FPoint&);
   static FWidget*  childWidgetAt (FWidget*, int, int);
   virtual FWidget* getFocusWidget() const;
   virtual void     setFocusWidget (FWidget*);
   static FWidget*  getClickedWidget();
   static void      setClickedWidget (FWidget*);
   static FWidget*  getMoveSizeWidget();
   static void      setMoveSizeWidget (FWidget*);
   static FWidget*  getOpenMenu();
   static void      setOpenMenu (FWidget*);
   int              numOfFocusableChildren();
   FWidget*         getParentWidget() const;
   bool             isRootWidget() const;
   bool             isWindowWidget() const;
   bool             isDialogWidget() const;
   bool             isMenuWidget() const;
   virtual bool     close();

   static FStatusBar* statusBar();
   static FMenuBar* menuBar();
   virtual void     setStatusbarMessage (FString);
   void             clearStatusbarMessage();
   FString          getStatusbarMessage();

   void             addCallback ( FString
                                , FCallback
                                , void* = null );
   void             addCallback ( FString
                                , FWidget*
                                , FMemberCallback
                                , void* = null );
   void             delCallback (FCallback);
   void             delCallback (FWidget*);
   void             delCallbacks();
   void             emitCallback (FString);

   void             addAccelerator (int);
   virtual void     addAccelerator (int, FWidget*);
   void             delAccelerator ();
   virtual void     delAccelerator (FWidget*);

   virtual void     redraw();
   virtual void     resize();
   virtual void     show();
   virtual void     hide();
   bool             setVisible();
   bool             isVisible() const;
   bool             isShown() const;

   virtual bool     setEnable(bool);
   virtual bool     setEnable();
   virtual bool     unsetEnable();
   virtual bool     setDisable();
   bool             isEnabled() const;

   // input cursor visibility for the widget
   virtual bool     setVisibleCursor(bool);
   virtual bool     setVisibleCursor();
   virtual bool     unsetVisibleCursor();
   bool             hasVisibleCursor() const;

   // widget focusing
   virtual bool     focusFirstChild();
   virtual bool     focusLastChild();
   virtual bool     setFocus (bool);
   virtual bool     setFocus();
   virtual bool     unsetFocus();
   bool             hasFocus() const;
   bool             acceptFocus() const;
   void             setFocusable();
   void             unsetFocusable();

   // ignore padding from the parent widget
   bool             ignorePadding (bool);
   bool             ignorePadding();
   bool             acceptPadding();
   bool             isPaddingIgnored();

   // get the primary widget colors
   short            getForegroundColor() const;
   short            getBackgroundColor() const;

   // positioning
   int              getX() const;
   int              getY() const;
   const FPoint     getPos() const;
   int              getTermX() const;
   int              getTermY() const;
   const FPoint     getTermPos() const;
   int              getWidth() const;
   int              getHeight() const;
   int              getTopPadding() const;
   int              getLeftPadding() const;
   int              getBottomPadding() const;
   int              getRightPadding() const;
   int              getClientWidth() const;
   int              getClientHeight() const;
   int              getMaxWidth() const;
   int              getMaxHeight() const;
   const FPoint&    getShadow() const;
   const FRect&     getGeometry() const;
   const FRect&     getGeometryWithShadow();
   const FRect&     getTermGeometry();
   const FRect&     getTermGeometryWithShadow();
   FPoint           termToWidgetPos (const FPoint&);
   void             setForegroundColor (short);
   void             setBackgroundColor (short);
   void             setColor();
   void             setColor (short, short);
   void             setX (int, bool = true);
   void             setY (int, bool = true);
   virtual void     setPos (const FPoint&, bool = true);
   virtual void     setPos (int, int, bool = true);
   virtual void     setWidth (int, bool = true);
   virtual void     setHeight (int, bool = true);
   virtual void     setSize (int, int, bool = true);
   void             setTopPadding (int, bool = true);
   void             setLeftPadding (int, bool = true);
   void             setBottomPadding (int, bool = true);
   void             setRightPadding (int, bool = true);
   void             setParentOffset();
   void             setTermOffset();
   void             setTermOffsetWithPadding();
   void             getTermSize();
   void             setTermSize (int, int);
   virtual void     setGeometry (const FRect&, bool = true);
   virtual void     setGeometry (int, int, int, int, bool = true);
   virtual void     setShadowSize (int, int);
   void             setMinimumSize (int, int);
   void             setMaximumSize (int, int);
   void             setFixedSize (int, int);
   virtual void     move (const FPoint&);
   virtual void     move (int, int);
   int              getFlags() const;

   FPoint           getCursorPos();
   bool             setCursorPos (const FPoint&);
   bool             setCursorPos (register int, register int);
   void             unsetCursorPos();

   void             printPos (const FPoint&);
   void             printPos (register int, register int);
   FPoint           getPrintPos() const;

   static void      setNormal();

   static bool      setBold (register bool);
   static bool      setBold();
   static bool      unsetBold();
   static bool      isBold();

   static bool      setDim (register bool);
   static bool      setDim();
   static bool      unsetDim();
   static bool      isDim();

   static bool      setItalic (register bool);
   static bool      setItalic();
   static bool      unsetItalic();
   static bool      isItalic();

   static bool      setUnderline (register bool);
   static bool      setUnderline();
   static bool      unsetUnderline();
   static bool      isUnderline();

   static bool      setBlink (register bool);
   static bool      setBlink();
   static bool      unsetBlink();
   static bool      isBlink();

   static bool      setReverse (register bool);
   static bool      setReverse();
   static bool      unsetReverse();
   static bool      isReverse();

   static bool      setStandout (register bool);
   static bool      setStandout();
   static bool      unsetStandout();
   static bool      isStandout();

   static bool      setInvisible (register bool);
   static bool      setInvisible();
   static bool      unsetInvisible();
   static bool      isInvisible();

   static bool      setProtected (register bool);
   static bool      setProtected();
   static bool      unsetProtected();
   static bool      isProtected();

   static bool      setCrossedOut (register bool);
   static bool      setCrossedOut();
   static bool      unsetCrossedOut();
   static bool      isCrossedOut();

   static bool      setDoubleUnderline (register bool);
   static bool      setDoubleUnderline();
   static bool      unsetDoubleUnderline();
   static bool      isDoubleUnderline();

   static bool      setAltCharset (register bool);
   static bool      setAltCharset();
   static bool      unsetAltCharset();
   static bool      isAltCharset();

   static bool      setPCcharset (register bool);
   static bool      setPCcharset();
   static bool      unsetPCcharset();
   static bool      isPCcharset();

   static bool      setTransparent (register bool);
   static bool      setTransparent();
   static bool      unsetTransparent();
   static bool      isTransparent();

   static bool      setTransShadow (register bool);
   static bool      setTransShadow();
   static bool      unsetTransShadow();
   static bool      isTransShadow();

   static bool      setInheritBackground (register bool);
   static bool      setInheritBackground();
   static bool      unsetInheritBackground();
   static bool      isInheritBackground();

   void             drawShadow();
   void             clearShadow();
   void             drawFlatBorder();
   void             clearFlatBorder();
   void             setDoubleFlatLine (int, bool = true);
   void             unsetDoubleFlatLine (int);
   void             setDoubleFlatLine (int, int, bool = true);
   void             unsetDoubleFlatLine (int, int);
   std::vector<bool>& doubleFlatLine_ref (int);
   virtual void     drawBorder (int, int, int, int);
   virtual void     drawBorder();

   static void      quit();
};

#pragma pack(pop)

// FWidget inline functions
//----------------------------------------------------------------------
inline void FWidget::processDestroy()
{ emitCallback("destroy"); }

//----------------------------------------------------------------------
inline void FWidget::setPrintArea (term_area* area)
{ print_area = area; }

//----------------------------------------------------------------------
inline const char* FWidget::getClassName() const
{ return "FWidget"; }

//----------------------------------------------------------------------
inline FWidget* FWidget::childWidgetAt (FWidget* p, const FPoint& pos)
{ return childWidgetAt (p, pos.getX(), pos.getY()); }

//----------------------------------------------------------------------
inline FWidget* FWidget::getParentWidget() const
{ return static_cast<FWidget*>(getParent()); }

//----------------------------------------------------------------------
inline bool FWidget::isRootWidget() const
{ return (! hasParent()); }

//----------------------------------------------------------------------
inline void FWidget::clearStatusbarMessage()
{ statusbar_message.clear(); }

//----------------------------------------------------------------------
inline FString FWidget::getStatusbarMessage()
{ return statusbar_message; }

//----------------------------------------------------------------------
inline void FWidget::addAccelerator (int key)
{ addAccelerator (key, this); }

//----------------------------------------------------------------------
inline void FWidget::delAccelerator()
{ delAccelerator(this); }

//----------------------------------------------------------------------
inline bool FWidget::setVisible()
{ return visible = true; }

//----------------------------------------------------------------------
inline bool FWidget::isVisible() const
{ return visible; }

//----------------------------------------------------------------------
inline bool FWidget::isShown() const
{ return shown; }

//----------------------------------------------------------------------
inline bool FWidget::isWindowWidget() const
{ return ((flags & fc::window_widget) != 0); }

//----------------------------------------------------------------------
inline bool FWidget::isDialogWidget() const
{ return ((flags & fc::dialog_widget) != 0); }

//----------------------------------------------------------------------
inline bool FWidget::isMenuWidget() const
{ return ((flags & fc::menu_widget) != 0); }

//----------------------------------------------------------------------
inline bool FWidget::isEnabled() const
{ return enable; }

//----------------------------------------------------------------------
inline bool FWidget::setVisibleCursor (bool on)
{ return visible_cursor = (on) ? true : false; }

//----------------------------------------------------------------------
inline bool FWidget::setVisibleCursor()
{ return setVisibleCursor(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetVisibleCursor()
{ return setVisibleCursor(false); }

//----------------------------------------------------------------------
inline bool FWidget::hasVisibleCursor() const
{ return visible_cursor; }

//----------------------------------------------------------------------
inline bool FWidget::setFocus()
{ return setFocus(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetFocus()
{ return setFocus(false); }

//----------------------------------------------------------------------
inline bool FWidget::hasFocus() const
{ return focus; }

//----------------------------------------------------------------------
inline bool FWidget::acceptFocus() const
{ return focusable; }

//----------------------------------------------------------------------
inline bool FWidget::ignorePadding (bool on)
{ return ignore_padding = on; }

//----------------------------------------------------------------------
inline bool FWidget::ignorePadding()
{ return ignore_padding = true; }

//----------------------------------------------------------------------
inline bool FWidget::acceptPadding()
{ return ignore_padding = false; }

//----------------------------------------------------------------------
inline bool FWidget::isPaddingIgnored()
{ return ignore_padding; }

//----------------------------------------------------------------------
inline void FWidget::setFocusable()
{ focusable = true; }

//----------------------------------------------------------------------
inline void FWidget::unsetFocusable()
{ focusable = false; }

//----------------------------------------------------------------------
inline short FWidget::getForegroundColor() const
{ return foreground_color; }

//----------------------------------------------------------------------
inline short FWidget::getBackgroundColor() const
{ return background_color; }

//----------------------------------------------------------------------
inline int FWidget::getX() const  // x-position relative to the widget
{ return adjust_wsize.getX(); }

//----------------------------------------------------------------------
inline int FWidget::getY() const  // y-position relative to the widget
{ return adjust_wsize.getY(); }

//----------------------------------------------------------------------
inline const FPoint FWidget::getPos() const  // position relative to the widget
{ return adjust_wsize.getPos(); }

//----------------------------------------------------------------------
inline int FWidget::getTermX() const  // x-position on terminal
{ return offset.getX1() + adjust_wsize.getX(); }

//----------------------------------------------------------------------
inline int FWidget::getTermY() const  // y-position on terminal
{ return offset.getY1() + adjust_wsize.getY(); }

//----------------------------------------------------------------------
inline const FPoint FWidget::getTermPos() const  // position on terminal
{ return FPoint(getTermX(), getTermY()); }

//----------------------------------------------------------------------
inline FPoint FWidget::termToWidgetPos (const FPoint& tPos)
{
  return FPoint ( tPos.getX() + 1 - offset.getX1() - adjust_wsize.getX()
                , tPos.getY() + 1 - offset.getY1() - adjust_wsize.getY() );
}

//----------------------------------------------------------------------
inline void FWidget::setForegroundColor (short color)
{
  // valid colors -1..254
  if ( color == fc::Default || color >> 8 == 0 )
    foreground_color = color;
}

//----------------------------------------------------------------------
inline void FWidget::setBackgroundColor (short color)
{
  // valid colors -1..254
  if ( color == fc::Default || color >> 8 == 0 )
    background_color = color;
}

//----------------------------------------------------------------------
inline void FWidget::setPos (const FPoint& p, bool adjust)
{ setPos (p.getX(), p.getY(), adjust); }

//----------------------------------------------------------------------
inline int FWidget::getWidth() const
{ return adjust_wsize.getWidth(); }

//----------------------------------------------------------------------
inline int FWidget::getHeight() const
{ return adjust_wsize.getHeight(); }

//----------------------------------------------------------------------
inline int FWidget::getTopPadding() const
{ return padding.top; }

//----------------------------------------------------------------------
inline int FWidget::getLeftPadding() const
{ return padding.left; }

//----------------------------------------------------------------------
inline int FWidget::getBottomPadding() const
{ return padding.bottom; }

//----------------------------------------------------------------------
inline int FWidget::getRightPadding() const
{ return padding.right; }

//----------------------------------------------------------------------
inline int FWidget::getClientWidth() const
{ return client_offset.getWidth(); }

//----------------------------------------------------------------------
inline int FWidget::getClientHeight() const
{ return client_offset.getHeight(); }

//----------------------------------------------------------------------
inline int FWidget::getMaxWidth() const
{ return offset.getWidth(); }

//----------------------------------------------------------------------
inline int FWidget::getMaxHeight() const
{ return offset.getHeight(); }

//----------------------------------------------------------------------
inline const FPoint& FWidget::getShadow() const
{ return wshadow; }

//----------------------------------------------------------------------
inline const FRect& FWidget::getGeometry() const
{ return adjust_wsize; }

//----------------------------------------------------------------------
inline const FRect& FWidget::getGeometryWithShadow()
{
  adjust_wsize_shadow.setCoordinates
  (
    adjust_wsize.x1_ref(),
    adjust_wsize.y1_ref(),
    adjust_wsize.x2_ref() + wshadow.x_ref(),
    adjust_wsize.y2_ref() + wshadow.y_ref()
  );
  return adjust_wsize_shadow;
}

//----------------------------------------------------------------------
inline const FRect& FWidget::getTermGeometry()
{
  adjust_wsize_term.setCoordinates
  (
    adjust_wsize.x1_ref() + offset.x1_ref(),
    adjust_wsize.y1_ref() + offset.y1_ref(),
    adjust_wsize.x2_ref() + offset.x1_ref(),
    adjust_wsize.y2_ref() + offset.y1_ref()
  );
  return adjust_wsize_term;
}

//----------------------------------------------------------------------
inline const FRect& FWidget::getTermGeometryWithShadow()
{
  adjust_wsize_term_shadow.setCoordinates
  (
    adjust_wsize.x1_ref() + offset.x1_ref(),
    adjust_wsize.y1_ref() + offset.y1_ref(),
    adjust_wsize.x2_ref() + offset.x1_ref() + wshadow.x_ref(),
    adjust_wsize.y2_ref() + offset.y1_ref() + wshadow.y_ref()
  );
  return adjust_wsize_term_shadow;
}

//----------------------------------------------------------------------
inline void FWidget::setGeometry (const FRect& box, bool adjust)
{
  setGeometry ( box.getX()
              , box.getY()
              , box.getWidth()
              , box.getHeight()
              , adjust );
}

//----------------------------------------------------------------------
inline void FWidget::setShadowSize (int right, int bottom)
{ wshadow.setPoint (right, bottom); }

//----------------------------------------------------------------------
inline void FWidget::setMinimumSize (int min_width, int min_height)
{ size_hints.setMinimum (min_width, min_height); }

//----------------------------------------------------------------------
inline void FWidget::setMaximumSize (int max_width, int max_height)
{ size_hints.setMaximum (max_width, max_height); }

//----------------------------------------------------------------------
inline void FWidget::setFixedSize (int width, int height)
{
  size_hints.setMinimum (width, height);
  size_hints.setMaximum (width, height);
}

//----------------------------------------------------------------------
inline void FWidget::move (const FPoint& pos)
{ move( pos.getX(), pos.getY() ); }

//----------------------------------------------------------------------
inline int FWidget::getFlags() const
{ return flags; }

//----------------------------------------------------------------------
inline FPoint FWidget::getCursorPos()
{ return widget_cursor_position; }

//----------------------------------------------------------------------
inline bool FWidget::setCursorPos (const FPoint& pos)
{ return setCursorPos (pos.getX(), pos.getY()); }

//----------------------------------------------------------------------
inline void FWidget::unsetCursorPos()
{ widget_cursor_position.setPoint(-1,-1); }

//----------------------------------------------------------------------
inline void FWidget::printPos (const FPoint& pos)
{ printPos (pos.getX(), pos.getY()); }

//----------------------------------------------------------------------
inline void FWidget::printPos (register int x, register int y)
{
  cursor->setPoint ( offset.getX1() + adjust_wsize.getX() - 1 + x,
                     offset.getY1() + adjust_wsize.getY() - 1 + y );
}

//----------------------------------------------------------------------
inline FPoint FWidget::getPrintPos() const
{
  int cx = cursor->getX();
  int cy = cursor->getY();
  return FPoint ( cx - offset.getX1() - adjust_wsize.getX() + 1
                , cy - offset.getY1() - adjust_wsize.getY() + 1 );
}

//----------------------------------------------------------------------
inline void FWidget::setNormal()
{
  next_attribute.bold          = \
  next_attribute.dim           = \
  next_attribute.italic        = \
  next_attribute.underline     = \
  next_attribute.blink         = \
  next_attribute.reverse       = \
  next_attribute.standout      = \
  next_attribute.invisible     = \
  next_attribute.protect       = \
  next_attribute.crossed_out   = \
  next_attribute.dbl_underline = \
  next_attribute.alt_charset   = \
  next_attribute.pc_charset    = \
  next_attribute.transparent   = \
  next_attribute.trans_shadow  = \
  next_attribute.inherit_bg    = false;

  next_attribute.fg_color      = fc::Default;
  next_attribute.bg_color      = fc::Default;
}

//----------------------------------------------------------------------
inline bool FWidget::setBold (register bool on)
{ return (next_attribute.bold = on); }

//----------------------------------------------------------------------
inline bool FWidget::setBold()
{ return setBold(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetBold()
{ return setBold(false); }

//----------------------------------------------------------------------
inline bool FWidget::isBold()
{ return next_attribute.bold; }

//----------------------------------------------------------------------
inline bool FWidget::setDim (register bool on)
{ return (next_attribute.dim = on); }

//----------------------------------------------------------------------
inline bool FWidget::setDim()
{ return setDim(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetDim()
{ return setDim(false); }

//----------------------------------------------------------------------
inline bool FWidget::isDim()
{ return next_attribute.dim; }

//----------------------------------------------------------------------
inline bool FWidget::setItalic (register bool on)
{ return (next_attribute.italic = on); }

//----------------------------------------------------------------------
inline bool FWidget::setItalic()
{ return setItalic(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetItalic()
{ return setItalic(false); }

//----------------------------------------------------------------------
inline bool FWidget::isItalic()
{ return next_attribute.italic; }

//----------------------------------------------------------------------
inline bool FWidget::setUnderline (register bool on)
{ return (next_attribute.underline = on); }

//----------------------------------------------------------------------
inline bool FWidget::setUnderline()
{ return setUnderline(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetUnderline()
{ return setUnderline(false); }

//----------------------------------------------------------------------
inline bool FWidget::isUnderline()
{ return next_attribute.underline; }

//----------------------------------------------------------------------
inline bool FWidget::setBlink (register bool on)
{ return (next_attribute.blink = on); }

//----------------------------------------------------------------------
inline bool FWidget::setBlink()
{ return setBlink(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetBlink()
{ return setBlink(false); }

//----------------------------------------------------------------------
inline bool FWidget::isBlink()
{ return next_attribute.blink; }

//----------------------------------------------------------------------
inline bool FWidget::setReverse (register bool on)
{ return (next_attribute.reverse = on); }

//----------------------------------------------------------------------
inline bool FWidget::setReverse()
{ return setReverse(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetReverse()
{ return setReverse(false); }

//----------------------------------------------------------------------
inline bool FWidget::isReverse()
{ return next_attribute.reverse; }

//----------------------------------------------------------------------
inline bool FWidget::setStandout (register bool on)
{ return (next_attribute.standout = on); }

//----------------------------------------------------------------------
inline bool FWidget::setStandout()
{ return setStandout(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetStandout()
{ return setStandout(false); }

//----------------------------------------------------------------------
inline bool FWidget::isStandout()
{ return next_attribute.standout; }

//----------------------------------------------------------------------
inline bool FWidget::setInvisible (register bool on)
{ return (next_attribute.invisible = on); }

//----------------------------------------------------------------------
inline bool FWidget::setInvisible()
{ return setInvisible(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetInvisible()
{ return setInvisible(false); }

//----------------------------------------------------------------------
inline bool FWidget::isInvisible()
{ return next_attribute.invisible; }

//----------------------------------------------------------------------
inline bool FWidget::setProtected (register bool on)
{ return (next_attribute.protect = on); }

//----------------------------------------------------------------------
inline bool FWidget::setProtected()
{ return setProtected(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetProtected()
{ return setProtected(false); }

//----------------------------------------------------------------------
inline bool FWidget::isProtected()
{ return next_attribute.protect; }

//----------------------------------------------------------------------
inline bool FWidget::setCrossedOut (register bool on)
{ return (next_attribute.crossed_out = on); }

//----------------------------------------------------------------------
inline bool FWidget::setCrossedOut()
{ return setCrossedOut(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetCrossedOut()
{ return setCrossedOut(false); }

//----------------------------------------------------------------------
inline bool FWidget::isCrossedOut()
{ return next_attribute.crossed_out; }

//----------------------------------------------------------------------
inline bool FWidget::setDoubleUnderline (register bool on)
{ return (next_attribute.dbl_underline = on); }

//----------------------------------------------------------------------
inline bool FWidget::setDoubleUnderline()
{ return setDoubleUnderline(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetDoubleUnderline()
{ return setDoubleUnderline(false); }

//----------------------------------------------------------------------
inline bool FWidget::isDoubleUnderline()
{ return next_attribute.dbl_underline; }

//----------------------------------------------------------------------
inline bool FWidget::setAltCharset (register bool on)
{ return (next_attribute.alt_charset = on); }

//----------------------------------------------------------------------
inline bool FWidget::setAltCharset()
{ return setAltCharset(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetAltCharset()
{ return setAltCharset(false); }

//----------------------------------------------------------------------
inline bool FWidget::isAltCharset()
{ return next_attribute.alt_charset; }

//----------------------------------------------------------------------
inline bool FWidget::setPCcharset (register bool on)
{ return (next_attribute.pc_charset = on); }

//----------------------------------------------------------------------
inline bool FWidget::setPCcharset()
{ return setPCcharset(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetPCcharset()
{ return setPCcharset(false); }

//----------------------------------------------------------------------
inline bool FWidget::isPCcharset()
{ return next_attribute.pc_charset; }

//----------------------------------------------------------------------
inline bool FWidget::setTransparent (register bool on)
{ return (next_attribute.transparent = on); }

//----------------------------------------------------------------------
inline bool FWidget::setTransparent()
{ return setTransparent(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetTransparent()
{ return setTransparent(false); }

//----------------------------------------------------------------------
inline bool FWidget::isTransparent()
{ return next_attribute.transparent; }

//----------------------------------------------------------------------
inline bool FWidget::setTransShadow (register bool on)
{ return (next_attribute.trans_shadow = on); }

//----------------------------------------------------------------------
inline bool FWidget::setTransShadow()
{ return setTransShadow(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetTransShadow()
{ return setTransShadow(false); }

//----------------------------------------------------------------------
inline bool FWidget::isTransShadow()
{ return next_attribute.trans_shadow; }

//----------------------------------------------------------------------
inline bool FWidget::setInheritBackground (register bool on)
{ return (next_attribute.inherit_bg = on); }

//----------------------------------------------------------------------
inline bool FWidget::setInheritBackground()
{ return setInheritBackground(true); }

//----------------------------------------------------------------------
inline bool FWidget::unsetInheritBackground()
{ return setInheritBackground(false); }

//----------------------------------------------------------------------
inline bool FWidget::isInheritBackground()
{ return next_attribute.inherit_bg; }

//----------------------------------------------------------------------
inline void FWidget::unsetDoubleFlatLine (int side)
{ setDoubleFlatLine(side, false); }

//----------------------------------------------------------------------
inline void FWidget::unsetDoubleFlatLine (int side, int pos)
{ setDoubleFlatLine(side, pos, false); }

//----------------------------------------------------------------------
inline void FWidget::drawBorder()
{ drawBorder (1, 1, getWidth(), getHeight()); }


// NewFont elements
//----------------------------------------------------------------------
const wchar_t NF_Drive[5] =
{
  fc::NF_shadow_box_left,
  fc::NF_shadow_box_middle,
  fc::NF_shadow_box_hdd,
  fc::NF_shadow_box_right,
  '\0'
};

const wchar_t NF_CD_ROM[5] =
{
  fc::NF_shadow_box_left,
  fc::NF_shadow_box_middle,
  fc::NF_shadow_box_cd,
  fc::NF_shadow_box_right,
  '\0'
};

const wchar_t NF_Net_Drive[5] =
{
  fc::NF_shadow_box_left,
  fc::NF_shadow_box_middle,
  fc::NF_shadow_box_net,
  fc::NF_shadow_box_right,
  '\0'
};

const wchar_t CHECKBOX[4] =
{
  fc::NF_shadow_box_left,
  fc::NF_shadow_box_middle,
  fc::NF_shadow_box_right,
  '\0'
};

const wchar_t CHECKBOX_ON[4] =
{
  fc::NF_shadow_box_left,
  fc::NF_shadow_box_checked,
  fc::NF_shadow_box_right,
  '\0'
};

const wchar_t RADIO_BUTTON[4] =
{
  fc::NF_radio_button1,
  fc::NF_radio_button2,
  fc::NF_radio_button3,
  '\0'
};

const wchar_t CHECKED_RADIO_BUTTON[4] =
{
  fc::NF_radio_button1,
  fc::NF_radio_button2_checked,
  fc::NF_radio_button3,
  '\0'
};

#endif  // _FWIDGET_H

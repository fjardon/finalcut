// File: fmenubar.h
// Provides: class FMenuBar
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
//       ▕▔▔▔▔▔▔▔▔▔▏
//       ▕ FWidget ▏
//       ▕▁▁▁▁▁▁▁▁▁▏
//            ▲
//            │
//       ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▔▔▏1     *▕▔▔▔▔▔▔▔▔▔▔▔▏
//       ▕ FWindow ▏ ▕ FMenuList ▏- - - -▕ FMenuItem ▏
//       ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▁▁▏       ▕▁▁▁▁▁▁▁▁▁▁▁▏
//            ▲           ▲
//            │           │
//            └─────┬─────┘
//                  │
//            ▕▔▔▔▔▔▔▔▔▔▔▏
//            ▕ FMenuBar ▏
//            ▕▁▁▁▁▁▁▁▁▁▁▏

#ifndef _FMENUBAR_H
#define _FMENUBAR_H

#include "fmenu.h"
#include "fmenulist.h"
#include "fwindow.h"


//----------------------------------------------------------------------
// class FMenuBar
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FMenuBar : public FWindow, public FMenuList
{
 private:
   bool mouse_down;
   FMenuItem* selectedMenuItem;

 private:
   FMenuBar (const FMenuBar&);
   FMenuBar& operator = (const FMenuBar&);
   void init();
   void menu_dimension();
   bool isMenu (FMenuItem*) const;
   int  getHotkeyPos (wchar_t*&, wchar_t*&, uInt);
   void draw();
   void drawItems();
   void adjustSize();

 public:
   explicit FMenuBar (FWidget* = 0);  // constructor
   virtual ~FMenuBar();  // destructor
   virtual const char* getClassName() const;

   void onMouseDown (FMouseEvent*);
   void onMouseUp (FMouseEvent*);
   void onMouseMove (FMouseEvent*);
   void hide();
   bool hasSelectedMenuItem() const;
   // make every setGeometry from FWidget available
   using FWidget::setGeometry;
   void setGeometry (int, int, int, int, bool = true);
   void cb_item_activated (FWidget*, void*);
   void cb_item_deactivated (FWidget*, void*);

 private:
   friend class FMenuItem;
};
#pragma pack(pop)


// FMenuBar inline functions
//----------------------------------------------------------------------
inline const char* FMenuBar::getClassName() const
{ return "FMenuBar"; }

//----------------------------------------------------------------------
inline bool FMenuBar::hasSelectedMenuItem() const
{ return selectedMenuItem; }

#endif  // _FMENUBAR_H

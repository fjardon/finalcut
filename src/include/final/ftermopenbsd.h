/***********************************************************************
* ftermopenbsd.h - Contains the NetBSD/OpenBSD terminal functions      *
*                                                                      *
* This file is part of the Final Cut widget toolkit                    *
*                                                                      *
* Copyright 2018-2019 Markus Gans                                      *
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

/*  Standalone class
 *  ════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 * ▕ FTermOpenBSD ▏
 * ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FTERMOPENBSD_H
#define FTERMOPENBSD_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include <sys/ioctl.h>
#include "final/fsystem.h"

#if defined(UNIT_TEST)
  #define WSKBDIO_GETENCODING uInt32(0x4004570F)
  #define WSKBDIO_SETENCODING uInt32(0x80045710)
  typedef uInt32 kbd_t;
#elif defined(__NetBSD__) || defined(__OpenBSD__)
  #include <sys/time.h>
  #include <dev/wscons/wsconsio.h>
#endif

namespace finalcut
{

//----------------------------------------------------------------------
// class FTermOpenBSD
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FTermOpenBSD final
{
  public:
    // Constructors
    FTermOpenBSD() = default;

    // Disable copy constructor
    FTermOpenBSD (const FTermOpenBSD&) = delete;

    // Destructor
    virtual ~FTermOpenBSD() = default;

    // Disable assignment operator (=)
    FTermOpenBSD& operator = (const FTermOpenBSD&) = delete;

    // Accessor
    const char*        getClassName() const;

    // Inquiries
    static bool        isBSDConsole();

    // Mutators
    static void        disableMetaSendsEscape();
    static void        enableMetaSendsEscape();

    // Methods
    static void        init();
    static void        finish();

  private:
#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
    // Methods
    static bool        saveBSDConsoleEncoding();
    static bool        setBSDConsoleEncoding (kbd_t);
    static bool        setBSDConsoleMetaEsc();
    static bool        resetBSDConsoleEncoding();

    // Data Members
    static kbd_t       bsd_keyboard_encoding;
    static bool        meta_sends_escape;
    static FSystem*    fsystem;
#endif  // defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
};
#pragma pack(pop)

// FTermOpenBSD inline functions
//----------------------------------------------------------------------
inline const char* FTermOpenBSD::getClassName() const
{ return "FTermOpenBSD"; }

//----------------------------------------------------------------------
#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
inline void FTermOpenBSD::enableMetaSendsEscape()
{ meta_sends_escape = true; }

//----------------------------------------------------------------------
inline void FTermOpenBSD::disableMetaSendsEscape()
{ meta_sends_escape = false; }
#endif  // defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)

}  // namespace finalcut

#endif  // FTERMOPENBSD_H

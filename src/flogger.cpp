/***********************************************************************
* flogger.cpp - The FINAL CUT text logger                              *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2020-2021 Markus Gans                                      *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <array>
#include <string>

#include "final/flogger.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FLogger
//----------------------------------------------------------------------

// constructors and destructor
//----------------------------------------------------------------------
FLogger::~FLogger() noexcept = default;  // destructor


// private methods of FLogger
//----------------------------------------------------------------------
void FLogger::newlineReplace ( std::string& str
                             , const std::string& replace_str ) const
{
  std::size_t pos{0};
  std::size_t npos{std::string::npos};

  while ( (pos = str.find("\n", pos)) != npos
       && pos + 1 < str.length() )
  {
    str.replace(pos, 1, replace_str);
    pos += replace_str.length();
  }
}

//----------------------------------------------------------------------
std::string FLogger::getTimeString() const
{
  std::array<char, 100> str;
  const auto& now = std::chrono::system_clock::now();
  const auto& t = std::chrono::system_clock::to_time_t(now);
  // Print RFC 2822 date
  struct tm time{};
  localtime_r (&t, &time);
  std::strftime (str.data(), str.size(), "%a, %d %b %Y %T %z", &time);
  return {str.data()};
}

//----------------------------------------------------------------------
std::string FLogger::getEOL()
{
  std::lock_guard<std::mutex> lock_guard(getMutex());

  if ( getEnding() == LineEnding::LF )
    return "\n";
  else if ( getEnding() == LineEnding::CR )
    return "\r";
  else if ( getEnding() == LineEnding::CRLF )
    return "\r\n";

  return "";
}

//----------------------------------------------------------------------
void FLogger::printLogLine (const std::string& msg)
{
  const std::string& log_level = [this] ()
  {
    std::lock_guard<std::mutex> lock_guard(getMutex());

    switch ( getLevel() )
    {
      case LogLevel::Info:
        return "INFO";

      case LogLevel::Warn:
        return "WARNING";

      case LogLevel::Error:
        return "ERROR";

      case LogLevel::Debug:
        return "DEBUG";
    }

    return "";
  }();

  const std::string prefix = [this, &log_level] ()
  {
    if ( timestamp )
      return getTimeString() + " [" + log_level + "] ";
    else
      return "[" + log_level + "] ";
  }();

  std::string message{msg};
  const std::string& eol = getEOL();
  const std::string replace_str = eol + prefix;
  newlineReplace (message, replace_str);
  output << prefix << message << eol;
}

}  // namespace finalcut



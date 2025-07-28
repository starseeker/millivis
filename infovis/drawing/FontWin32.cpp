/* -*- C++ -*-
 *
 * Copyright (C) 2016 Jean-Daniel Fekete
 * 
 * This file is part of MillionVis.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <infovis/drawing/FontWin32.hpp>

#if defined(WIN32) || defined(_WIN32)

#include <windows.h>
#include <GL/gl.h>
#include <wingdi.h>

namespace infovis {

FontWin32::FontWin32(void * h,
		     const string& n, Style s, float sz)
  : Font(n, s, sz),
    hfont(h),
    base(0),
    ascent(0),
    descent(0)
{
  HWND hwnd = GetDesktopWindow();
  HDC hdc = GetDC(hwnd);

  HFONT prevfnt = (HFONT)SelectObject(hdc, h);

  base = glGenLists(256);
  if (wglUseFontBitmaps (hdc, 0, 255, base) == FALSE) {
    throw false;
  }
  TEXTMETRIC tm;
  GetTextMetrics(hdc, &tm);
  ascent = tm.tmAscent;
  descent = tm.tmDescent;
  GetCharWidth32(hdc, 0, 255, widths);
  SelectObject(hdc, prevfnt);
  ReleaseDC(hwnd, hdc);
}

FontWin32::~FontWin32()
{
  if (base != 0)
    glDeleteLists(base, 255);
}

bool
FontWin32::install()
{
  return true;
}

void
FontWin32::finish() {}

Font::Format
FontWin32::getFormat() const
{
  return format_bitmap;
}

bool
FontWin32::isFixedWidth() const
{
  return false;			// +++FIXME
}

void
FontWin32::paint(const string& str, float x, float y)
{
  HWND hwnd = GetDesktopWindow();
  HDC hdc = GetDC(hwnd);

  glListBase(base);
  glRasterPos2f(x, y);
  glCallLists (str.length(), GL_UNSIGNED_BYTE, str.c_str());
  ReleaseDC(hwnd, hdc);
}

float
FontWin32::getLeading() const
{
  return 0;
}

float
FontWin32::getAscent() const
{
  return ascent;
}

float
FontWin32::getDescent() const
{
  return descent;
}

float
FontWin32::charWidth(int ch) const
{
  if (ch < 0 || ch > 255)
    return 0;
  return widths[ch];
}

float
FontWin32::stringWidth(const string& str)
{
  float w = 0;
  for (string::const_iterator i = str.begin();
       i != str.end(); i++) {
    w += charWidth(*i);
  }
  return w;
}

Box
FontWin32::getStringBounds(const string& str)
{
  return Box(0, -descent,
	     stringWidth(str), ascent);
}


Font *
FontWin32Creator::create(const string& name, Font::Style style, float size)
{
  if (name == "default" ||
      name == "SYSTEM_FONT" ||
      name == "system_font" ||
      name == "SYSTEM FONT" ||
      name == "system font") {
    static Font * def;
    if (def == 0)
      def = new FontWin32(GetStockObject(SYSTEM_FONT),
			  name, style, size);
    return def;
  }
  LOGFONT log;

  log.lfHeight	= -int(size);
  log.lfWidth	= 0;
  log.lfEscapement 	= 0 ;
  log.lfOrientation 	= 0 ;
  log.lfWeight 		=
    (style == Font::style_bold ||
     style == Font::style_bold_italic) ? FW_BOLD : FW_NORMAL ;
  log.lfItalic 		=
    (style == Font::style_italic ||
     style == Font::style_bold_italic) ? TRUE : FALSE ;
  log.lfUnderline 	= FALSE ;
  log.lfStrikeOut 	= FALSE ;
  log.lfCharSet		= ANSI_CHARSET ;
  log.lfOutPrecision = OUT_DEFAULT_PRECIS ;
  log.lfClipPrecision= CLIP_DEFAULT_PRECIS ;
  log.lfQuality 		= PROOF_QUALITY ;
  log.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE ;
  strncpy(log.lfFaceName, name.c_str(), 32) ;  
  HFONT fnt = CreateFontIndirect(&log);
  if (fnt == 0)
    return 0;
  FontWin32 * f = 0;
  try {
    f = new FontWin32(fnt, name, style, size);
  }
  catch(...) {}
  return f;
}

FontWin32Creator * font_win32_creator;

void installFontWin32()
{
  font_win32_creator = new FontWin32Creator;
}

} // namespace infovis
#else
namespace infovis {
void installFontWin32()
{
}
} // namespace infovis
#endif // defined(WIN32) || defined(_WIN32)


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
#ifndef INFOVIS_DRAWING_FONTWIN32_HPP
#define INFOVIS_DRAWING_FONTWIN32_HPP

#include <infovis/drawing/Font.hpp>

struct _GLYPHMETRICS;

namespace infovis {

/**
 * Implementation of Win32 based OpenGL fonts.
 */
class FontWin32 : public Font
{
protected:
  void * hfont;
  int base;
  int ascent;
  int descent;
  int widths[256];
public:
  FontWin32(void * hfont, const string& name, Style style, float size);
  virtual ~FontWin32();
  
  virtual bool install();
  virtual void finish();

  virtual Format getFormat() const;
  virtual bool isFixedWidth() const;
  virtual void paint(const string& str, float x, float y);
  virtual float getLeading() const;
  virtual float getAscent() const;
  virtual float getDescent() const;
  virtual float charWidth(int ch) const;
  virtual float stringWidth(const string& str);
  virtual Box getStringBounds(const string& str);
};

/**
 * Creator of Win32 fonts
 */
class FontWin32Creator : public Font::Creator {
public:
  FontWin32Creator() {}
  Font * create(const string& name, Font::Style style, float size);
};

extern void installFontWin32();

} // namespace infovis

#endif // INFOVIS_DRAWING_FONTWIN32_HPP

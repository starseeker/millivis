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
#include <infovis/drawing/Font.hpp>
//#include <infovis/drawing/FontGlut.hpp>
//#include <infovis/drawing/FontWin32.hpp>
//#include <infovis/drawing/FontFT.hpp>

//#define PRINT
#ifdef PRINT
#include <iostream>
#endif

namespace infovis {

Font::Font(const string& n, Style s, float sz)
  : name(n),
    style(s),
    size(sz) {}

Font::~Font() {}

void
Font::paintGrown(const string& str, float x, float y)
{
  paint(str, x, y);
}

float
Font::getHeight() const
{
  return getLeading() + getAscent() + getDescent();
}

Font::Creator::Creator()
{
  next = first;
  first = this;
}

extern void installFontGlut();
extern void installFontWin32();
extern void installFontFT();

Font *
Font::create(const string& name, Style style, float size)
{
  static bool inited;
  if (! inited) {
    installFontGlut();
    installFontWin32();
    installFontFT();
	inited = true;
  }
  if (size < 0) {
    size = -size;
  }
  else {
    size *= point_size;
  }
  static Font * deflt;
  if (deflt && name == "default")
    return deflt;

  Font * f = 0;
  for (Creator * c = Creator::first; c != 0; c = c->next) {
    if ((f = c->create(name, style, size)) != 0) {
#ifdef PRINT
      std::cout << "Font " << name << " Size " << size << " created by "
		<< typeid(*c).name() << std::endl;
#endif
      break;
    }
  }
  if (deflt == 0 && name == "default")
    deflt = f;
  return f;
}

Font *
Font::create(const string& name, const string& style, float size)
{
  if (style == "plain")
    return create(name, style_plain, size);
  else if (style == "bold")
    return create(name, style_bold, size);
  else if (style == "italic")
    return create(name, style_italic, size);
  else if (style == "bold italic")
    return create(name, style_bold_italic, size);
  return create(name, style_plain, size);
}

float Font::point_size = 1.0f;

void
Font::setPointSize(float ptsz) { point_size = ptsz; }
float
Font::getPointSize() { return point_size; }


Font::Creator::~Creator() {}

Font::Creator * Font::Creator::first;

void
Font::center(const string& str, float x, float y)
{
  float w = stringWidth(str);
  paint(str, x - w / 2, y);
}

void
Font::rightJustify(const string& str, float x, float y)
{
  float w = stringWidth(str);
  paint(str, x - w, y);
}

} // namespace infovis

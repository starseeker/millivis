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
#ifndef INFOVIS_DRAWING_FONTGLUT_HPP
#define INFOVIS_DRAWING_FONTGLUT_HPP

#include <infovis/drawing/Font.hpp>

namespace infovis {

/**
 * Implementation of Glut based stroked fonts.
 */
class FontGlutStroke : public Font
{
protected:
  void * type;
public:

  FontGlutStroke(void * t,
		 const string& name, Style style, float size);

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
 * Implementation of Glut based bitmap fonts.
 */
class FontGlutBitmap : public FontGlutStroke
{
protected:
  float ascent;
  float descent;
public:

  FontGlutBitmap(void * t,
		 float ascent, float descent,
		 const string& name, Style style, float size);

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
 * Creator or Glut based fonts.
 */
class FontGlutCreator : public Font::Creator {
public:
  FontGlutCreator() {}
  Font * create(const string& name, Font::Style style, float size);
};

extern void installFontGlut();



} // namespace infovis

#endif // INFOVIS_DRAWING_FONTGLUT_HPP

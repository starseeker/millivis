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
#include <infovis/drawing/FontGlut.hpp>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

namespace infovis {

static const float font_ascent = 119.05;
static const float font_descent = 33.33;
static const float font_height = 119.05 + 33.33;
static const float font_scale = 1.0f / (119.05 + 33.33);

FontGlutStroke::FontGlutStroke(void * t,
			       const string& name, Style style, float size)
  : Font(name, style, size),
    type(t) {}

bool
FontGlutStroke::install()
{
  return true;
}

void
FontGlutStroke::finish() {}

Font::Format
FontGlutStroke::getFormat() const
{
  return format_plotter;
}

bool
FontGlutStroke::isFixedWidth() const
{
  return type == GLUT_STROKE_MONO_ROMAN;
}

void
FontGlutStroke::paint(const string& str, float x, float y)
{
  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef(size * font_scale, size * font_scale, 1);
  for (string::const_iterator i = str.begin(); i != str.end(); i++) {
    glutStrokeCharacter(type, *i);
  }
  glPopMatrix();
}

float
FontGlutStroke::getLeading() const
{
  return 0;
}

float
FontGlutStroke::getAscent() const
{
  return font_ascent * font_scale * size;
}

float
FontGlutStroke::getDescent() const
{
  return font_descent * font_scale * size;
}

float
FontGlutStroke::charWidth(int ch) const
{
  return font_scale * size * glutStrokeWidth(type, ch);
}

float
FontGlutStroke::stringWidth(const string& str)
{
  int w = 0;
  for (string::const_iterator i = str.begin();
       i != str.end(); i++) {
    w += glutStrokeWidth(type, *i);
  }
  return w * font_scale * size;
}

Box
FontGlutStroke::getStringBounds(const string& str)
{
  return Box(0, -font_descent * font_scale * size,
	     stringWidth(str), font_ascent * font_scale * size);
}

FontGlutBitmap::FontGlutBitmap(void * t,
			       float a, float d,
			       const string& name, Style style, float size)
  : FontGlutStroke(t, name, style, size),
    ascent(a), descent(d) {}

Font::Format
FontGlutBitmap::getFormat() const
{
  return format_bitmap;
}

bool
FontGlutBitmap::isFixedWidth() const
{
  return
    type == GLUT_BITMAP_8_BY_13 ||
    type == GLUT_BITMAP_9_BY_15;
}

void
FontGlutBitmap::paint(const string& str, float x, float y)
{
  glRasterPos2f(x, y);
  for (string::const_iterator i = str.begin(); i != str.end(); i++) {
    glutBitmapCharacter(type, *i);
  }
}

float
FontGlutBitmap::getLeading() const
{
  return 0;
}

float
FontGlutBitmap::getAscent() const
{
  return ascent;
}

float
FontGlutBitmap::getDescent() const
{
  return descent;
}

float
FontGlutBitmap::charWidth(int ch) const
{
  return glutBitmapWidth(type, ch);
}

float
FontGlutBitmap::stringWidth(const string& str)
{
  int w = 0;
  for (string::const_iterator i = str.begin();
       i != str.end(); i++) {
    w += glutBitmapWidth(type, *i);
  }
  return w;
}

Box
FontGlutBitmap::getStringBounds(const string& str)
{
  return Box(0, -descent,
	     stringWidth(str), ascent);
}


Font *
FontGlutCreator::create(const string& name, Font::Style style, float size)
{
  if (name == "GLUT_STROKE_ROMAN" ||
      name == "GLUT STROKE ROMAN" ||
      name == "glut_stroke_roman" ||
      name == "default" ||
      name == "glut stroke roman")
    return new FontGlutStroke(GLUT_STROKE_ROMAN, name, style, size);
  if (name == "GLUT_STROKE_MONO_ROMAN" ||
      name == "GLUT STROKE MONO ROMAN" ||
      name == "glut_stroke_mono_roman" ||
      name == "glut stroke mono roman")
    return new FontGlutStroke(GLUT_STROKE_MONO_ROMAN, name, style, size);
  if (name == "GLUT_BITMAP_8_BY_13" ||
      name == "8x13")
    return new FontGlutBitmap(GLUT_BITMAP_8_BY_13, 7, 6,
			      name, style, size);
  return 0;
}

static FontGlutCreator * font_glut_creator;

void installFontGlut()
{
  font_glut_creator = new FontGlutCreator;
}

} // namespace infovis

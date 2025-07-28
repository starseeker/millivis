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
#ifndef INFOVIS_DRAWING_GL_SUPPORT_HPP
#define INFOVIS_DRAWING_GL_SUPPORT_HPP

#include <infovis/drawing/box.hpp>
#include <infovis/drawing/color.hpp>
#include <infovis/drawing/gl.hpp>

namespace infovis {

// guess this one
inline int type_of(char) { return GL_BYTE; }
inline int type_of(unsigned char) { return GL_UNSIGNED_BYTE; }
inline int type_of(short) { return GL_SHORT; }
inline int type_of(unsigned short) { return GL_UNSIGNED_SHORT; }
inline int type_of(int) { return GL_INT; }
inline int type_of(unsigned int) { return GL_UNSIGNED_INT; }
inline int type_of(float) { return GL_FLOAT; }
inline int type_of(double) { return GL_DOUBLE; }
  
template <class Box>
void draw_box(const Box& b)
{
  gl::rect(xmin(b), ymin(b), xmax(b), ymax(b));
}

template <class Box>
void stroke_box(const Box& b)
{
  gl::begin_line_loop l;
  gl::vertex(xmin(b), ymin(b));
  gl::vertex(xmax(b), ymin(b));
  gl::vertex(xmax(b), ymax(b));
  gl::vertex(xmin(b), ymax(b));
}

template <class Color>
void set_color(const Color& c, color_space_rgb)
{
  gl::color(c[0], c[1], c[2]);
}

template <class Color>
void set_color(const Color& c, color_space_rgba)
{
  gl::color(c[0], c[1], c[2], c[3]);
}

#if 0
template <class Color, class S>
void set_color(const Color& c, S)
{
  color_rgba rgba(c);
  color(rgba[0], rgba[1], rgba[2], rgba[3]);
}
#endif

template <class Color>
void set_color(const Color& c)
{
  set_color(c, color_space_traits<Color>::color_space());
}

template <class Coord>
void set_vertex(const point_<2, Coord>& p)
{
  gl::vertex(x(p), y(p));
}
} // namespace infovis

#endif // INFOVIS_DRAWING_GL_SUPPORT_HPP

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
#ifndef INFOVIS_DRAWING_COLOR_IO_HPP
#define INFOVIS_DRAWING_COLOR_IO_HPP

#include <infovis/drawing/color.hpp>
#include <string>
#include <iostream>

namespace infovis {

template <class T, class S>
std::ostream& operator << (std::ostream& out, const color_type<T,S>& color)
{
  typedef color_type<T,S> Color;
  out << Color::name() << ":";
  for (int i = 0; i < Color::last_channel; i++) {
    if (i != 0)
      out << "/";
    out << double(color[i]);
  }
  return out << " ";
}

template <class T, class S>
std::istream& operator >> (std::istream& in, color_type<T,S>& color)
{
  typedef color_type<T,S> Color;
  int i;
  const char * n = Color::name();

  for (i = 0; n[i] != 0; i++) {
    if (in.get() != n[i])
      return in;		// don't read
  }

  if (in.get() != ':')
    return in;
  for (i = 0; i < Color::last_channel; i++) {
    double c;
    if (i != 0)
      in.ignore(2, '/');
    in >> c;
    color[i] = c;
  }
  in.ignore(1, ' ');
  return in;
}

} // namespace infovis

#endif // INFOVIS_DRAWING_COLOR_IO_HPP

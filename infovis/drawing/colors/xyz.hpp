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
#ifndef INFOVIS_DRAWING_COLORS_XXY_HPP
#define INFOVIS_DRAWING_COLORS_XYZ_HPP

#include <infovis/drawing/color.hpp>

namespace infovis {

struct color_space_xyz {
  enum channel_name { x, y, z, last_channel };
  static const char * name() { return "xyz"; }
};

struct color_xyz : public color_type<float, color_space_xyz>
{
  typedef color_type<float, color_space_xyz> super;

  color_xyz();
  explicit color_xyz(float val) : super(val) {}
  color_xyz(const color_xyz& r) : super(r.begin(), r.end()) { }
  color_xyz(float X, float Y, float Z) {
    channel[x] = X;
    channel[y] = Y;
    channel[z] = Z;
  }
};

void convert(color_rgb<float>& rgb, const color_xyz& xyz);
void convert(color_xyz& xyz, const color_rgb<float>& rgb);


} // namespace infovis

#endif // INFOVIS_DRAWING_COLORS_XYZ_HPP

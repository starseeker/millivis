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
#include <infovis/drawing/colors/uvY.hpp>

namespace infovis {

color_uvY::color_uvY()
{
  color_xyz white;

  float div =
    white[color_xyz::x] +
    white[color_xyz::y] * 15.0f +
    white[color_xyz::z] * 3.0f;

  channel[u_prime] = 4.0f * white[color_xyz::x] / div;
  channel[v_prime] = 9.0f * white[color_xyz::y] / div;
  channel[Y] = white[color_xyz::y];
}

void convert(color_xyz& xyz, const color_uvY& uvY)
{
  typedef color_xyz To;
  typedef color_uvY From;

  double div, Y;

  Y = uvY[From::Y];
  div = 6.0 * uvY[From::u_prime] - 16.0 * uvY[From::v_prime] + 12.0;
  if (div == 0.0) {
    xyz = color_xyz(0);
  }
  else {
    xyz[To::x] = 9.0 * uvY[From::u_prime] / div;
    xyz[To::y] = 4.0 * uvY[From::v_prime] / div;
    xyz[To::z] = (-3.0 * uvY[From::u_prime] -
		  20.0 * uvY[From::v_prime] +
		  12.0) / div;
  }
}

void convert(color_uvY& uvY, const color_xyz& xyz)
{
  typedef color_xyz From;
  typedef color_uvY To;

  float div = xyz[From::x] + 15.0*xyz[From::y] + 3*xyz[From::z];

  if (div == 0) {
    uvY = color_uvY();
  } else {
    uvY[To::u_prime] = 4.0 * xyz[From::x] / div;
    uvY[To::v_prime] = 9.0 * xyz[From::x] / div;
    uvY[To::Y] = xyz[From::y];    
  }
}


} // namespace infovis

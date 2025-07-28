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
#include <infovis/drawing/colors/xyz.hpp>

namespace infovis {

color_xyz::color_xyz()
{
  channel[x] = float(0.38106149108714790+0.32025712365352110+0.24834578525933100);
  channel[y] = float(0.20729745115140850+0.68054638776373240+0.11215616108485920);
  channel[z] = float(0.02133944350088028+0.14297193020246480+1.24172892629665500);
}

void
convert(color_rgb<float>& rgb, const color_xyz& xyz)
{
  rgb[0] = (xyz[0] * 3.48340481253539000f +
	    xyz[1] * -1.52176374927285200f +
	    xyz[2] * -0.55923133354049780f);
  rgb[1] = (xyz[0] * -1.07152751306193600f +
	    xyz[1] * 1.96593795204372400f +
	    xyz[2] * 0.03673691339553462f);
  rgb[2] = (xyz[0] * 0.06351179790497788f +
	    xyz[1] * -0.20020501000496480f +
	    xyz[2] * 0.81070942031648220f);
}

void
convert(color_xyz& xyz, const color_rgb<float>& rgb)
{
  xyz[0] = (rgb[0] * 0.38106149108714790f +
	    rgb[1] * 0.32025712365352110f +
	    rgb[2] * 0.24834578525933100f);
  xyz[1] = (rgb[0] * 0.20729745115140850f +
	    rgb[1] * 0.68054638776373240f +
	    rgb[2] * 0.11215616108485920f);
  xyz[2] = (rgb[0] * 0.02133944350088028f +
	    rgb[1] * 0.14297193020246480f +
	    rgb[2] * 1.24172892629665500f);
}


} // namespace infovis

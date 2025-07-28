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
#include <treemap2/ColorRamp.hpp>
//#include <infovis/drawing/colors/hvc.hpp>
#define BREWER_COLORS

namespace infovis {


static const ColorRamp&
_ramp_categorical1()
{
  static ColorRamp ramp;
  if (ramp.empty()) {
    ramp.push_back(Color(228U,26U,28U));
    ramp.push_back(Color(55U,126U,184U));
    ramp.push_back(Color(77U,175U,74U));
    ramp.push_back(Color(152U,78U,163U));
    ramp.push_back(Color(255U,127U,0U));
    ramp.push_back(Color(255U,255U,51U));
    ramp.push_back(Color(166U,86U,40U));
    ramp.push_back(Color(247U,129U,191U));
  }
  return ramp;
}

static const ColorRamp&
_ramp_categorical2()
{
  static ColorRamp ramp;
  if (ramp.empty()) {
    ramp.push_back(color_half_grey);
    ramp.push_back(color_white);
    ramp.push_back(color_red);
    ramp.push_back(color_green);
    ramp.push_back(color_blue);
    ramp.push_back(color_cyan);
    ramp.push_back(color_magenta);
    ramp.push_back(color_yellow);
  }
  return ramp;
}


static const ColorRamp&
_ramp_sequential1()
{
  static ColorRamp ramp;
  if (ramp.empty()) {
#ifdef BREWER_COLORS
    ramp.push_back(Color(255U,247U,251U));
    ramp.push_back(Color(3U,78U,123U));
#else
    ramp.push_back(color_blue);
    ramp.push_back(color_red);
#endif
  }
  return ramp;
}

static const ColorRamp&
_ramp_sequential2()
{
  static ColorRamp ramp;
  if (ramp.empty()) {
#ifdef BREWER_COLORS
    ramp.push_back(Color(255U,255U,229U));
    ramp.push_back(Color(140U,051U,005U));
#else
    ramp.push_back(color_red);
    ramp.push_back(color_green);
#endif
  }
  return ramp;
}

const ColorRamp&
getRamp(Ramp r)
{
  switch(r) {
  case ramp_categorical1:
    return _ramp_categorical1();
  case ramp_categorical2:
    return _ramp_categorical2();
  case ramp_sequential1:
    return _ramp_sequential1();
  case ramp_sequential2:
    return _ramp_sequential2();
  }
  return _ramp_sequential1();
}

} // namespace infovis

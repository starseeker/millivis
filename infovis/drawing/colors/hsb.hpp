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
#ifndef INFOVIS_DRAWING_COLORS_HSB_HPP
#define INFOVIS_DRAWING_COLORS_HSB_HPP

#include <infovis/drawing/color.hpp>

namespace infovis {

struct color_space_hsb {
  enum channel_name { hue, saturation, brighness, last_channel };
  static const char * name() { return "hsb"; }
};

struct color_hsb : public color_type<float, color_space_hsb>
{
  typedef color_type<float, color_space_hsb> super;
  color_hsb() {
    channel[hue] = float(0);
    channel[saturation] = float(0);
    channel[brighness] = float(1);
  }
  color_hsb(float val) : super(val) {}
  color_hsb(const color_hsb& r) {
    for (int chan = 0; chan < last_channel; chan++) {
      channel[chan] = r.channel[chan];
    }
  }
  template <class ChannelIt>
  color_hsb(ChannelIt start, ChannelIt end) {
    int chan;
    for (chan = 0; chan < last_channel; chan++) {
      if (start != end)
	channel[chan] = *start++;
    }
    while(chan < last_channel)
      channel[chan++] = float(0);
  }
  color_hsb(float h, float s, float b) {
    channel[hue] = h;
    channel[saturation] = s;
    channel[brighness] = b;
  }
  color_hsb(double h, double s, double b) {
    detail::round(channel[hue], h * super::one());
    detail::round(channel[saturation], s * super::one());
    detail::round(channel[brighness], b * super::one());
  }
};

void convert(color_rgb<float>& rgb, const color_hsb& c);
void convert(color_hsb& hsb, const color_rgb<float>& c);

} // namespace infovis

#endif // INFOVIS_DRAWING_COLORS_HSB_HPP

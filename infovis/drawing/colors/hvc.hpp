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
#ifndef INFOVIS_DRAWING_COLORS_HVC_HPP
#define INFOVIS_DRAWING_COLORS_HVC_HPP

#include <infovis/drawing/color.hpp>
#include <infovis/drawing/colors/uvY.hpp>

namespace infovis {

struct color_space_hvc {
  enum channel_name { hue, value, chroma, last_channel };
};

struct color_hvc : public color_type<float, color_space_hvc>
{
  typedef color_type<float, color_space_hvc> super;

  color_hvc();
  color_hvc(float val) : super(val) {}
  color_hvc(const color_hvc& r) : super(r.begin(), r.end()) { }
  template <class ChannelIt>
  color_hvc(ChannelIt start, ChannelIt end) {
    int chan;
    for (chan = 0; chan < color_space_hvc::last_channel; chan++) {
      if (start != end)
	this->channel[chan] = *start++;
    }
    while(chan < color_space_hvc::last_channel)
      this->channel[chan++] = float(0);
  }
  color_hvc(float h, float v, float c) {
    this->channel[color_space_hvc::hue] = h;
    this->channel[color_space_hvc::value] = v;
    this->channel[color_space_hvc::chroma] = c;
  }
  color_hvc(double h, double v, double c) {
    this->channel[color_space_hvc::hue] = h;
    this->channel[color_space_hvc::value] = v;
    this->channel[color_space_hvc::chroma] = c;
  }

  bool is_valid() const;
  bool check_modify();
  bool clip_chroma();
  bool clip_value();
  bool clip_value_chroma();
  float max_chroma();		// given a hue/value
  float max_value();		// given a hue/chroma
  float min_value();		// given a hue/chroma
  float max_value_chroma();	// given a hue
};

void convert(color_uvY& uvY, const color_hvc& hvc);
void convert(color_hvc& hvc, const color_uvY& uvY);
void convert(color_xyz& xyz, const color_hvc& hvc);
void convert(color_hvc& hvc, const color_xyz& xyz);
void convert(color_rgb<float>& rgb, const color_hvc& hvc);
void convert(color_hvc& hvc, const color_rgb<float>& rgb);

} // namespace infovis

#endif // INFOVIS_DRAWING_COLORS_HVC_HPP

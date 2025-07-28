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
#ifndef INFOVIS_DRAWING_LITE_RANGESLIDER_HPP
#define INFOVIS_DRAWING_LITE_RANGESLIDER_HPP

#include <infovis/drawing/lite/LiteSlider.hpp>

namespace infovis {
class Font;

/**
 * Lite component implementing a range slider.
 */
class LiteRangeSlider : public LiteSlider
{
public:
  LiteRangeSlider(BoundedRangeObservable * observable,
		  const Box& bounds,
		  direction dir = left_to_right,
		  const Color& fg = color_black,
		  const Color& bg = color_white,
		  Font * font = 0,
		  float minmax_size = 10
		  );
  LiteRangeSlider(const Box& bounds,
		  direction dir = left_to_right,
		  const Color& fg = color_black,
		  const Color& bg = color_white,
		  Font * font = 0,
		  float minmax_size = 10
		  );


  virtual Lite * clone() const;

  virtual float getMinSize() const;
  virtual void setMinSize(float v);

  virtual float getMaxSize() const;
  virtual void setMaxSize(float v);

  virtual string maxLabel();

protected:
  float min_size_;
  float max_size_;
  float saved_range_;
  Zone zone_dragged_;
};


} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_RANGESLIDER_HPP

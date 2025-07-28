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
#include <infovis/drawing/lite/LiteSliderExt.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LiteSliderExt::LiteSliderExt(BoundedRangeObservable * observable,
			     const Box& bounds,
			     const Box& max_bounds,
			     direction dir,
			     const Color& fg,
			     const Color& bg,
			     Font * font)
  : LiteSlider(observable, bounds, dir, fg, bg, font),
    max_bounds_(max_bounds)
{ }

LiteSliderExt::LiteSliderExt(const Box& bounds,
			     const Box& max_bounds,
			     direction dir,
			     const Color& fg,
			     const Color& bg,
			     Font * font)
  : LiteSlider(bounds, dir, fg, bg, font),
    max_bounds_(max_bounds)
{ }

const Box&
LiteSliderExt::getMaxBounds() const { return max_bounds_; }

void
LiteSliderExt::setMaxBounds(const Box& v) {max_bounds_ = v; }

static float
inter(float min, float max, float t)
{
  return min * (1 - t) + max * t;
}

static float
param(float min, float max, float val)
{
  return (val - min) / (max - min);
}

Lite *
LiteSliderExt:: clone() const
{
  return new LiteSliderExt(getObservable(),
			   getBounds(),
			   getMaxBounds(),
			   getDirection(),
			   getForegroundColor(),
			   getBackgroundColor());
}

float
LiteSliderExt::getValue(const Point& pos, Zone) const
{
  float v = LiteSlider::getValue(pos);
  if (infovis::inside(bounds, x(pos), y(pos))) {
    return v;
  }
  float p;
  float min, max, val;

  if (direction_ == left_to_right || direction_ == right_to_left) {
    if (y(pos) < ymin(bounds)) {
      p = param(ymin(bounds), ymin(max_bounds_), y(pos));
    }
    else {
      p = param(ymax(bounds), ymax(max_bounds_), y(pos));
    }
    min = inter(xmin(bounds), xmin(max_bounds_), p);
    max = inter(xmax(bounds), xmax(max_bounds_), p);
    p = param(min, max, x(pos));
  }
  else {
    if (x(pos) < xmin(bounds)) {
      p = param(xmin(bounds), xmin(max_bounds_), x(pos));
    }
    else {
      p = param(xmax(bounds), xmax(max_bounds_), x(pos));
    }
    min = inter(ymin(bounds), ymin(max_bounds_), p);
    max = inter(ymax(bounds), ymax(max_bounds_), p);
    p = param(min, max, y(pos));
  }
  const BoundedRange * range = getObservable()->getBoundedRange();
  val = inter(range->min(), range->max(), p);
  if (val < range->min())
    val = range->min();
  else if (val > range->max())
    val = range->max();
  return val;
}
  

} // namespace infovis

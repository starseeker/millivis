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
#ifndef TREEMAP_LITERANGESLIDERGRAPH_HPP
#define TREEMAP_LITERANGESLIDERGRAPH_HPP

#include <infovis/drawing/lite/LiteRangeSlider.hpp>
#include <BarGraph.hpp>

namespace infovis {

class LiteRangeSliderGraph : public LiteRangeSlider
{
public:
  LiteRangeSliderGraph(BoundedRangeObservable * observable,
		       const Box& bounds,
		       const BarGraph * bar,
		       direction dir = left_to_right,
		       const Color& fg = color_black,
		       const Color& bg = color_white,
		       Font * font = 0,
		       float minmax_size = 10
		       );
  LiteRangeSliderGraph(const Box& bounds,
		       const BarGraph * bar,
		       direction dir = left_to_right,
		       const Color& fg = color_black,
		       const Color& bg = color_white,
		       Font * font = 0,
		       float minmax_size = 10
		       );
  
  virtual Lite * clone() const;

  virtual void renderOverlay(const Box& b);
protected:
  const BarGraph * bar_;
};

} // namespace infovis

#endif // TREEMAP_LITERANGESLIDERGRAPH_HPP

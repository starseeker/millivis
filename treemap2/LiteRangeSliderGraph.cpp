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
#include <LiteRangeSliderGraph.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LiteRangeSliderGraph::LiteRangeSliderGraph(BoundedRangeObservable * observable,
					   const Box& bounds,
					   const BarGraph * bar,
					   direction dir,
					   const Color& fg,
					   const Color& bg,
					   Font * font,
					   float minmax_size
					   )
  : LiteRangeSlider(observable, bounds, dir, fg, bg, font, minmax_size),
    bar_(bar)
{ }

LiteRangeSliderGraph::LiteRangeSliderGraph(const Box& bounds,
					   const BarGraph * bar,
					   direction dir,
					   const Color& fg,
					   const Color& bg,
					   Font * font,
					   float minmax_size
					   )
  : LiteRangeSlider(bounds, dir, fg, bg, font, minmax_size),
    bar_(bar)
{ }
  
Lite *
LiteRangeSliderGraph::clone() const
{
  return new LiteRangeSliderGraph(getObservable(),
				  getBounds(),
				  bar_,
				  getDirection(),
				  getForegroundColor(),
				  getBackgroundColor(),
				  getFont(),
				  min_size_);
}

void
LiteRangeSliderGraph::renderOverlay(const Box& b)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  LiteRangeSlider::renderOverlay(b);
  Box inner(xmin(b)+getMinSize(), ymin(b),
	    xmax(b)-getMaxSize(), ymax(b));
  set_color(Color(0,1.0,0, 0.7));
  bar_->render(inner, direction_);
  glPopAttrib();
}


} // namespace infovis

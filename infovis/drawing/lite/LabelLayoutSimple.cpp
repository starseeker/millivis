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
#include <infovis/drawing/lite/LabelLayoutSimple.hpp>

namespace infovis {

LabelLayoutSimple::LabelLayoutSimple(const Box& box)
  : LabelLayout(box) 
{ }

LabelLayout::LayoutResult
LabelLayoutSimple::doLayout()
{
  for (List::const_iterator i = list_.begin(); i != list_.end(); i++) {
    LiteDisplacedLabel * lb = *i;
    Box b(lb->getBounds());

    if (! contains(bounds_, b)) {
      float dx = 0;
      float dy = 0;
      if (xmin(b) < xmin(bounds_)) {
	dx = xmin(bounds_) - xmin(b);
      }
      else if (xmax(b) > xmax(bounds_)) {
	dx = xmax(bounds_) - xmax(b);
      }
      if (ymin(b) < ymin(bounds_)) {
	dy = ymin(bounds_) - ymin(b);
      }
      else if (ymax(b) > ymax(bounds_)) {
	dy = ymax(bounds_) - ymax(b);
      }
      lb->movePosition(Vector(dx, dy));
    }
  }
  return layout_dont_overlap;
}

} // namespace infovis

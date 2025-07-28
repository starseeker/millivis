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
#include <infovis/drawing/lite/LayoutTable.hpp>
#include <algorithm>

namespace infovis {

LayoutTable::LayoutTable(int columns, float hgap, float vgap)
  : columns_(columns), hgap_(hgap), vgap_(vgap)
{ }

LayoutTable::~LayoutTable()
{ }

Box
LayoutTable::doLayoutIn(Lite * container)
{
  int i;
  int rows_ = (lite_.size() + columns_-1) / columns_;

  std::vector<float> columns(columns_, 0.0f);
  std::vector<float> rows(rows_, 0.0f);
  for (i = 0; i < lite_.size(); i++) {
    Vector v(extent(lite_[i]->getMinBounds()));

    if (dx(v) > columns[i % columns_])
      columns[i % columns_] = dx(v);
    if (dy(v) > rows[i / columns_])
      rows[i / columns_] = dy(v);
  }

  Box bounds(container->getBounds());
  std::vector<float> c_start(columns_+1);
  c_start[0] = xmin(bounds);
  for (i = 0; i < columns_; i++) {
    c_start[i+1] = c_start[i] + columns[i] + hgap_;
  }

  std::vector<float> r_start(rows_+1);
  r_start[0] = ymin(bounds);
  for (i = 0; i < rows_; i++) {
    r_start[i+1] = r_start[i] + rows[i] + vgap_;
  }

  for (i = 0; i < lite_.size(); i++) {
    Lite * l = lite_[i];
    int c = i % columns_;
    int r = i / columns_;
    l->setPosition(Point(c_start[c], r_start[r])); // move everything
    l->setBounds(Box(c_start[c], // give more room
		     r_start[r],
		     c_start[c] + columns[c],
		     r_start[r] + rows[r]));
  }
  return Box(min(bounds), Point(c_start[columns_]-hgap_,
				r_start[rows_]-vgap_));
}

} // namespace infovis

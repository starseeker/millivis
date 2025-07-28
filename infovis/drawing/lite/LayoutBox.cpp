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
#include <infovis/drawing/lite/LayoutBox.hpp>
#include <algorithm>

namespace infovis {

LayoutBox::LayoutBox(Direction dir, float gap)
  : dir_(dir), gap_(gap) { }
LayoutBox::~LayoutBox() { }

Layout *
LayoutBox::clone() const
{
  return new LayoutBox(dir_);	// don't copy contents
}

Box
LayoutBox::doLayoutIn(Lite * container)
{
  Vector size(0, 0);

  Box b(container->getBounds());
  Point p(min(b));
  for (Lite::List::const_iterator i = lite_.begin();
       i != lite_.end(); i++) {
    Lite * l = *i;
    Box b(l->getBounds());
    Vector v(extent(b));

    l->movePosition(p-min(b));
    if (dir_ == horizontal) {
      set_dx(size, dx(size)+gap_+dx(v));
      set_dy(size, std::max(dy(size),dy(v)));
      set_x(p, x(p)+gap_+dx(v));
    }
    else {
      set_dx(size, std::max(dx(size),dx(v)));
      set_dy(size, dy(size)+gap_+dy(v));
      set_y(p, y(p)+gap_+dy(v));
    }
  }
  container->setSize(size);
  return Box(min(b),size);
}

} // namespace infovis

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
#ifndef TREEMAP2_BORDERDRAWER_HPP
#define TREEMAP2_BORDERDRAWER_HPP

#include <infovis/drawing/notifiers/BoundedRange.hpp>
#include <infovis/tree/treemap/drawing/border_drawer.hpp>
#include <types.hpp>

namespace infovis {

struct BorderDrawer : public DefaultBoundedRangeObservable
{
  BorderDrawer(const Tree& t)
    : DefaultBoundedRangeObservable(1, 10, 1), tree_(t) {  }

  bool begin_border(const Box& b, 
		    node_descriptor n,
		    unsigned depth) const {
    if (is_leaf(n, tree_))
      return false;
    else
      return width(b) > (2+value_) && height(b) > (2+value_);
  }
  bool left_border(Box& b, 
		   node_descriptor n,
		   unsigned depth) const {
    if (! is_leaf(n, tree_))
      b = Box(xmin(b), ymax(b)-value_, xmax(b), ymax(b));
    return true;
  }
  bool right_border(Box& b, 
		   node_descriptor n,
		    unsigned depth) const {
    return false;
  }
  bool top_border(Box& b, 
		  node_descriptor n,
		  unsigned depth) const {
    if (! is_leaf(n, tree_))
      b = Box(xmin(b), ymin(b), xmin(b)+value_, ymax(b)-value_);
    return true;
  }
  bool bottom_border(Box& b, 
		     node_descriptor n,
		     unsigned depth) const {
    return false;
  }
  void remaining_box(Box& b,
		     node_descriptor n,
		     unsigned depth) const {
    if (! is_leaf(n, tree_)) {
      set_xmin(b,xmin(b)+value_);
      set_ymax(b,ymax(b)-value_);
    }
  }  

  const Tree& tree_;
};

} // namespace infovis

#endif // TREEMAP2_BORDERDRAWER_HPP

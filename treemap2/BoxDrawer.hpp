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
#ifndef TREEMAP2_BOXDRAWER_HPP
#define TREEMAP2_BOXDRAWER_HPP

#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/treemap/drawing/border_drawer.hpp>
#include <BorderDrawer.hpp>
#include <vector>

namespace infovis {

struct BoxDrawer : public null_drawer<Tree,Box>
{
  typedef std::vector<Box, gc_alloc<Box,true> > BoxList;

  BoxDrawer(const BorderDrawer& b, BoxList& bm)
    : border_(b), boxes_(bm), depth_(0)
  { }

  void set_depth(unsigned d) { depth_ = d; }
  unsigned get_depth() const { return depth_; }
  void start() { }
  void push(const Box& b, node_descriptor n) {
    boxes_[n] = b;
  }
  bool begin_box(const Box& b,
		 node_descriptor n,
		 unsigned depth) {
    return
      depth <= depth_;
  }
  void draw_box(const Box& b,
		node_descriptor n,
		unsigned depth) {
    if (depth < depth_)
      push(b, n);
  }
  void draw_border(Box& b, 
		   node_descriptor n,
		   unsigned depth) {
    if (border_.begin_border(b, n, depth)) {
      border_.remaining_box(b, n, depth);
    }
    if (depth == depth_)
      push(b, n);
  }
  void remove_border(Box& b, 
		   node_descriptor n,
		   unsigned depth) {
    if (border_.begin_border(b, n, depth)) {
      border_.remaining_box(b, n, depth);
    }
  }
  void end_box(const Box& b, 
	       node_descriptor n,
	       unsigned depth) { }

  void finish() { }
  const BoxList& getBoxes() const { return boxes_; }

protected:
  const BorderDrawer& border_;
  BoxList& boxes_;
  unsigned depth_;
};

} // namespace infovis

#endif // TREEMAP2_BOXDRAWER_HPP

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
#ifndef INFOVIS_TREE_TREEMAP_SLICE_AND_DICE_HPP
#define INFOVIS_TREE_TREEMAP_SLICE_AND_DICE_HPP

#include <infovis/alloc.hpp>
// TODO: Removed boost/config.hpp - C++17 modernization
#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/tree_concepts.hpp>
#include <infovis/tree/sum_weight_visitor.hpp>
#include <infovis/drawing/direction.hpp>
#include <infovis/tree/treemap/treemap.hpp>
#include <infovis/tree/treemap/drawing/drawer.hpp>

namespace infovis {

/**
 * Implementation of the slice and dice treemap algorithm
 */
template <class Tree, 
	  class Box,
	  class WeightMap,
	  class Drawer = null_drawer<Tree,Box>,
          class Filter = filter_none>
struct treemap_slice_and_dice :
    public treemap<Tree,Box,WeightMap,Drawer,Filter>
{
  typedef treemap<Tree,Box,WeightMap,Drawer,Filter> super;
  typedef typename super::coord_type coord_type;
  typedef typename super::dist_type dist_type;
  typedef typename super::children_iterator children_iterator;
  typedef typename super::node_descriptor node_descriptor;

  treemap_slice_and_dice(const Tree& tree,
			 WeightMap wm,
			 Drawer drawer,
			 Filter filter)
    : super(tree, wm, drawer,filter)
  { }

  treemap_slice_and_dice(const Tree& tree,
			 WeightMap wm,
			 Drawer drawer = Drawer())
    : super(tree, wm, drawer)
  { }

  unsigned visit(direction dir, const Box& box,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth = 0)
  {
    if (! this->drawer_.begin_box(box,n,depth))
      return 0;
    Box b(box);
    unsigned ret = 1;
    this->drawer_.draw_border(b, n, depth);
    if (is_leaf(n,this->tree_)) {
      this->drawer_.draw_box(b, n, depth);
    }
    else {
      const float tw = infovis::get(this->weight_, n);

      this->drawer_.begin_strip(box, n, depth, dir);
      if (dir == left_to_right) {
	dist_type w = width(b);
	float x = xmin(b); // + w * weight_[n] / (2*tw);

#if 0
	if (x > xmin(b)) {
	  this->drawer_.draw_box(Box(xmin(b),ymin(b),x,ymax(b)), n, depth);
	}
#endif
	children_iterator i,end;
	for (std::tie(i,end) = children(n,this->tree_); i != end; i++) {
	  node_descriptor child = *i;
	  if (this->filter_(child))
	    continue;
	  float nw = w * infovis::get(this->weight_, child) / tw;
	  float e = /*(i == (end-1)) ? xmax(b) : */coord_type(x+nw);
	  ret += visit(flip(dir),
		       Box(x,ymin(b),e,ymax(b)),
		       child, depth+1);
	  x += nw;
	}
#if 0
	if (x < xmax(b)) {
	  this->drawer_.draw_box(Box(x,ymin(b),xmax(b),ymax(b)), n, depth);
	}
#endif
      }
      else {
	dist_type h = height(b);
	float y = ymin(b); // + h * weight_[n] / (2*tw);

#if 0
	if (y > ymin(b)) {
	  this->drawer_.draw_box(Box(xmin(b),ymin(b),xmax(b),y), n, depth);
	}
#endif
	children_iterator i,end;
	for (std::tie(i,end) = children(n,this->tree_); i != end; i++) {
	  node_descriptor child = *i;
	  if (this->filter_(child))
	    continue;
	  float nh = h * infovis::get(this->weight_, child) / tw;
	  float e = /* (i == (end-1)) ? ymax(b) : */coord_type(y+nh);
	  ret += visit(flip(dir),
		       Box(xmin(b),y,xmax(b),e), child, depth+1);
	  y += nh;
	}
#if 0
	if (y < ymax(b)) {
	  this->drawer_.draw_box(Box(xmin(b),y,xmax(b),ymax(b)), n, depth);
	}
#endif
      }
      this->drawer_.end_strip(box, n, depth, dir);
    }
    this->drawer_.end_box(box,n,depth);
    return ret;
  }
};
} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_SLICE_AND_DICE_HPP

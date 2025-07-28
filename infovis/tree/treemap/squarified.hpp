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
#ifndef INFOVIS_TREE_TREEMAP_SQUARIFIED_HPP
#define INFOVIS_TREE_TREEMAP_SQUARIFIED_HPP

#include <infovis/alloc.hpp>
// TODO: Removed boost/config.hpp - C++17 modernization
#include <infovis/tree/tree.hpp>
#include <infovis/tree/tree_concepts.hpp>
#include <infovis/tree/sum_weight_visitor.hpp>
#include <infovis/tree/treemap/treemap.hpp>
#include <infovis/tree/treemap/drawing/drawer.hpp>
#include <tuple> // TODO: Added for std::tie - C++17 modernization

#include <cassert>

namespace infovis {

/**
 * Parametric class for changing the orientation of a squarified treemap
 */
template <class Tree, class Box>
struct treemap_chose_orient {
  bool operator()(const Box& b,
		  typename tree_traits<Tree>::node_descriptor n,
		  unsigned depth) const {
    return width(b) > height(b);
  }
};

/**
 * Squarified treemap algorithm implementation
 * 
 * This class implements the squarified treemap algorithm by Bruls, Huizing, and van Wijk.
 * The algorithm aims to create rectangular subdivisions where the aspect ratios are as 
 * close to 1:1 (square) as possible, making the visualization easier to read.
 * 
 * The squarified algorithm works by:
 * 1. Sorting children by weight (descending)
 * 2. Building rows/columns of rectangles that maintain good aspect ratios
 * 3. Alternating between horizontal and vertical subdivision based on available space
 * 
 * Template parameters:
 * - Tree: Tree data structure type
 * - Box: Bounding box type for layout
 * - WeightMap: Property map providing node weights
 * - Drawer: Drawing/rendering interface (default: null_drawer)
 * - Orient: Orientation policy (default: treemap_chose_orient)
 * - Filter: Node filtering policy (default: filter_none)
 */
template <class Tree,
	  class Box,
	  class WeightMap,
	  class Drawer = null_drawer<Tree,Box>,
	  class Orient = treemap_chose_orient<Tree,Box>,
	  class Filter = filter_none
>
struct treemap_squarified
  : public treemap<Tree,Box,WeightMap,Drawer,Filter>
{
  typedef treemap<Tree,Box,WeightMap,Drawer,Filter> super;
  typedef typename super::box_type box_type;
  typedef typename super::dist_type dist_type;
  typedef typename super::children_iterator children_iterator;
  typedef typename super::node_descriptor node_descriptor;

  treemap_squarified(const Tree& tree,
		     WeightMap wm,
		     Drawer drawer = Drawer(),
		     Orient orient = Orient(),
		     Filter filter = Filter())
    : super(tree, wm, drawer,filter),
      orient_(orient)
  { }

  unsigned visit(const box_type& box,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth = 0)
  {
    if (! this->drawer_.begin_box(box,n,depth)) return 0;
    box_type b(box);
    unsigned ret = 1;
    this->drawer_.draw_border(b, n, depth);
    if (is_leaf(n,this->tree_)) {
      this->drawer_.draw_box(b, n, depth);
    }
    else {
      ret = visit_strips(b, n, depth);
    }
    this->drawer_.end_box(box,n, depth);
    return ret;
  }    
  
  unsigned visit_strips(box_type& b,
			typename tree_traits<Tree>::node_descriptor n,
			unsigned depth)
  {
    const float tw = infovis::get(this->weight_,n);
    unsigned ret = 1;

    // C++17 structured bindings instead of std::tie
    auto [i, end] = children(n, this->tree_);
    const float scale = width(b) * height(b) / tw;
    if (scale == 0) {
#if 0
      while (i != end) {
	node_descriptor child = *i;
	ret += visit(b, child, depth+1);
	i++;
      }
#else
      // nothing
#endif
    }
    else while (i != end) {
      if (this->filter_(*i)) {
	continue;
      }
      if (orient_(b, n, depth)) {
	dist_type w = height(b);
	float y = ymin(b);
	float width;

	this->drawer_.begin_strip(b, n, depth, bottom_to_top);
	auto e = squarify(i, end, w, scale, width);
	if (width == 0) {
	  // can happen if all the remaining children have a weight==0
	  i = e;
	}
	for (; i != e; i++) {
	  const auto child = *i;
	  const float nw = infovis::get(this->weight_,child) * scale / width;
	  ret += visit(box_type(xmin(b),y,xmin(b)+width,y+nw),
		       child, depth+1);
	  y += nw;
	}
	set_xmin(b, width + xmin(b));
	this->drawer_.end_strip(b, n, depth, bottom_to_top);
      }
      else {
	dist_type w = width(b);
	float x = xmin(b);
	float width;
	this->drawer_.begin_strip(b, n, depth, left_to_right);
	auto e = squarify(i, end, w, scale, width);
	if (width == 0) {
	  // can happen if all the remaining children have a weight==0
	  i = e;
	}
	for (; i != e; i++) {
	  const auto child = *i;
	  const float nw = infovis::get(this->weight_,child) * scale / width;
	  ret += visit(box_type(x, ymin(b), x+nw, ymin(b)+width),
		       child, depth+1);
	  x += nw;
	}
	set_ymin(b, width + ymin(b));
	this->drawer_.end_strip(b, n, depth, left_to_right);
      }
    }
    return ret;
  }

  children_iterator squarify(children_iterator beg,
			     children_iterator end,
			     float length,
			     float scale,
			     float &width) const
  {
    assert(length != 0);
    float s = 0;
    while (beg != end && s == 0) {
      if (! this->filter_(*beg))
	s = infovis::get(this->weight_,*beg++) * scale;
    }
    width = s / length;
    if (beg == end) {
      return beg;
    }
    float s2 = s*s;
    float min_area = s;
    float max_area = s;
    float worst = std::max(length / width, width / length);
    float w2 = length * length;
    while (beg != end) {
      if (this->filter_(*beg))
	continue;
      float area = infovis::get(this->weight_,*beg) * scale;
      if (area == 0) {
	beg++;
	continue;
      }
      s += area;
      s2 = s*s;
      if (area < min_area)
	min_area = area;
      else if (area > max_area)
	max_area = area;
      float cur_worst = std::max(w2*max_area/s2, s2/(w2*min_area));
      if (cur_worst > worst) {
	s -= area;
	break;
      }
      worst = cur_worst;
      beg++;
    }
    width = s / length;
    return beg;
  }
  Orient orient_;
};

} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_SQUARIFIED_HPP

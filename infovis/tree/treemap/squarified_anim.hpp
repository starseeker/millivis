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
#ifndef INFOVIS_TREE_TREEMAP_SQUARIFIED_ANIM_HPP
#define INFOVIS_TREE_TREEMAP_SQUARIFIED_ANIM_HPP

#include <infovis/tree/treemap/squarified.hpp>

namespace infovis {

/**
 * Animated squarifed treemap implementation
 */
template <class Tree, 
	  class Box,
	  class Drawer,
	  class WeightMap,
	  class WeightMap2 = WeightMap,
	  class Orient = treemap_chose_orient<Tree,Box>,
	  class Filter = filter_none
>
struct treemap_squarified_anim :
    public treemap_squarified<Tree,Box,WeightMap,Drawer,Orient,Filter>
{
  using super = treemap_squarified<Tree,Box,WeightMap,Drawer,Orient,Filter>;
  using coord_type = typename box_traits<Box>::coord_type;
  using dist_type = typename box_traits<Box>::dist_type;
  using children_iterator = typename tree_traits<Tree>::children_iterator;
  using node_descriptor = typename tree_traits<Tree>::node_descriptor;

  treemap_squarified_anim(const Tree& tree,
			  WeightMap wm,
			  WeightMap2 wm2,
			  Drawer drawer = Drawer(),
			  Orient orient = Orient(),
			  Filter filter = Filter())
    : super(tree,wm,drawer, orient, filter),
      weight2_(wm2)
  { }

  unsigned visit_anim(const Box& box, const Box& box2,
		      typename tree_traits<Tree>::node_descriptor n,
		      unsigned depth = 0) {
    if (! this->drawer_.begin_box(box2,n, depth))
      return 0;
    Box b(box);
    Box b2(box2);
    this->drawer_.draw_border(b2, n,  depth);
    unsigned ret = 1;
    if (degree(n,this->tree_) == 0) {
      this->drawer_.draw_box(b2, n,  depth);
    }
    else {
      this->drawer_.remove_border(b, n,  depth);
      const float tw = infovis::get(this->weight_,n);
      const float tw2 = infovis::get(weight2_,n);

      // C++17 structured bindings instead of boost::tie
      auto [i, end] = children(n, this->tree_);
      float scale = width(b) * height(b) / tw;
      float scale2 = width(b2) * height(b2) / tw2;
      if (scale == 0) {
	// nothing
      }
      else while (i != end) {
	if (this->filter_(*i)) {
	  continue;
	}
	if (this->orient_(b, n, depth)) {
	  dist_type w = height(b);
	  dist_type w2 = height(b2);
	  float y = ymin(b);
	  float y2 = ymin(b2);
	  float width;

	  auto e = this->squarify(i, end, w, scale, width);
	  float width2 = recompute_width(i, e, w2, scale2);
	  for (; i != e; i++) {
	    const auto child = *i;
	    const float nw = infovis::get(this->weight_,child) * scale / width;
	    const float nw2 = infovis::get(weight2_,child) * scale2 / width2;
	    ret += visit_anim(Box(xmin(b),y,xmin(b)+width,y+nw),
			      Box(xmin(b2),y2,xmin(b2)+width2,y2+nw2),
			      child, depth+1);
	    y += nw;
	    y2 += nw2;
	  }
	  set_xmin(b, width + xmin(b));
	  set_xmin(b2, width2 + xmin(b2));
	}
	else {
	  dist_type w = width(b);
	  dist_type w2 = width(b2);
	  float x = xmin(b);
	  float x2 = xmin(b2);
	  float width;
	  auto e = this->squarify(i, end, w, scale, width);
	  float width2 = recompute_width(i, e, w2, scale2);
	  for (; i != e; i++) {
	    const auto child = *i;
	    const float nw = infovis::get(this->weight_,child) * scale / width;
	    const float nw2 = infovis::get(weight2_,child) * scale2 / width2;
	    ret += visit_anim(Box(x, ymin(b), x+nw, ymin(b)+width),
			      Box(x2, ymin(b2), x2+nw2, ymin(b2)+width2),
			      child, depth+1);
	    x += nw;
	    x2 += nw2;
	  }
	  set_ymin(b, width + ymin(b));
	  set_ymin(b2, width2 + ymin(b2));
	}
      }
    }
    this->drawer_.end_box(box2,n,depth);
    return ret;
  }
  float recompute_width(typename tree_traits<Tree>::children_iterator beg,
			typename tree_traits<Tree>::children_iterator end,
			float length,
			float scale) const
  {
    float s = 0;
    while (beg != end) {
      if (! this->filter_(*beg))
	s += infovis::get(weight2_,*beg++) * scale;
    }
    return s / length;
  };
  WeightMap2 weight2_;
};


} // namespace infovis 

#endif // INFOVIS_TREE_TREEMAP_SQUARIFIED_ANIM_HPP

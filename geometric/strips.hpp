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
#ifndef INFOVIS_TREE_TREEMAP_STRIPS_HPP
#define INFOVIS_TREE_TREEMAP_STRIPS_HPP

#include <infovis/alloc.hpp>
#include <infovis/config.hpp>
#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/tree_concepts.hpp>
#include <infovis/tree/treemap/drawing/drawer.hpp>
#include <infovis/drawing/position.hpp>
#include <infovis/drawing/direction.hpp>

#include <algorithm>
#include <cassert>
#include <vector>
#include <stack>
#include <map>

namespace infovis {

/**
 * Record the layout of a treemap.
 *
 * A treemap is organized in recursive strip lists.
 * Each node is associated with a strip list.
 */
template <class Tree>
struct strips
{
  typedef typename tree_traits<Tree>::node_descriptor node_descriptor;

  struct strip : public std::vector<node_descriptor> {
    direction direction_;
    position position_;

    strip() { }
    strip(direction dir, position pos)
      : direction_(dir),
	position_(pos)
    { }
    strip(direction dir)
      : direction_(dir),
	position_(default_position(dir))
    { }
  };

  typedef std::vector<strip> striplist;
  typedef std::map<node_descriptor,striplist> Map;
  typedef Map::iterator iterator;
  typedef Map::const_iterator const_iterator;

  Map node_striplist_;

  bool has_striplist(node_descriptor n) const {
    return (node_striplist.find(n) != node_striplist.end());
  }
  striplist& node_striplist(node_descriptor n) {
    return node_striplist_[n];
  }
  const striplist& node_striplist(node_descriptor n) const {
    Map::const_iterator i = node_striplist_.find(n);
    if (i == node_striplist_.end())
      throw std::runtime_error("no striplist for node");
    return i->second;
  }
  int size() const { return node_striplist_.size(); }

  iterator begin() { return node_striplist_.begin(); }
  iterator end() { return node_striplist_.end(); }
  const_iterator begin() const { return node_striplist_.begin(); }
  const_iterator end() const { return node_striplist_.end(); }
};

template <class Tree, class Box>
struct strips_drawer :
    public null_drawer<Tree,Box>,
    public strips<Tree>
{
  typedef typename tree_traits<Tree> traits;
  node_descriptor current_node_;
  strip * current_strip_;
  const Tree& tree_;
  std::stack<node_descriptor,std::vector<node_descriptor> > node_stack_;

  strips_drawer(const Tree& tree)
    : tree_(tree),
      current_node_(traits::nil()),
      current_strip_(0)
  { }

  void add_node(node_descriptor n) {
    if (current_strip_ != 0)
      current_strip_->push_back(n);
  }

  void begin_strip(const Box& b,
		   node_descriptor n,
		   unsigned depth,
		   direction dir) {
    node_stack_.push(current_node_);
    current_node_ = n;
    striplist& slist = node_striplist(n);
    slist.push_back(strip(dir));
    current_strip_ = &slist.back();
  }
  bool begin_box(const Box& b,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
    add_node(n);
    return true;
  }

  void end_strip(const Box& b,
		 node_descriptor n,
		 unsigned depth,
		 direction dir) {
    current_node_ = node_stack_.top();
    node_stack_.pop();
    if (node_stack_.size() != 0)
      current_strip_ = &node_striplist(current_node_).back();
  }
};

template <class Tree,
	  class Box,
	  class WeightMap,
	  class TotalWeightMap = WeightMap,
	  class Drawer = null_drawer<Tree,Box>
>
struct treemap_strips
  : public treemap<Tree,Box,WeightMap,TotalWeightMap,Drawer>
{
  typedef treemap<Tree,Box,WeightMap,TotalWeightMap,Drawer> super;
  typedef typename strips<Tree>& Strips;

  treemap_strips(const Tree& tree,
		 const typename strips<Tree>& s,
		 WeightMap wm,
		 TotalWeightMap twm,
		 Drawer drawer = Drawer())
    : super(tree, wm, twm, drawer),
      strips_(s)
  { }

  float compute_width(typename strips<Tree>::strip::const_iterator beg,
		      const typename strips<Tree>::strip::const_iterator end,
		      float length,
		      float scale) const
  {
    float s = 0;
    while (beg != end) {
      s += boost::get(total_weight_,*beg++) * scale;
      //s += total_weight_[*beg++] * scale;
    }
    return s / length;
  };

  unsigned visit_strips(box_type& b,
			typename tree_traits<Tree>::node_descriptor n,
			unsigned depth)
  {
    const float tw = boost::get(total_weight_,n) - boost::get(weight_,n);
    unsigned ret = 1;

    const strips<Tree>::striplist& striplist = strips_.node_striplist(n);
    children_iterator i, end;
    tie(i,end) = children(n, tree_);
    float scale = width(b) * height(b) / tw;
    for (strips<Tree>::striplist::const_iterator j = striplist.begin();
	 j != striplist.end(); j++) {
      const strips<Tree>::strip& s = *j;
      if (s.direction_ == bottom_to_top) {
	dist_type w = height(b);
	float y = ymin(b);
	float width = compute_width(s.begin(), s.end(), w, scale);

	drawer_.begin_strip(b, n, depth, bottom_to_top);

	for (strips<Tree>::strip::const_iterator k = s.begin();
	     k != s.end(); k++) {
	  node_descriptor child = *k;
	  float nw = boost::get(total_weight_,child) * scale / width;
	  ret += visit(box_type(xmin(b),y,xmin(b)+width,y+nw),
		       child, depth+1);
	  y += nw;
	}
	set_xmin(b, width + xmin(b));
	drawer_.end_strip(b, n, depth, bottom_to_top);
      }
      else {
	dist_type w = width(b);
	float x = xmin(b);
	float width = compute_width(s.begin(), s.end(), w, scale);

	drawer_.begin_strip(b, n, depth, left_to_right);

	for (strips<Tree>::strip::const_iterator k = s.begin();
	     k != s.end(); k++) {
	  node_descriptor child = *k;
	  float nw = boost::get(total_weight_,child) * scale / width;
	  ret += visit(box_type(x, ymin(b), x+nw, ymin(b)+width),
		       child, depth+1);
	  x += nw;
	}
	set_ymin(b, width + ymin(b));
	drawer_.end_strip(b, n, depth, left_to_right);
      }
    }
    return ret;
  }

  unsigned visit(const box_type& box,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth = 0)
  {
    if (! drawer_.begin_box(box,n,depth)) return 0;
    box_type b(box);
    unsigned ret = 1;
    drawer_.draw_border(b, n, depth);
    if (is_leaf(n,tree_)) {
      drawer_.draw_box(b, n, depth);
    }
    else {
      ret = visit_strips(b, n, depth);
    }
    drawer_.end_box(box,n, depth);
    return ret;
  }    
  
  const typename strips<Tree>& strips_;
};

} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_STRIPS_HPP

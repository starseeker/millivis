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
#ifndef TREEMAP2_FASTPICKER_HPP
#define TREEMAP2_FASTPICKER_HPP

#include <infovis/drawing/Font.hpp>
#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/treemap/drawing/pick_drawer.hpp>

#include <types.hpp>
#include <BorderDrawer.hpp>

namespace infovis {

class FastPicker : public null_drawer<Tree,Box>
{
public:
  FastPicker(const Tree& tree, const BorderDrawer& b);

  void start();

  void enter(const Box& b, node_descriptor n,
	     unsigned depth) {
    hit_ = n;
    path_boxes.push_back(b);
  }
  void add_label(const Point& ctr, node_descriptor n) {
    label_centers.push_back(ctr);
    labels.push_back(n);
  }

  void set_show_all_labels(bool b) {
    show_all_labels_ = b;
  }

  void toggle_show_all_labels() {
    show_all_labels_  = ! show_all_labels_;
  }

  node_descriptor getHit() const { return hit_; }
  const PathBoxes getPathBoxes() const { return path_boxes; }

  void set_pos(int x, int y);
  int get_x_pos() const { return x_pos; }
  int get_y_pos() const { return y_pos; }
  
  void set_label_level(int l);
  void set_labels_clip(Box b);

  bool begin_box(const Box& b,
		 node_descriptor n,
		 unsigned depth) {
    if (int(xmin(b)) == int(xmax(b)) ||
	int(ymin(b)) == int(ymax(b)))
      return false;

    if (empty(labels_clip)) {
      if (depth == label_level &&
	  //! filter_->fast_get(n) &&
	  (show_all_labels_ || ! is_leaf(n, get_tree()))) {
	add_label(Point(xmin(b) + width(b)/2,
			ymin(b) + height(b)/2),
		  n);
      }
      if (infovis::inside(b, x_pos, y_pos)) {
	enter(b, n, depth);
	return true;
      }
      return (depth < label_level);
    }
    else if (! infovis::intersects(labels_clip, b)) {
      return false;
    }
    if (depth == label_level &&
	//! filter_->fast_get(n) &&
	(show_all_labels_ || ! is_leaf(n, border.tree_))) {
      add_label(Point(xmin(b) + width(b)/2,
		      ymin(b) + height(b)/2), n);
    }
    return true;
    // Continue picking
  }
  void draw_box(const Box& b,
		node_descriptor n,
		unsigned depth) { }
  void draw_border(Box& b, 
		   node_descriptor n,
		   unsigned depth) {
    if (border.begin_border(b, n, depth)) {
      border.remaining_box(b, n, depth);
    }
  }
  void remove_border(Box& b, 
		   node_descriptor n,
		   unsigned depth) {
    draw_border(b, n, depth);
  }
  void end_box(const Box& b, 
	       node_descriptor n,
	       unsigned depth) { }
  
  void finish(const Box& bounds,
	      Font * font,
	      float line_alpha,
	      const StringColumn& name);

  const Tree& get_tree() const { return border.tree_; }
protected:
  int x_pos, y_pos;
  node_descriptor hit_;
  std::vector<Point,gc_alloc<Point,true> > label_centers;
  Path labels;
  PathBoxes path_boxes;
  int label_level;
  Box labels_clip;
  const BorderDrawer& border;
  bool show_all_labels_;
};

} // namespace infovis

#endif // TREEMAP2_FASTPICKER_HPP

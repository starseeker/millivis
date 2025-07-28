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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_GL_DRAWER_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_GL_DRAWER_HPP

#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/treemap/drawing/drawer.hpp>
#include <infovis/tree/treemap/drawing/color_drawer.hpp>
#include <infovis/tree/treemap/drawing/border_drawer.hpp>
#include <infovis/drawing/box.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

/**
 * Drawer using OpenGL
 */
template <class Tree, class Box,
	  class ColorDrawer = color_drawer<Tree>,
	  class BorderDrawer = border_drawer<Tree, Box>
>
struct gl_drawer : public null_drawer<Tree,Box>
{
  ColorDrawer color;
  BorderDrawer border;

  void start() { }
  void finish() { }
  bool begin_box(const Box& b,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
    return (width(b) > 0) && (height(b) > 0);
  }
  void draw_box(const Box& b,
		typename tree_traits<Tree>::node_descriptor n,
		unsigned depth) {
    infovis::set_color(color(n, depth));
    infovis::draw_box(b);
  }
  void draw_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    if (border.begin_border(b, n, depth)) {
      set_color(color_black);
      Box b_box = b;
      if (border.left_border(b_box, n, depth)) {
	infovis::draw_box(b_box);
	b_box = b;
      }
      if (border.top_border(b_box, n, depth)) {
	infovis::draw_box(b_box);
	b_box = b;
      }
      if (border.right_border(b_box, n, depth)) {
	infovis::draw_box(b_box);
	b_box = b;
      }
      if (border.bottom_border(b_box, n, depth)) {
	infovis::draw_box(b_box);
      }
      border.remaining_box(b, n, depth);
    }
  }
  void remove_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    if (border.begin_border(b, n, depth)) {
      border.remaining_box(b, n, depth);
    }
  }
  void end_box(const Box& b, 
	       typename tree_traits<Tree>::node_descriptor n,
	       unsigned depth) { }
};

} // namespace infovis

#endif // INFOVIS_TREEMAP_DRAWING_DRAWER_HPP

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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_GL_CS_DRAWER_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_GL_CS_DRAWER_HPP

#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/treemap/drawing/gl_drawer.hpp>
#include <infovis/tree/treemap/drawing/color_drawer.hpp>
#include <infovis/tree/treemap/drawing/border_drawer.hpp>
#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <vector>

namespace infovis {

#define BufSize 10000

/**
 * OpenGL drawer using vertex arrays
 */
template <class Tree,
	  class Box,
	  class ColorDrawer = color_drawer<Tree>,
	  class BorderDrawer = border_drawer<Tree,Box>
>
struct gl_cs_drawer : public null_drawer<Tree,Box>
{
  typedef box_traits<Box>::coord_type Coord;

  gl_cs_drawer(const Tree& t) : tree(t) { init(); }
  gl_cs_drawer(const Tree& t, ColorDrawer c)
    : tree(t), color(c) { init(); }
  gl_cs_drawer(const Tree& t, BorderDrawer b)
    : tree(t), border(b) { init(); }
  gl_cs_drawer(const Tree& t, ColorDrawer c, BorderDrawer b)
    : tree(t), color(c), border(b) { init(): }

  void init() {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    current = 0;
  }
  void flush() {
    glDrawArrays(GL_QUADS, 0, current);
    current = 0;
  }
  void start() {
    current = 0;
    glColorPointer(Color::last_channel, type_of(colors[0][0]), 0, colors);
    glVertexPointer(2, type_of(x(vertices[0])), 0, vertices);
  }
  void push(const Box& b, unsigned depth, const Color& c) {
    if ((BufSize - current) <= 4) {
      flush();
    }
    vertices[current] = min(b);
    colors[current] = c;
    current++;
    vertices[current] = Point(xmax(b), ymin(b));
    colors[current] = c;
    current++;
    vertices[current] = Point(xmax(b), ymax(b));
    colors[current] = c;
    current++;
    vertices[current] = Point(xmin(b), ymax(b));
    colors[current] = c;
    current++;
  }

  bool begin_box(const Box& b,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
    return (width(b) > 0 && height(b) > 0);
  }
  void draw_box(const Box& b,
		typename tree_traits<Tree>::node_descriptor n,
		unsigned depth) {
    push(b, depth, color(n, depth));
  }
  void draw_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    if (border.begin_border(b, n, depth)) {
      const Color& c = (degree(n, tree) > 0) ? color_white : color_black;
      Box b_box = b;
      if (border.left_border(b_box, n, depth)) {
        push(b_box, depth, c);
	b_box = b;
      }
      if (border.top_border(b_box, n, depth)) {
        push(b_box, depth, c);
	b_box = b;
      }
      if (border.right_border(b_box, n, depth)) {
        push(b_box, depth, c);
	b_box = b;
      }
      if (border.bottom_border(b_box, n, depth)) {
        push(b_box, depth, c);
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

  void finish() {
    flush();
  }

  const Tree& tree;
  Point vertices[BufSize];
  Color colors[BufSize];
  int current;
  ColorDrawer color;
  BorderDrawer border;
};

} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_DRAWING_GL_CS_DRAWER_HPP

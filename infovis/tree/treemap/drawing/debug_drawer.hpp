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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_DEBUG_DRAWER_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_DEBUG_DRAWER_HPP

#include <infovis/tree/treemap/drawing/drawer.hpp>
#include <iostream>

namespace infovis {

template <class Tree, class Box, class Drawer>
struct debug_drawer : public Drawer
{
  enum State {
    drawer_inited,
    drawer_started,
    drawer_visiting,
    drawer_finished
  };
#ifndef NDEBUG
  State state;
#endif

  debug_drawer() : Drawer() {
#ifndef NDEBUG
    state = drawer_inited;
#endif
  }
  debug_drawer(const Drawer& d) : Drawer(d) {
#ifndef NDEBUG
    state = drawer_inited;
#endif
  }
  
  void start() {
#ifndef NDEBUG
    if (state != drawer_inited &&
	state != drawer_finished) {
      std::cerr << "Incorrect state in Drawer::start\n";
    }
    state = drawer_started;
#endif
    Drawer::start();
  }
  void finish() {
#ifndef NDEBUG
    if (state != drawer_started &&
	state != drawer_visiting) {
      std::cerr << "Incorrect state in Drawer::finish\n";
    }
    state = drawer_finished;
#endif
    Drawer::finish();
  }

  bool begin_box(const Box& b,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
#ifndef NDEBUG
    if (state != drawer_started &&
	state != drawer_visiting) {
      std::cerr << "Incorrect state in Drawer::begin_box\n";
    }
    state = drawer_visiting;
#endif
    return Drawer::begin_box(b, n, depth);
  }
  void draw_box(const Box& b,
		typename tree_traits<Tree>::node_descriptor n,
		unsigned depth) {
    Drawer::draw_box(b, n, depth);
  }
  void draw_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    Drawer::draw_border(b, n, depth);
  }
  void remove_border(Box& b, 
		     typename tree_traits<Tree>::node_descriptor n,
		     unsigned depth) {
    Drawer::remove_border(b, n, depth);
  }
  void end_box(const Box& b, 
	       typename tree_traits<Tree>::node_descriptor n,
	       unsigned depth) {
    Drawer::end_box(b, n, depth);
  }
};
} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_DRAWING_DEBUG_DRAWER_HPP

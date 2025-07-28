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

/**
 * Debug wrapper for treemap drawers
 * 
 * This template wraps any drawer implementation with debug state checking to
 * ensure proper calling sequence during treemap rendering. In debug builds,
 * it validates that drawer methods are called in the correct order.
 */
template <class Tree, class Box, class Drawer>
struct debug_drawer : public Drawer
{
  enum class State {
    inited,
    started,
    visiting,
    finished
  };
  
#ifndef NDEBUG
  State state = State::inited;
#endif

  debug_drawer() : Drawer() {
#ifndef NDEBUG
    state = State::inited;
#endif
  }
  
  explicit debug_drawer(const Drawer& d) : Drawer(d) {
#ifndef NDEBUG
    state = State::inited;
#endif
  }
  
  void start() {
#ifndef NDEBUG
    if (state != State::inited && state != State::finished) {
      std::cerr << "Incorrect state in Drawer::start\n";
    }
    state = State::started;
#endif
    Drawer::start();
  }
  void finish() {
#ifndef NDEBUG
    if (state != State::started && state != State::visiting) {
      std::cerr << "Incorrect state in Drawer::finish\n";
    }
    state = State::finished;
#endif
    Drawer::finish();
  }

  bool begin_box(const Box& b, typename tree_traits<Tree>::node_descriptor n, unsigned depth) {
#ifndef NDEBUG
    if (state != State::started && state != State::visiting) {
      std::cerr << "Incorrect state in Drawer::begin_box\n";
    }
    state = State::visiting;
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

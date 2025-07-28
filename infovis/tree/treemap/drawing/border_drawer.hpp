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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_BORDER_DRAWER_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_BORDER_DRAWER_HPP

#include <infovis/tree/tree_traits.hpp>
#include <infovis/drawing/box.hpp>

namespace infovis {

/**
 * Class for drawing border
 */
template <class Tree, class Box>
struct border_drawer
{
  typedef typename box_traits<Box>::coord_type coord_type;

  bool begin_border(Box& b, 
		    typename tree_traits<Tree>::node_descriptor n,
		    unsigned depth) const {
    return (width(b) > 3 && height(b) > 3);
  }
  bool left_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth) const {
    b = Box(xmin(b), ymax(b)-1, xmax(b), ymax(b));
    return true;
  }
  bool right_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		    unsigned depth) const {
    return false;
  }
  bool top_border(Box& b, 
		  typename tree_traits<Tree>::node_descriptor n,
		  unsigned depth) const {
    b = Box(xmin(b), ymin(b), xmin(b)+1, ymax(b)-1);
    return true;
  }
  bool bottom_border(Box& b, 
		     typename tree_traits<Tree>::node_descriptor n,
		     unsigned depth) const {
    return false;
  }
  void remaining_box(Box& b,
		     typename tree_traits<Tree>::node_descriptor n,
		     unsigned depth) const {
    set_xmin(b,xmin(b)+1);
    set_ymax(b,ymax(b)-1);
  }
};


} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_DRAWING_BORDER_DRAWER_HPP

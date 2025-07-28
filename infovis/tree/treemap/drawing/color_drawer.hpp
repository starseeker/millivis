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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_COLOR_DRAWER_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_COLOR_DRAWER_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/tree/tree_traits.hpp>

namespace infovis {

/**
 * Default drawer for drawing different colors.
 */ 
template <class Tree>
struct color_drawer {
  const Color& operator()(typename tree_traits<Tree>::node_descriptor n,
			  unsigned depth) const {
    switch(depth % 6) {
    case 0:
      return color_red;
      break;
    case 1:
      return color_green;
      break;
    case 2:
      return color_blue;
      break;
    case 3:
      return color_cyan;
      break;
    case 4:
      return color_magenta;
      break;
    case 5:
      return color_yellow;
      break;
    }
    return color_half_grey;		// will not happen
  }
};

} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_DRAWING_COLOR_DRAWER_HPP

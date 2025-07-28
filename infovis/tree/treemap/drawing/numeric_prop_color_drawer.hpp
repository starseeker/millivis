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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_NUMERIC_PROP_COLOR_DRAWER_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_NUMERIC_PROP_COLOR_DRAWER_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/tree/property_tree.hpp>

namespace infovis {

/**
 * Sample color drawer using the a numeric property
 */
struct numeric_prop_color_drawer
{
  typedef property_tree Tree;
  typedef tree_traits<Tree>::node_descriptor node_descriptor;
  const Tree::numeric_prop * color_prop;
  

  numeric_prop_color_drawer()
    : color_prop(0) { }

  void set_color_prop(const property_tree::numeric_prop * c) {
    color_prop = c;
  }

  const Color& operator() (node_descriptor n,
			   unsigned depth) const {
    int code = int((*color_prop)[n]);
    switch(code) {
    case 1:
    case 2: return (color_white);
    case 3: return (color_red);
    case 4: return(color_green);
    case 5: return (color_blue);
    case 6: return(color_cyan);
    case 7: return(color_magenta);
    case 8: return(color_yellow);
    default: return color_half_grey;
    }
  }
};

} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_DRAWING_NUMERIC_PROP_COLOR_DRAWER_HPP

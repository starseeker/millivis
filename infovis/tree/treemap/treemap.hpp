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
#ifndef INFOVIS_TREE_TREEMAP_TREEMAP_HPP
#define INFOVIS_TREE_TREEMAP_TREEMAP_HPP

#include <infovis/alloc.hpp>
// TODO: Removed boost/config.hpp - C++17 modernization
#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/tree_concepts.hpp>
#include <infovis/table/filter.hpp>
#include <infovis/tree/sum_weight_visitor.hpp>
#include <infovis/tree/treemap/drawing/drawer.hpp>

namespace infovis {

/**
 * Base treemap class.
 */
template <class Tree,
	  class Box,
	  class WeightMap,
	  class Drawer = null_drawer<Tree,Box>,
	  class Filter = filter_none
>
struct treemap
{
  typedef Box box_type;
  typedef typename box_traits<Box>::coord_type coord_type;
  typedef typename box_traits<Box>::dist_type dist_type;
  typedef typename tree_traits<Tree>::children_iterator children_iterator;
  typedef typename tree_traits<Tree>::node_descriptor node_descriptor;
    
  treemap(const Tree& tree,
	  WeightMap wm,
	  Drawer drawer = Drawer(),
	  Filter filter = Filter())
    : tree_(tree),
      weight_(wm),
      drawer_(drawer),
      filter_(filter)
  { }

  void start() { drawer_.start(); }
  void finish() { drawer_.finish(); }

  const Tree& tree_;
  WeightMap weight_;
  Drawer drawer_;
  Filter filter_;
};

} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_TREEMAP_HPP


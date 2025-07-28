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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_DRAWER_CONCEPTS_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_DRAWER_CONCEPTS_HPP

#include <infovis/alloc.hpp>
#include <infovis/drawing/box_concepts.hpp>
#include <infovis/tree/tree_concepts.hpp>

namespace infovis {

/**
 * Concept (abstract requirements) for a treemap
 */
template <class Tree, class Box>
struct TreemapDrawerConcept
{
  typedef typename tree_traits<Tree>::node_descriptor node_descriptor;
  typedef typename tree_traits<Tree>::children_iterator children_iterator;    

  void constraints() {
    function_requires<TreeConcept<Tree> >();
    function_requires<BoxConcept<Box> >();

    start();
    begin_box(b, n, depth);
    begin_strip(b, n, depth);
    end_strip(b, n, depth);
    draw_box(b, n, depth);
    draw_border(vb, n, depth);
    remove_border(vb, n, depth);
    end_box(b, n, depth);
    finish();
  }

  const Box b;
  Box vb;
  node_descriptor n;
  const Tree t;
  unsigned depth;
};

} // namespace infovis


#endif // INFOVIS_TREE_TREEMAP_DRAWING_DRAWER_CONCEPTS_HPP

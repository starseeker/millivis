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
#ifndef INFOVIS_DRAWING_LITE_LITE_AS_TREE_HPP
#define INFOVIS_DRAWING_LITE_LITE_AS_TREE_HPP

#include <infovis/drawing/lite/Lite.hpp>
#include <infovis/tree/tree_traits.hpp>

namespace infovis {

/**
 * Specialize tree_traits to handle a tree of lite as a tree.
 */
template<>
struct tree_traits<const Lite*>
{
  typedef const Lite* node_descriptor;    
  typedef Lite::const_iterator children_iterator;    
  typedef Lite::List::size_type degree_size_type;
  static node_descriptor nil() { return 0; }
};

inline tree_traits< const Lite* >::node_descriptor
root(const Lite * t)
{
  return t;
}

inline std::pair< tree_traits< const Lite* >::children_iterator,
		  tree_traits< const Lite* >::children_iterator>
children(tree_traits< const Lite* >::node_descriptor n, const Lite*)
{
  return
    std::pair<
    tree_traits< const Lite* >::children_iterator,
    tree_traits< const Lite* >::children_iterator
    >(n->begin(), n->end());
}

inline tree_traits< const Lite* >::degree_size_type
degree(tree_traits< const Lite* >::node_descriptor n,
       const Lite*)
{
  return n->childCount();
}

inline bool
is_leaf(tree_traits< const Lite* >::node_descriptor n,
       const Lite* l)
{
  return degree(n, l);
}

} // namespace infovis
#endif // INFOVIS_DRAWING_LITE_LITE_AS_TREE_HPP

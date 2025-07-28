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
//  (C) Copyright Jeremy Siek 1999. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

#ifndef INFOVIS_TREE_TRAITS_HPP
#define INFOVIS_TREE_TRAITS_HPP

#include <infovis/alloc.hpp>
#include <boost/config.hpp>
#include <boost/tuple/tuple.hpp>

namespace infovis {

/**
 * Traits of a tree.
 * This class is the default set of types and static methods a tree
 * structure should offer to be usable to all the algorithms.
 */
template <class T>
struct tree_traits {
  typedef  typename T::node_descriptor node_descriptor; /// Type to reference a node.
  typedef typename T::children_iterator children_iterator; /// Type to reference an iterator over the children of a node
  typedef typename T::degree_size_type degree_size_type; /// Type for the degree of nodes
  static node_descriptor nil() { return T::nil(); } /// static method to access the value of the root of the tree
};

} /* namespace infovis */

#endif /* INFOVIS_TREE_TRAITS_HPP */

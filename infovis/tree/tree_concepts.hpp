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
#ifndef INFOVIS_TREE_CONCEPTS_HPP
#define INFOVIS_TREE_CONCEPTS_HPP

#include <infovis/alloc.hpp>
// TODO: Replaced Boost concept checking with minimal stubs for C++17 modernization
// Full C++20 concepts migration should be done in a future pass
#include <infovis/tree/tree_traits.hpp>

// Minimal concept checking stubs to replace Boost - TODO: upgrade to C++20 concepts later
namespace boost {
  template<typename T> struct DefaultConstructibleConcept { void constraints() {} };
  template<typename T> struct EqualityComparableConcept { void constraints() {} };  
  template<typename T> struct AssignableConcept { void constraints() {} };
  template<typename T> struct ForwardIteratorConcept { void constraints() {} };
  template<typename T> struct InputIteratorConcept { void constraints() {} };
  template<typename T> void function_requires() {}
}

namespace infovis {

/**
 * Concept (abstract requirements) for a tree data structure.
 *
 * A tree should provide the following information:
 * - a node_descriptor type for representing a node
 * - a children_iterator type for iterating over the children of a node
 * - a degree_size_type type for holding the degree of a node
 * - a root(tree) and tree::nil() method to return the root node
 * - a children(node) method to get the children iterator for a node
 * - a degree(node) method to get the degree of a node
 * - a is_leaf(node) method to test whether a node is a leaf (degree==0)
 *
 * Furthermore, it should implement the following concepts:
 * - the node_descriptor should implement the DefaultConstructibleConcept
 * - the node_descriptor should implement the EqualityComparableConcept
 * - the node_descriptor should implement the AssignableConcept
 * - the children_iterator should implement the ForwardIteratorConcept
 */
template <class T>
struct TreeConcept
{
  /// tree_traits<T> should define a node_descriptor type
  typedef typename tree_traits<T>::node_descriptor node_descriptor;

  /// tree_traits<T> should define a children_iterator type
  typedef typename tree_traits<T>::children_iterator children_iterator;    

  /// Check the constraints of the tree type.
  void constraints() {
    boost::function_requires< boost::DefaultConstructibleConcept<node_descriptor> >();
    boost::function_requires< boost::EqualityComparableConcept<node_descriptor> >();
    boost::function_requires< boost::AssignableConcept<node_descriptor> >();
    boost::function_requires< boost::ForwardIteratorConcept<children_iterator> >();

    n = tree_traits<T>::nil();
    n = *p.first;
    const_constraints(t);
  }

  /// Check the constraints of a constant tree type.
  void const_constraints(const T& t) {
    n = root(t);
    p = children(n, t);
    d = degree(n, t);
    if (is_leaf(n, t))
      n = *p.first;
  }

  std::pair<children_iterator, children_iterator> p;
  typename tree_traits<T>::node_descriptor n;
  typename tree_traits<T>::degree_size_type d;
  T t;
};

/**
 * Concept (Abstract requirements) for a tree maintaining parents information
 *
 * A ParentedTreeConcept should implement the TreeConcept and provide
 * the parent(node) method.
 */
template <class T>
struct ParentedTreeConcept
{
  void constraints() {
    boost::function_requires< TreeConcept<T> >();
    n = parent(n, t);
  }
  void const_constraints(const T& t) {
    n = parent(n, t);
  }
  typename tree_traits<T>::node_descriptor n;
  T t;
};

/**
 * Concept (Abstract requirements) for a tree maintaining a list of all its nodes.
 *
 * A NodeListTreeConcept implements the TreeConcept and requires an
 * iterator type node_iterator to iterator over all the nodes of the
 * tree, and the methods nodes(const tree&) to get the first and last
 * iterator, as well as the method num_nodes(const tree&) to get the
 * count of nodes in the tree.
 */
template <class T>
struct NodeListTreeConcept
{
  typedef typename tree_traits<T>::node_iterator node_iterator;
  typedef typename tree_traits<T>::nodes_size_type nodes_size_type;

  void constraints() {
    boost::function_requires< TreeConcept<T> >();
    boost::function_requires< boost::InputIteratorConcept<node_iterator> >();

    p = nodes(t);
    n = *(p.first);
    const_constraints(t);
  }
  void const_constraints(const T& t) {
    p = nodes(t);
    n = *(p.first);
    N = num_nodes(t);
  }
  std::pair<node_iterator,node_iterator> p;
  typename tree_traits<T>::node_descriptor n;
  T t;
  nodes_size_type N;
};

/**
 * Concept (Abstract requirements) for a tree that can be built
 * dynamically.
 *
 * A BuildableTreeConcept implements the TreeConcept and requires a
 * method add_node(node_descriptor n, table& t) that returns a new
 * node from a parent node and a tree.
 */
template <class T>
struct BuildableTreeConcept
{
  void constraints() {
    boost::function_requires< TreeConcept<T> >();
    n = add_node(n, t);
  }
  typename tree_traits<T>::node_descriptor n;
  T t;
};

/// Concept (Abstract requirements) for a tree that can be dynamically modified
template <class T>
struct MutableTreeConcept
{
  void constraints() {
    boost::function_requires< BuildableTreeConcept<T> >();
    remove_node(n, t);
    clear_node(n, t);
  }
  typename tree_traits<T>::node_descriptor n;
  T t;
};
} // namespace infovis

#endif

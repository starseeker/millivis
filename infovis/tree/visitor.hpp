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
#ifndef INFOVIS_TREE_VISITOR_HPP
#define INFOVIS_TREE_VISITOR_HPP

#include <infovis/alloc.hpp>
#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/tree_concepts.hpp>
#include <tuple> // TODO: Added for std::tie - C++17 modernization

namespace infovis {

/**
 * Method for traversing a tree that implements the TreeConcept.
 *
 * @param v a node
 * @param t a tree
 * @param visitor a TreeVisitor (@see null_tree_visitor)
 */
template <class Tree, class TreeVisitor>
void traverse_tree(typename tree_traits<Tree>::node_descriptor v,
		   const Tree& t, TreeVisitor& visitor)
{
  boost::function_requires< TreeConcept<Tree> > ();
  visitor.preorder(v);
  typedef typename tree_traits<Tree>::children_iterator children_iterator;
  children_iterator i, end;
  std::pair<children_iterator,children_iterator> p = children(v,t);
  // TODO: Replaced boost::tie with std::tie - C++17 modernization  
  std::tie(i, end) = children(v, t);
  if (i != end) {
    traverse_tree(*i++, t, visitor);
    visitor.inorder(v);
    while (i != end)
      traverse_tree(*i++, t, visitor);
  } else
    visitor.inorder(v);
  visitor.postorder(v);
}

/**
 * The simplest tree visitor.
 *
 * A tree visitor should implment three methods:
 * - preorder(Node)
 * - inorder(Node)
 * - postorder(Node).
 */
struct null_tree_visitor {
  /**
   * Called when initially entering the node in a depth first traversal
   */
  template <class Node> void preorder(Node) { }
  /**
   * Called after some children of the node have been visited in a
   * depth first traversal.
   */
  template <class Node> void inorder(Node) { }
  /**
   * Called after the children of the node have been visited in a
   * depth first traversal.
   */
  template <class Node> void postorder(Node) { }
};

template <class Tree, class TreeSearcher>
void search_tree(typename tree_traits<Tree>::node_descriptor v,
		 const Tree& t, TreeSearcher& searcher)
{
  boost::function_requires< TreeConcept<Tree> > ();
  if (searcher.preorder(v)) {
    typedef typename tree_traits<Tree>::children_iterator children_iterator;
    children_iterator i, end;
    std::pair<children_iterator,children_iterator> p = children(v,t);
    tie(i, end) = children(v, t);
    if (i != end) {
      search_tree(*i++, t, searcher);
      searcher.inorder(v);
      while (i != end)
	search_tree(*i++, t, searcher);
    } else
      searcher.inorder(v);
    searcher.postorder(v);
  }
}

/**
 * Visitor for searching
 */ 
struct null_tree_searcher {
  template <class Node> bool preorder(Node) { return true; }
  template <class Node> void inorder(Node) { }
  template <class Node> void postorder(Node) { }
};



} // namespace infovis

#endif // INFOVIS_TREE_VISITOR_HPP

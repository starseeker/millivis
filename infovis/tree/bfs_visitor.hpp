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
#ifndef INFOVIS_TREE_BFS_VISITOR_HPP
#define INFOVIS_TREE_BFS_VISITOR_HPP

#include <infovis/alloc.hpp>
#include <boost/pending/queue.hpp>
#include <infovis/tree/visitor.hpp>

namespace infovis {

/**
 * Breadth first search visitor concept
 */
template <class Visitor, class Tree>
struct TreeBFSVisitorConcept {
  void constraints() {
    boost::function_requires< boost::CopyConstructibleConcept<Visitor> >();
    vis.initialize_node(n, t);
    bool v = vis.discover_node(n, t);
    vis.examine_node(n, t);
    vis.finish_node(n, t);
  }
  Visitor vis;
  Tree t;
  typename tree_traits<Tree>::node_descriptor n;
};
  
/**
 * Default breadth first search visitor, doing nothing.
 */
struct null_bfs_visitor {
  template <class Tree>
  void initialize_node(typename tree_traits<Tree>::node_descriptor n,
		       const Tree& t) { }
    
  template <class Tree>
  bool discover_node(typename tree_traits<Tree>::node_descriptor n,
		     const Tree& t) { return true; }

  template <class Tree>
  void examine_node(typename tree_traits<Tree>::node_descriptor n,
		    const Tree& t) { }

  template <class Tree>
  void finish_node(typename tree_traits<Tree>::node_descriptor n,
		   const Tree& t) { }
    
};

/**
 * Method to visit a tree using a breadth first search traversal.
 * @param n the starting node
 * @param t the tree
 * @param vis the Breadth first search visitor
 * @param Q a Buffer for holding nodes.
 */
template <class Tree, class BFSVisitor, class Buffer>
void bfs_visit(typename tree_traits<Tree>::node_descriptor n,
	       const Tree& t,
	       BFSVisitor vis, Buffer& Q)
{
  boost::function_requires< TreeConcept<Tree> >();
  boost::function_requires< TreeBFSVisitorConcept<BFSVisitor,Tree> >();
  typedef tree_traits<Tree> TTraits;
  typedef typename TTraits::node_descriptor node;

  if (vis.discover_node(n, t))
    Q.push(n);
  while (! Q.empty()) {
    node u = Q.top();
    Q.pop();
    vis.examine_node(u, t);
    typename TTraits::children_iterator i, end;
    for (boost::tie(i, end) = children(u, t); i != end; i++) {
      node v = *i;
      if (vis.discover_node(v, t))
	Q.push(v);
    }	// for
    vis.finish_node(n, t);
  } // while
}

/**
 * Method to visit a tree using a breadth first search traversal.
 * @param n the starting node
 * @param t the tree
 * @param vis the Breadth first search visitor
 */
template <class Tree, class BFSVisitor>
void bfs_visit(typename tree_traits<Tree>::node_descriptor n,
	       const Tree& t,
	       BFSVisitor vis)
{
  typedef boost::queue<tree_traits<Tree>::node_descriptor> Buffer;
  Buffer Q;
  bfs_visit(n, t, vis, Q);
}

/**
 * Method to search a tree using a breadth first search traversal.
 * @param n the starting node
 * @param t the tree
 * @param vis the Breadth first search visitor
 * @param Q a Buffer for holding nodes.
 */
template <class Tree, class BFSVisitor, class Buffer >
void bfs_search(typename tree_traits<Tree>::node_descriptor n,
		const Tree& t,
		BFSVisitor vis, Buffer& Q)
{
  typename tree_traits<Tree>::node_iterator i, end;
  for (tie(i, end) = nodes(t); i != end; i++) {
    vis.initialize_node(*i, t);
  }
  bfs_visit(t, n, Q, vis);
}

/**
 * Method to search a tree using a breadth first search traversal.
 * @param n the starting node
 * @param t the tree
 * @param vis the Breadth first search visitor
 */
template <class Tree, class BFSVisitor>
void bfs_search(typename tree_traits<Tree>::node_descriptor n,
		const Tree& t,
		BFSVisitor vis)
{
  typedef typename boost::queue<typename tree_traits<Tree>::node_descriptor> Buffer;
  Buffer Q;
  bfs_search(n, t, vis, Q);
}


} // namespace infovis 

#endif // INFOVIS_TREE_BFS_VISITOR_HPP

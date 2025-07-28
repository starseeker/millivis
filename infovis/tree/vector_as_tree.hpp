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
#ifndef INFOVIS_TREE_VECTOR_AS_TREE_HPP
#define INFOVIS_TREE_VECTOR_AS_TREE_HPP

#include <infovis/alloc.hpp>
// TODO: Removed boost/config.hpp - C++17 modernization
#include <cassert>

#include <vector>
#include <algorithm>
#include <tuple> // TODO: Added for std::tie - C++17 modernization

#include <infovis/tree/tree_traits.hpp>
// TODO: Removed unused boost/pending/property.hpp - C++17 modernization

namespace infovis {

typedef std::vector<
  unsigned,
  gc_alloc<unsigned,true> 
> vector_as_tree_children;

typedef std::vector<
  vector_as_tree_children,
  gc_alloc<vector_as_tree_children>
> vector_as_tree;

/**
 * tree_traits specialization for considering a vector of unsigned as a tree.
 */
template <>
struct tree_traits< vector_as_tree >
{
  typedef unsigned node_descriptor;
  typedef std::vector<unsigned,gc_alloc<unsigned,true> > NodeList;
  typedef NodeList::const_iterator children_iterator;

  typedef NodeList::size_type degree_size_type;
  // NodeListTreeConcept
  typedef vector_as_tree::const_iterator node_iterator;
  typedef vector_as_tree::size_type nodes_size_type;
  static node_descriptor nil() { return unsigned(-1); }
};

inline tree_traits< vector_as_tree >::node_descriptor
root(const vector_as_tree& t)
{
  return t.size() == 0 ? unsigned(-1) : 0;
}

inline std::pair< tree_traits< vector_as_tree >::children_iterator,
		  tree_traits< vector_as_tree >::children_iterator>
children(tree_traits< vector_as_tree >::node_descriptor n,
	 const vector_as_tree& t)
{
  return
    std::pair< tree_traits< vector_as_tree >::children_iterator,
    tree_traits< vector_as_tree >::children_iterator>(t[n].begin(),
						      t[n].end());
}
inline tree_traits< vector_as_tree >::degree_size_type
degree(tree_traits< vector_as_tree >::node_descriptor n,
       const vector_as_tree& t)
{
  return t[n].size();
}

inline bool
is_leaf(tree_traits< vector_as_tree >::node_descriptor n,
       const vector_as_tree& t)
{
  return t[n].empty();
}

inline tree_traits< vector_as_tree >::nodes_size_type
num_nodes(const vector_as_tree& t)
{
  return t.size();
}

inline std::pair< tree_traits< vector_as_tree >::node_iterator,
		  tree_traits< vector_as_tree >::node_iterator >
nodes(const vector_as_tree& t)
{
  return
    std::pair< tree_traits< vector_as_tree >::node_iterator,
    tree_traits< vector_as_tree >::node_iterator >(t.begin(), t.end());
}

inline tree_traits< vector_as_tree >::node_descriptor
add_node(tree_traits< vector_as_tree >::node_descriptor n,
	 vector_as_tree& t)
{
  tree_traits< vector_as_tree >::node_descriptor nn = t.size();
  t.push_back(vector_as_tree_children());
  if (n != unsigned(-1))
    t[n].push_back(nn);
  return nn;
}

inline void
remove_node(tree_traits< vector_as_tree >::node_descriptor n,
	    vector_as_tree& t)
{
  assert(!"implemented");
}

inline void
clear_node(tree_traits< vector_as_tree >::node_descriptor n,
	   vector_as_tree& t)
{
  assert(!"implemented");
}

inline void
clear(vector_as_tree& t)
{
  t.erase(t.begin(), t.end());
}

template <class Compare>
inline void sort_node(tree_traits< vector_as_tree >::node_descriptor n,
		      vector_as_tree& t, Compare comp)
{
  std::sort(t[n].begin(), t[n].end(), comp);
}

template <class Compare>
inline void sort(vector_as_tree& t, Compare comp)
{
  tree_traits<vector_as_tree>::node_iterator b, i, e;
  // TODO: Replaced boost::tie with std::tie - C++17 modernization
  std::tie(b, e) = nodes(t);
  for (i = b; i != e; i++) {
    sort_node(i - b, t, comp);
  }
}

// get/put helpers
template <class T, class V, class A>
inline void put(std::vector<T,A>& pa, int k, const V& val) { pa[k] = val;  }

template <class T,class A>
inline const T& get(const std::vector<T,A>& pa, int k) { return pa[k]; }

} // namespace infovis
#endif // INFOVIS_TREE_VECTOR_AS_TREE_HPP

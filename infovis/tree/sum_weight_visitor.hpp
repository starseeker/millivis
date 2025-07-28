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
#ifndef INFOVIS_TREE_SUM_WEIGHT_VISITOR_HPP
#define INFOVIS_TREE_SUM_WEIGHT_VISITOR_HPP

#include <infovis/alloc.hpp>
#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/visitor.hpp>
#include <vector>

namespace infovis {

/**
 * Recursively compute the sum of the values of the children and store
 * in into the parent
 */
template <class Tree, class WeightMap>
struct sum_weight_visitor
{
  const Tree& tree_;
  WeightMap& weight_;
  typedef typename tree_traits<Tree>::node_descriptor node_descriptor;
  typedef typename tree_traits<Tree>::children_iterator children_iterator;

  sum_weight_visitor(const Tree& t, WeightMap& w)
    : tree_(t), weight_(w) { }

  void preorder(node_descriptor n) {}
  void inorder(node_descriptor n) {
  }
  void postorder(node_descriptor n) {
    if (! is_leaf(n, tree_)) {
      weight_[n] = 0;
      children_iterator i, e;
      boost::tie(i, e) = children(n, tree_);
      while (i != e) {
	weight_[n] += weight_[*i];
	++i;
      }
    }
  }
};

/**
 * Compute the sum of the weights in place.
 * @param t a tree
 * @param weights a column
 */
template <class Tree, class WeightMap>
void sum_weights(const Tree& t,
		 WeightMap& weight)
{
  sum_weight_visitor<Tree,WeightMap> vis(t, weight);
  traverse_tree(root(t), t, vis);
}

template <class Tree, class WeightMap>
struct test_sum_weight_visitor
{
  const Tree& tree_;
  WeightMap& weight_;
  typedef typename tree_traits<Tree>::node_descriptor node_descriptor;
  typedef typename tree_traits<Tree>::children_iterator children_iterator;

  test_sum_weight_visitor(const Tree& t, WeightMap& w)
    : tree_(t), weight_(w) { }

  void preorder(node_descriptor n) {}
  void inorder(node_descriptor n) {
  }
  void postorder(node_descriptor n) {
    if (! is_leaf(n, tree_)) {
      float sum = 0;
      children_iterator i, e;
      tie(i, e) = children(n, tree_);
      while (i != e) {
	sum += weight_[*i];
	++i;
      }
      if (sum != weight_[n])
	throw false;
    }
  }
};

template <class Tree, class WeightMap>
bool test_sum_weights(const Tree& t,
		      WeightMap& weight)
{
  test_sum_weight_visitor<Tree,WeightMap> vis(t, weight);
  try {
    traverse_tree(root(t), t, vis);
  }
  catch (bool b) {
    return b;
  }
  return true;
}

} // namespace infovis

#endif // INFOVIS_TREE_SUM_WEIGHT_VISITOR_HPP

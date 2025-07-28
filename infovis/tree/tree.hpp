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
#ifndef INFOVIS_TREE_TREE_HPP
#define INFOVIS_TREE_TREE_HPP

#include <infovis/table/table.hpp>
#include <infovis/table/column.hpp>
#include <infovis/tree/tree_traits.hpp>
#include <stdexcept>
#include <cassert>

namespace infovis {

/**
 * Base container for tree structured data types.
 * A tree is a table with topological information stored in four
 * columns.  An index represents a node number, with all associated
 * values stored in the columns.  The four internal columns are:
 * - child: the index of the first child for a given node
 * - next: the next sibling node
 * - last: the index of the last child for a given node
 * - parent: the index of the parent for a given node.
 *
 * This class is compatible with the tree_concepts.  It implements the
 * following concepts:
 * - TreeConcept
 * - ParentedTreeConcept
 * - NodeListTreeConcept
 * - BuildableTreeConcept
 * @see TreeConcept
 * @see ParentedTreeConcept
 * @see NodeListTreeConcept
 * @see BuildableTreeConcept
 */
class tree : public table
{
public:
  enum {
    root = 0,			/// the index of the root node
  };
  typedef unsigned node_descriptor; /// Type to reference a node (\sa tree_traits}
  typedef unsigned nodes_size_type;/// Type to reference an iterator over the children of a node (\sa tree_traits}
  typedef unsigned degree_size_type; /// Type for the degree of nodes (\sa tree_traits}

  class node_iterator;
  class children_iterator;
  class names_iterator;

  static node_descriptor nil() { return root; }	/// static method to access the value of the root of the tree (\sa tree_traits}

  /**
   * Default constructor for tree, with an initial capacity of 10 value.
   */
  tree(unsigned capacity = 10);

  /**
   * Copy constructor for tree.
   */
  tree(const tree& other);

  /**
   * Reset the tree, emptying it.
   */
  void reset();

  /**
   * Return the first child of a node.
   * @param n the node
   * @return the first child or the nil value if node has no children
   */
  node_descriptor child(node_descriptor n) const { return child_[n]; }

  /**
   * Return the next sibling of a node.
   * @param n the node
   * @return the next sibling or the nil value if node is the last sibling
   */
  node_descriptor next(node_descriptor n) const { return next_[n]; }

  /**
   * Return the last child of a node.
   * @param n the node
   * @return the last child or the nil value if node has no children
   */
  node_descriptor last(node_descriptor n) const { return last_[n]; }

  /**
   * Return the parent of a node.
   * @param n the node
   * @return the parent or the nil value if node is the top node
   */
  node_descriptor parent(node_descriptor n) const { return parent_[n]; }


  /**
   * Return the number of nodes in the tree.
   * @return the number of nodes in the tree
   */
  nodes_size_type num_nodes() const { return child_.size(); }

  /**
   * Return the iterator to the begining of the nodes in the tree.
   * @return the iterator to the begining of the nodes in the tree
   */
  node_iterator begin_nodes() const { return node_iterator(0); }

  /**
   * Return the iterator to the end of the nodes in the tree.
   * @return the iterator to the end of the nodes in the tree
   */
  node_iterator end_nodes() const { return node_iterator(num_nodes()); }


  /**
   * Return the iterator to the begining of the children of a node.
   * @param n the node
   * @return the iterator to the begining of the children of the node
   */
  children_iterator begin_child(node_descriptor n) const {
    return children_iterator(this, child_[n]);
  }

  /**
   * Return the iterator to the end of the children of a node.
   * @param n the node
   * @return the iterator to the end of the children of the node
   */
  children_iterator end_child(node_descriptor n) const {
    return children_iterator(this, root);
  }

  /**
   * Return the iterator to the begining of the names of the nodes.
   * @return the iterator to the begining of the names of the nodes
   */
  names_iterator begin_names() const {
    return names_iterator(this, 0);
  }

  /**
   * Return the iterator to the end of the names of the nodes.
   * @return the iterator to the end of the names of the nodes
   */
  names_iterator end_names() const {
    return names_iterator(this, column_count());
  }

  /**
   * Return the degree of a node, i.e. the number of children
   * @param n the node
   * @return the degree of the node
   */
  degree_size_type degree(node_descriptor n) const {
    node_descriptor last = last_[n];
    degree_size_type cnt = 0;
    for (node_descriptor c = child_[n]; c != last; c = next_[c])
      cnt++;
    return cnt;
  }

  /**
   * Return true if the node is a leaf_node.
   *
   * Use this method rather than degree(n)==0
   * @param n the node
   * @return true if the node is a leaf_node.
   */
  bool is_leaf(node_descriptor n) const {
    return child_[n] == nil();
  }


  /**
   * Add a node to the tree
   * @param n the parent of the node
   * @return the new node
   */
  virtual node_descriptor add_node(node_descriptor n);

  /**
   * Clear the table.
   */
  virtual void clear();


  /**
   * Outputs a readable representation of the tree.
   * @param out the output stream
   */
  virtual void print(std::ostream& out) const;

  template <class Compare>
  void sort_node(node_descriptor n, Compare comp) {
    if (is_leaf(n))
      return;
#if defined(STL_MEMBER_TEMPLATES)
    std::vector<node_descriptor, gc_alloc<node_descriptor> >
      children(begin_child(n), end_child(n));
#else
    std::vector<node_descriptor, gc_alloc<node_descriptor> > children;
    for (children_iterator ci = begin_child(n); ci != end_child(n); ci++) {
      children.push_back(*ci);
    }
#endif
    std::sort(children.begin(), children.end(), comp);
    child_[n] = children.front();
    last_[n] = children.back();
    next_[children.back()] = root;
    for (int i = 0; i < (children.size()-1); i++) {
      next_[children[i]] = children[i+1];
    }
  }


  /**
   * The node_iterator class for the tree type.
   */
  class node_iterator {
  public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef node_descriptor value_type;
    typedef int distance_type;
    typedef const node_descriptor& reference;
    typedef const node_descriptor* pointer;
    typedef distance_type difference_type;

    node_iterator() : cur_(0) { }
    node_iterator(node_descriptor n) : cur_(n) { }
    node_iterator(const node_iterator& other) : cur_(other.cur_) { }

    value_type operator * () const { return cur_; }
    node_iterator& operator ++() { cur_++; return *this; }
    node_iterator operator++(int) { cur_++; return node_iterator(cur_-1); }
    node_iterator& operator --() { cur_--; return *this; }
    node_iterator operator--(int) { cur_--; return node_iterator(cur_+1); }
    node_iterator operator + (int d) { return node_iterator(cur_+d); }
    node_iterator operator - (int d) { return node_iterator(cur_-d); }
    node_iterator& operator += (int d) { cur_+=d; return *this; }
    node_iterator& operator -= (int d) { cur_-=d; return *this; }

    bool operator == (const node_iterator& other) const { return cur_==other.cur_; }
    bool operator != (const node_iterator& other) const { return cur_!=other.cur_; }
  protected:
    value_type cur_;
  };


  /**
   * The children_iterator type for this tree type
   */
  class children_iterator  {
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef node_descriptor value_type;
    typedef int distance_type;
    typedef const node_descriptor& reference;
    typedef distance_type difference_type;
    typedef const node_descriptor* pointer;

    children_iterator()
      : tree_(0), cur_(0)
    { }
    children_iterator(const tree * tree, node_descriptor n)
      : tree_(tree),
	cur_(n)
    { }
    children_iterator(const children_iterator& other)
      : tree_(other.tree_),
	cur_(other.cur_)
    { }

    const value_type& operator * () const { return cur_; }
    
    children_iterator& operator ++() {
      cur_ = tree_->next(cur_);
      return *this;
    }
    children_iterator operator ++(int) {
      children_iterator tmp = *this;
      cur_ = tree_->next(cur_);
      return tmp;
    }
    int operator - (const children_iterator& other) const {
      return cur_ - other.cur_;
    }
    bool operator == (const children_iterator& other) const {
      return cur_ == other.cur_;
    }
    bool operator != (const children_iterator& other) const {
      return cur_ != other.cur_;
    }
  protected:
    const tree * tree_;
    node_descriptor cur_;
  };


  /**
   * The name_iterator type
   */
  class names_iterator  {
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef std::string value_type;
    typedef int distance_type;

    names_iterator()
      : tree_(0), col_(0)
    { }
    names_iterator(const tree * tree)
      : tree_(tree),
	col_(0)
    { }

    names_iterator(const tree * tree, int col)
      : tree_(tree),
	col_(col)
    { }
    names_iterator(const names_iterator& other)
      : tree_(other.tree_),
	col_(other.col_)
    { }

    value_type operator * () const {
      return tree_->get_column(col_)->get_name();
    }
    
    names_iterator& operator ++() {
      col_++;
      return *this;
    }
    names_iterator operator ++(int) {
      names_iterator tmp = *this;
      col_++;
      return tmp;
    }
    int operator - (const names_iterator& other) const {
      return col_ - other.col_;
    }
    bool operator == (const names_iterator& other) const {
      return tree_ == other.tree_ && col_ == other.col_;
    }
    bool operator != (const names_iterator& other) const {
      return tree_ != other.tree_ || col_ != other.col_;
    }
  protected:
    const tree * tree_;
    int col_;
  };

protected:
  UnsignedColumn child_;
  UnsignedColumn next_;
  UnsignedColumn last_;
  UnsignedColumn parent_;
};

/**
 * Sort nodes in the tree given a comparison function
 * t the tree
 * comp the comparison function
 */
template <class Compare>
inline void
sort(tree& t, Compare comp)
{
  for (tree::node_descriptor i = 0; i < t.num_nodes(); i++) {
    t.sort_node(i, comp);
  }
}

/**
 * Return the value of the root of the tree.
 * @return the value of the root of the tree
 * @see TreeConcept
 */
inline tree::node_descriptor
root(const tree& t)
{ return t.root; }

/**
 * Return a pair or children iterator for iterating over the children
 * of a node
 * @param n the node
 * @param t the tree
 * @return a pair or children iterator
 * @see TreeConcept
 */
inline std::pair< tree::children_iterator, tree::children_iterator>
children(tree::node_descriptor n, const tree& t)
{ return std::pair<
    tree::children_iterator,
    tree::children_iterator >(t.begin_child(n), t.end_child(n));
};

/**
 * Return the degree of a node
 * @param n the node
 * @param t the tree
 * @return the degree of the node
 * @see TreeConcept
 */
inline tree::degree_size_type
degree(tree::node_descriptor n, const tree& t)
{
  return t.degree(n);
}

/**
 * Return true if the node is a leaf node
 * @param n the node
 * @param t the tree
 * @return true if the node is a leaf node
 * @see TreeConcept
 */
inline bool
is_leaf(tree::node_descriptor n, const tree& t)
{
  return t.is_leaf(n);
}

/**
 * Return the number of nodes in the tree.
 * @param t the tree
 * @return the number of nodes in the tree
 * @see NodeListTreeConcept
 */
inline tree::nodes_size_type
num_nodes(const tree& t) { return t.num_nodes(); }

/**
 * Return a pair or node iterator for iterating over the children
 * of the tree.
 * @param t the tree
 * @return a pair or children iterator
 * @see NodeListTreeConcept
 */
inline std::pair< tree::node_iterator, tree::node_iterator>
nodes(const tree& t)
{
  return std::pair<
    tree::node_iterator,
    tree::node_iterator>(t.begin_nodes(), t.end_nodes());
}

/**
 * Return the parent of a node.
 *
 * @param n the node
 * @param t the tree
 * @return the parent node
 * @see ParentedTreeConcept
 */
inline tree::node_descriptor 
parent(tree::node_descriptor n, const tree& t)
{
  return t.parent(n);
}

/**
 * Add a node to a tree
 * @param parent the parent node
 * @param t the tree
 * @return the created node
 * @see BuildableTreeConcept
 */
inline tree::node_descriptor
add_node(tree::node_descriptor parent, tree& t)
{
  return t.add_node(parent);
}

/**
 * Tree traits for this tree.  Just inherits from tree.
 * @see tree_traits
 */
template<>
struct tree_traits<tree> : public tree {
};

} // namespace infovis

#endif // INFOVIS_TREE_TREE_HPP

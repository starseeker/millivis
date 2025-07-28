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
#include <infovis/tree/tree.hpp>
#include <infovis/tree/visitor.hpp>
#include <ostream>
#include <stdexcept>

namespace infovis {

tree::tree(unsigned capacity)
  : child_("#child", capacity),
    next_("#next", capacity),
    last_("#last", capacity),
    parent_("#parent", capacity)
{
  child_.add(nil());		// child of root is nil so far
  add_column(&child_);
  next_.add(nil());		// next of root is nil always
  add_column(&next_);
  last_.add(nil());		// last child of root is nil so fat
  add_column(&last_);
  parent_.add(nil());		// parent of root is always nil
  add_column(&parent_);
  reset();
}

tree::tree(const tree& other)
  : child_(other.child_),
    next_(other.next_),
    last_(other.last_),
    parent_(other.parent_)
{
  for (unsigned i = 0; i < other.column_count(); i++) {
    const column * c = other.get_column(i);
    if (c == &other.child_ ||
	c == &other.next_ ||
	c == &other.last_ ||
	c == &other.parent_)
      continue;
    add_column(c->clone());
  }
  reset();
}

void
tree::reset()
{
  resize(1);
  parent_.set(root, root);
  parent_.undefine(root);
  next_.set(root, root);
  next_.undefine(root);
  child_.set(root, root);
  child_.undefine(root);
  last_.set(root, root);
  last_.undefine(root);
}

tree::node_descriptor
tree::add_node(node_descriptor par)
{
  if (par >= num_nodes())
    throw std::out_of_range("tree::add_node");
  node_descriptor n = num_nodes();
  resize(n+1);
  parent_.set(n, par);
  next_.set(n, root);
  next_.undefine(n);		// will look undefine but still contains null
  if (last_[par] == root) {
    child_.set(par, n);
  }
  else {
    next_.set(last_[par], n);
  }
  last_.set(par, n);
  return n;
}

void
tree::clear()
{
  table::clear();
  child_.clear();
  child_.add(nil());		// child of root is nil so far
  add_column(&child_);
  next_.clear();
  next_.add(nil());		// next of root is nil always
  add_column(&next_);
  last_.clear();
  last_.add(nil());		// last child of root is nil so fat
  add_column(&last_);
  parent_.clear();
  parent_.add(nil());		// parent of root is always nil
  add_column(&parent_);
  
}

struct print_tree_visitor
{
  const tree& t;
  unsigned indent;
  std::ostream& out;

  print_tree_visitor(const tree& tree, std::ostream& o)
    : t(tree), indent(0), out(o)
  { }

  void tab(int i) {
    while (i-- != 0)
      out << ' ';
  }

  void preorder(tree::node_descriptor n) {
    tab(indent++);
    out << n << std::endl;
    for (tree::names_iterator name = t.begin_names();
	 name != t.end_names(); name++) {
      tab(indent);
      column * c = t.find_column(*name);
      if (c->defined(n)) {
	tab(indent);
	out << ": " << *name << " = " << c->get_value(n) << std::endl;
      }
    }    
  }
  void inorder(tree::node_descriptor n) {
  }
  void postorder(tree::node_descriptor n) {
    indent--;
  }
};

void
tree::print(std::ostream& out) const
{
  print_tree_visitor vis(*this, out);
  traverse_tree(nil(), *this, vis);
}


}

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
#include <infovis/tree/property_tree.hpp>
#include <infovis/tree/visitor.hpp>
#include <infovis/tree/algorithm.hpp>
#ifdef VECTOR_AS_TREE
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#else
#include <infovis/tree/dir_tree.hpp>
#include <infovis/tree/xml_tree.hpp>
#endif
#include <iostream>

using namespace infovis;

#ifdef VECTOR_AS_TREE
typedef property_tree Tree;
typedef Tree::prop_id prop_id;
#else
typedef tree Tree;
#endif
typedef tree_traits<Tree>::node_descriptor node_descriptor;


struct PrintVisitor
{
  const Tree& t;
  unsigned indent;

  PrintVisitor(const Tree& tree)
    : t(tree),
      indent(0)
  {}

  static void tab(int i) {
    while (i-- != 0)
      std::cout << ' ';
  }
  void preorder(node_descriptor n) {
    tab(indent++);
    std::cout << n << std::endl;
    for (Tree::names_iterator name = t.begin_names();
	 name != t.end_names(); name++) {
      tab(indent);
#ifdef VECTOR_AS_TREE
      std::cout << ": " << *name << " = ";
      Tree::prop_id id = t.get_prop_id(*name);
      if (id.is_numeric()) {
	std::cout << t.get_prop_numeric(id)[n] << std::endl;
      }
      else if (id.is_string()) {
	std::cout << t.get_prop_string(id)[n] << std::endl;
      }
#else
      column * c = t.find_column(*name);
      if (c->defined(n)) {
	tab(indent);
	std::cout << ": " << *name << " = " << c->get_value(n) << std::endl;
      }
#endif
      
    }
  }
  void inorder(node_descriptor n) {
  }
  void postorder(node_descriptor n) {
    indent--;
  }
};

#ifdef VECTOR_AS_TREE
struct properties_copier {
  const Tree& in_;
  Tree& out_;

  properties_copier(const Tree& in, Tree& out) : in_(in), out_(out) { }
  void operator()(node_descriptor in, node_descriptor out) {
    for (Tree::names_iterator i = in_.begin_names(); i != in_.end_names(); i++) {
      prop_id oid = out_.get_prop_id(*i);
      
    }
  }
  
};
#endif

struct prune_cvs {
  const property_tree::string_prop& name_;

  prune_cvs(const property_tree::string_prop& name)
    : name_(name) { }
  bool operator() (tree_traits<property_tree>::node_descriptor n) const {
    return ! (name_[n] == "CVS");
  }
};

int main(int argc, char * argv[])
{
  Tree t;

#ifdef VECTOR_AS_TREE
  if (argc == 1) {
    dir_property_tree(".", t);
  }
  else {
    if (xml_property_tree(argv[1], t) == 1) {
      dir_property_tree(argv[1], t);
    }
  }
#else
  if (argc == 1) {
    dir_tree(".", t);
  }
  else {
    if (xml_tree(argv[1], t) == 1) {
      dir_tree(argv[1], t);
    }
  }
#endif

  PrintVisitor vis(t);
  traverse_tree(root(t), t, vis);

  Tree tree2;
  copy_tree(t, tree2, null_copier());

  PrintVisitor vis2(tree2);
  traverse_tree(root(tree2), tree2, vis2);

#ifdef VECTOR_AS_TREE
  Tree tree3;
  copy_tree_prune_if(t, tree3, props_copier(t, tree3),
		     prune_cvs(t.get_prop_string("name")));
  PrintVisitor vis3(tree3);
  traverse_tree(root(tree3), tree3, vis3);
#endif
  std::cout << "Depth of tree1: " << depth(t) << std::endl;
  std::cout << "Depth of tree2: " << depth(tree2) << std::endl;
  //std::cout << "Depth of tree3: " << depth(tree3) << std::endl;

  return 0;
}

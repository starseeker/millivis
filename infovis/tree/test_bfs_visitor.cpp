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
#ifdef VECTOR_AS_TREE
#include <infovis/tree/property_tree.hpp>
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#else
#include <infovis/tree/tree.hpp>
#include <infovis/tree/dir_tree.hpp>
#include <infovis/tree/xml_tree.hpp>
#endif
#include <infovis/tree/bfs_visitor.hpp>
#include <iostream>

using namespace infovis;

#ifdef VECTOR_AS_TREE
typedef property_tree Tree;
typedef Tree::prop_id prop_id;
#else
typedef tree Tree;
typedef column * prop_id;
#endif
typedef tree_traits<Tree>::node_descriptor node_descriptor;


struct PrintBFSVisitor
{
  unsigned indent;

  PrintBFSVisitor()
    : indent(0)
  {}

  static void tab(int i) {
    while (i-- != 0)
      std::cout << ' ';
  }
  void initialize_node(node_descriptor n, const Tree& tree) { }
  bool discover_node(node_descriptor n, const Tree& tree) {
    return true;
  }
  void examine_node(node_descriptor n, const Tree& t) {
    indent++;
    tab(indent);
    std::cout << n << std::endl;
    for (Tree::names_iterator name = t.begin_names();
	 name != t.end_names(); name++) {
#ifdef VECTOR_AS_TREE
      tab(indent);
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
  void finish_node(node_descriptor n, const Tree& t) {
    indent--;
  }
};

int main(int argc, char * argv[])
{
  Tree tree;

#ifdef VECTOR_AS_TREE
  if (argc == 1) {
    dir_property_tree(".", tree);
  }
  else {
    if (xml_property_tree(argv[1], tree) == 1) {
      dir_property_tree(argv[1], tree);
    }
  }
#else
  if (argc == 1) {
    dir_tree(".", tree);
  }
  else {
    if (xml_tree(argv[1], tree) == 1) {
      dir_tree(argv[1], tree);
    }
  }
#endif

  PrintBFSVisitor vis;
  bfs_visit(root(tree), tree, vis);

  return 0;
}

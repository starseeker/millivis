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
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#include <iostream>

using namespace infovis;

typedef property_tree Tree;
typedef tree_traits<Tree>::node_descriptor node_descriptor;
typedef Tree::prop_id prop_id;

struct PrintVisitor
{
  const Tree& tree;
  unsigned indent;

  PrintVisitor(const Tree& t)
    : tree(t),
      indent(0)
  {}

  static void tab(int i) {
    while (i-- != 0)
      std::cout << ' ';
  }
  void preorder(node_descriptor n) {
    tab(indent++);
    for (Tree::names_iterator name = tree.begin_names();
	 name != tree.end_names(); name++) {
      tab(indent);
      std::cout << ": " << *name << " = ";
      Tree::prop_id id = tree.get_prop_id(*name);
      if (id.is_numeric()) {
	std::cout << tree.get_prop_numeric(id)[n] << std::endl;
      }
      else {
	std::cout << tree.get_prop_string(id)[n] << std::endl;
      }
    }
  }
  void inorder(node_descriptor n) {
  }
  void postorder(node_descriptor n) {
    indent--;
  }
};

int main(int argc, char * argv[])
{
  Tree tree;

  prop_id names = tree.add_property("name", property_tree::type_string);
  prop_id sizes = tree.add_property("size", property_tree::type_numeric);

  if (argc == 1) {
    dir_property_tree(".", tree);
  }
  else {
    if (xml_property_tree(argv[1], tree) == 1) {
      dir_property_tree(argv[1], tree);
    }
  }

  PrintVisitor vis(tree);
  traverse_tree(root(tree), tree, vis);

  return 0;
}

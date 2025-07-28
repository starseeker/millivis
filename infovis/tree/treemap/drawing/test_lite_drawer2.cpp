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
#include <infovis/tree/treemap/slice_and_dice.hpp>
#include <infovis/tree/treemap/squarified.hpp>
#include <infovis/tree/treemap/drawing/drawer.hpp>
#include <infovis/tree/treemap/drawing/gl_drawer.hpp>
#include <infovis/tree/treemap/drawing/pick_drawer.hpp>
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/gl_support.hpp>
//#define PRINT
#ifdef PRINT
#include <iostream>
#endif

using namespace infovis;

typedef property_tree Tree;
typedef Tree::numeric_prop WeightMap;
typedef tree_traits<Tree>::node_descriptor node_descriptor;

template <class Tree, class Box, class Color = color_drawer<Tree> >
struct lite_drawer : public null_drawer<Tree,Box>
{
  const Tree& tree;
  Color color;
  int x_off;
  int y_off;

  lite_drawer(const Tree& t, int dx = 5, int dy = 5)
    : tree(t), x_off(dx), y_off(dy) { }
  bool begin_box(const Box& b,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
    return ! (width(b) <= 0 || height(b) <= 0);
  }
  void draw_box(const Box& b,
		typename tree_traits<Tree>::node_descriptor n,
		unsigned depth) {
    color(n, depth);
    infovis::draw_box(b);
  }
  void draw_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    if (width(b) > 3 && height(b) > 3) {
      gl::color(0, 0, 0);
      gl::rect(xmin(b), ymax(b)-1, xmax(b), ymax(b));
      gl::rect(xmin(b), ymin(b), xmin(b)+1, ymax(b)-1);
      set_xmin(b,xmin(b)+1);
      set_ymax(b,ymax(b)-1);
    }
    if (degree(n, tree) > 0 &&
	width(b) > (x_off+3) && height(b) > (y_off+3)) {
      gl::color(1.0f, 1.0f, 1.0f);
      gl::rect(xmin(b), ymax(b)-y_off, xmax(b), ymax(b));
      gl::rect(xmin(b), ymin(b), xmin(b)+x_off, ymax(b)-y_off);
      set_xmin(b,xmin(b)+x_off);
      set_ymax(b,ymax(b)-y_off);
    }
  }
  void end_box(const Box& b, 
	       typename tree_traits<Tree>::node_descriptor n,
	       unsigned depth) { }
};


class LiteTreemap : public LiteBounded
{
public:
  LiteTreemap(const Box& b,
	      Tree& t, Tree::prop_id w)
    : LiteBounded(b),
      tree(t), weight_id(w), 
      treemap(t,
	      t.get_prop_numeric(w),
	      lite_drawer<Tree,Box>(t))
  {
    validate();
  }

  void doRender(const RenderContext& c) {
    if (! c.is_picking)
      treemap.visit(bounds, root(tree));
    else
      draw_box(bounds);
  }
  Lite * clone() const {
    return new LiteTreemap(bounds,
			   tree, weight_id);
  }
  
protected:
  void validate() { }

  Tree& tree;
  Tree::prop_id weight_id;
  treemap_squarified<
    Tree,Box,Tree::numeric_prop,lite_drawer<Tree,Box>
  > treemap;
};

struct compare_weight {
  const WeightMap& weight;

  compare_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] < weight[n2];
  }
};

#ifdef PRINT
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
#endif

int main(int argc, char * argv[])
{
  LiteWindow::init(argc, argv);

  Tree tree;

  Tree::prop_id names = tree.add_property("name", property_tree::type_string);
  Tree::prop_id sizes = tree.add_property("size", property_tree::type_numeric);

  if (argc == 1) {
    dir_property_tree(".", tree);
  }
  else {
    if (xml_property_tree(argv[1], tree) == 1) {
      dir_property_tree(argv[1], tree);
    }
  }
  string prop;
  Tree::prop_id weight;
  for (Tree::names_iterator n = tree.begin_names();
       n != tree.end_names(); n++) {
    prop = *n;
    weight = tree.get_prop_id(prop);
    if (weight.is_numeric())
      break;
  }
  if (! weight.is_numeric()) {
    exit(1);
  }
  WeightMap& wm = tree.get_prop_numeric(weight);
  sum_weights(tree, wm);

#ifdef PRINT
  PrintVisitor vis(tree);
  traverse_tree(root(tree), tree, vis);
#endif
  sort(tree, compare_weight(wm));
#ifdef PRINT
  traverse_tree(root(tree), tree, vis);
#endif
  LiteWindow win("test_lite_drawer", Box(0, 0, 800, 600));

  LiteTreemap lt(Box(0, 0, 800, 600),
		 tree, weight);
  win.addChild(&lt);

  win.run();
  return 0;
}

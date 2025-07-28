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
#include <infovis/tree/treemap/drawing/gl_drawer.hpp>
#include <infovis/tree/treemap/drawing/debug_drawer.hpp>
#include <infovis/tree/treemap/drawing/pick_drawer.hpp>
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <geometric.hpp>
#include <strips.hpp>
#include <infovis/drawing/direction_io.hpp>
#include <infovis/drawing/position_io.hpp>
#include <iostream>

#define STRIPS
//#define PRINT

using namespace infovis;

typedef property_tree Tree;
typedef Tree::numeric_prop WeightMap;
typedef tree_traits<Tree>::node_descriptor node_descriptor;
typedef tree_traits<Tree>::node_iterator node_iterator;

typedef debug_drawer<Tree, Box, gl_drawer<Tree,Box> > Drawer;

typedef strips<Tree> Strips;
typedef strips_drawer<Tree,Box> StripsDrawer;

static void
print_strips(const Tree& t, const Strips& s)
{
  Strips::const_iterator i;
  for (i = s.begin(); i != s.end(); i++) {
    std::cout << "[" << i->first << "]\n";
    const Strips::striplist& sl = i->second;
    for (Strips::striplist::const_iterator j = sl.begin();
	 j != sl.end(); j++) {
      std::cout << "\t" << j->direction_
		<< " "  << j->position_;
      for (std::vector<node_descriptor>::const_iterator k = j->begin();
	   k != j->end(); k++) {
	std::cout << " " << *k;
      }
      std::cout << std::endl;
    }
  }
}

static void 
make_strips(const Box& b,
	    const Tree& t, Tree::prop_id w, Tree::prop_id sw)
{
  StripsDrawer drawer(t);
  treemap_squarified<
    Tree,Box,Tree::numeric_prop,Tree::numeric_prop, StripsDrawer&
    > treemap(t,
	      t.get_prop_numeric(w),
	      t.get_prop_numeric(sw),
	      drawer);
  treemap.start();
  treemap.visit(b,root(t));
  treemap.finish();
  print_strips(t,drawer);
}

typedef treemap_geometric<
    Tree,Box,Tree::numeric_prop, Drawer
  > tree_map;

class LiteTreemap : public LiteBounded
{
public:
  LiteTreemap(const Box& b,
	      Tree& t, Tree::prop_id w)
    : LiteBounded(b),
      tree_(t), weight_id_(w),
#if 1
      drawer_(t),
      treemap_(t,
	       t.get_prop_numeric(w),
	       drawer_)
#else
    treemap_(t,
	     t.get_prop_numeric(w))
#endif
  {
    treemap_.start();
    treemap_.visit(bounds, root(tree_));
    treemap_.finish();
  }

  void doRender(const RenderContext& c) {
    if (! c.is_picking) {
      treemap_strips<
	Tree,
	Box,
	Tree::numeric_prop,Tree::numeric_prop,
	Drawer
	> treemap(tree_,
		  drawer_,
		  tree_.get_prop_numeric(weight_id_));
      treemap.start();
      treemap.visit(bounds, root(tree_));
      treemap.finish();
    }
    else
      draw_box(bounds);
  }
  Lite * clone() const {
    return new LiteTreemap(bounds,
			   tree_, weight_id_);
  }
  
protected:
  Tree& tree_;
  Tree::prop_id weight_id_;
#if 1
  StripsDrawer drawer_;
  treemap_squarified<
    Tree,Box,Tree::numeric_prop,StripsDrawer&
    > treemap_;
#else
  treemap_squarified<
    Tree,Box,Tree::numeric_prop,Drawer
  > treemap_;
#endif
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
  LiteWindow win("test_geometric", Box(0, 0, 800, 600));

  LiteTreemap lt(Box(0, 0, 800, 600),
		 tree, weight);
#ifdef STRIPS
  make_strips(Box(0, 0, 800, 600),
	      tree, weight);
#endif
  win.addChild(&lt);

  win.run();
  return 0;
}


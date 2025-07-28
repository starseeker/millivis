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
#include <infovis/tree/treemap/slice_and_dice.hpp>
#include <infovis/tree/treemap/squarified.hpp>
#include <boost/directory.h>
#include <infovis/drawing/direction_io.hpp>
#include <infovis/drawing/box_io.hpp>
#include <iostream>

using namespace infovis;

typedef FloatColumn WeightMap;

static void build_tree(tree_traits< vector_as_tree >::node_descriptor parent,
		       vector_as_tree& tree, const std::string& dirname,
		       std::vector<std::string,gc_alloc<std::string> >& names,
		       WeightMap& wm)
{
  using namespace infovis::filesystem;
  //std::cout << "entering " << dirname << std::endl;
  for (dir_it it(dirname); it != dir_it(); ++it) {
    if ((*it)[0] == '.')
      continue;
    tree_traits< vector_as_tree >::node_descriptor n = add_node(parent, tree);
    names.push_back(*it);
    wm.add(boost::filesystem::get<size>(it));
    if (boost::filesystem::get<is_directory>(it))
      build_tree(n, tree, dirname+"/"+(*it), names, wm);
  }
  //std::cout << "leaving " << dirname << std::endl;
}

static inline void
tab(int depth) { while(depth--) std::cout << ' '; }

template <class Tree, class Box>
struct print_drawer : public null_drawer<Tree,Box> {
  void begin_strip(const Box& b,
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth,
		   direction dir) {
    tab(depth);
    std::cout << "begin_strip " << b << ", dir=" << dir << std::endl;
  }
  void end_strip(const Box& b,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth,
		 direction dir) {
    tab(depth);
    std::cout << "end_strip " << b << ", dir=" << dir << std::endl;
  }

#if 0
  bool begin_box(const Box& b,
		 typename tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
    tab(depth);
    std::cout << "begin_" << b << std::endl;
    return true;
  }
#endif
  void draw_box(const Box& b,
		typename tree_traits<Tree>::node_descriptor n,
		unsigned depth) {
    tab(depth);
    std::cout << "node " <<  n << " draw_" << b << std::endl;
  }
#if 0
  void draw_border(Box& b, 
		   typename tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    tab(depth);
    std::cout << "border_" << b << std::endl;
  }
  void end_box(const Box& b, 
	       typename tree_traits<Tree>::node_descriptor n,
	       unsigned depth) {
    tab(depth);
    std::cout << "end_" << b << std::endl;
  }
#endif
};

int main(int argc, char * argv[])
{
  typedef box_min_max<float> Box;
  typedef vector_as_tree Tree;
  std::vector<std::string,gc_alloc<std::string> > names;
  WeightMap * wm;

  Tree t;

  if (argc == 1) {
    names.push_back(".");
    build_tree(root(t), t, ".", names, wm);
  }
  else {
    names.push_back(argv[1]);
    build_tree(root(t), t, argv[1], names, wm);
  }
  
  sum_weights(t, wm);
  PrintVisitor vis(names, wm);
  traverse_tree(root(t), t, vis);

  treemap_squarified<
    Tree,
    Box,
    WeightMap&,
    print_drawer<Tree,Box>
    > tms(t, wm);
  tms.start();
  tms.visit(Box(0, 0, 1024,1024), root(t));
  tms.finish();
  return 0;
}

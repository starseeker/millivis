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
#include <infovis/tree/vector_as_tree.hpp>
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#else
#include <infovis/tree/tree.hpp>
#include <infovis/tree/dir_tree.hpp>
#include <infovis/tree/xml_tree.hpp>
#endif
#include <infovis/tree/sum_weight_visitor.hpp>
#include <iostream>

using namespace infovis;

int main(int argc, char * argv[])
{
  int i;
#ifdef VECTOR_AS_TREE
  std::vector<float, gc_alloc<float,true> > wm;
  std::vector<float, gc_alloc<float,true> > swm;
  vector_as_tree t;

  if (argc == 1) {
    dir_property_tree(".", tree);
  }
  else {
    if (xml_property_tree(argv[1], tree) == 1) {
      dir_property_tree(argv[1], tree);
    }
  }
  
  //sum_weight_visitor<vector_as_tree,std::vector<float,gc_alloc<float,true> > > vis(t, wm, swm);
#else
  tree t;

  if (argc == 1) {
    dir_tree(".", t);
  }
  else {
    if (xml_tree(argv[1], t) == 1) {
      dir_tree(argv[1], t);
    }
  }

  FloatColumn * wm = 0;
  for (i = 0; i < t.column_count(); i++) {
    wm = FloatColumn::cast(t.get_column(i));
    if (wm != 0)
      break;
  }
  if (wm == 0) {
    std::cerr << "Cannot find a float column\n";
    return 1;
  }
#endif

  //traverse_tree(root(t), t, vis);
  sum_weights(t, *wm);
  //swm = sum_weights(tree, wm);
  for (i = 0; i < wm->size(); i++) {
    std::cout << "sum[" << i << "] = " << wm->get_value(i) << std::endl;
  }
  
  return 0;
}

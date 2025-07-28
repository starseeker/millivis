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
#ifndef TREEMAP2_TYPES_HPP
#define TREEMAP2_TYPES_HPP

#include <infovis/drawing/lite/Lite.hpp>
#include <infovis/tree/ObservableTree.hpp>
//#include <infovis/table/filter.hpp>
#include <vector>

namespace infovis {

//typedef tree Tree;
typedef ObservableTree Tree;

typedef FloatColumn WeightMap;
typedef UnsignedColumn FilterColumn;

inline unsigned
node_depth(unsigned n, const Tree& t)
{
  unsigned depth = 0;
  if (n == root(t))
    return 0;
  while (n != root(t)) {
    depth++;
    n = parent(n, t);
  }
  return depth;
}

typedef tree_traits<Tree>::node_descriptor node_descriptor;
typedef Lite::Path Path;
typedef std::vector<Box> PathBoxes;

inline string
subprop(const string& sub, const string& name)
{
  return name + "_" + sub;
}

inline bool
has_subprop(const string& sub, const string& name)
{
  int i;
  if (name.length() < (sub.length()+1))
    return false;
  for (i = 0; i < sub.length(); i++) {
    if (sub[sub.length()-1-i] != name[name.length()-1-i])
      return 0;
  }
  return name[name.length()-1-i] == '_';
}

} // namespace infovis

#endif // TREEMAP2_TYPES_HPP

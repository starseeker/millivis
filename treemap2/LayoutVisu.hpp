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
#ifndef TREEMAP2_LAYOUTVISU_HPP
#define TREEMAP2_LAYOUTVISU_HPP

#include <LiteTreemap.hpp>
#include <infovis/alloc.hpp>
#include <infovis/tree/treemap/drawing/weight_interpolator.hpp>

namespace infovis {

struct Filter {
  const FilterColumn& filter_;

  Filter(const FilterColumn& filter)
    : filter_(filter)
  { }

  Filter(const Filter& other)
    : filter_(other.filter_)
  { }

  bool operator()(node_descriptor n) const {
    return filter_[n];
  }
};

class LayoutVisu
{
public:
  LayoutVisu(LiteTreemap * tm) : tm_(tm) { }
  virtual ~LayoutVisu();

  virtual unsigned draw(float param) = 0;
  virtual unsigned pick(float param) = 0;
  virtual void boxlist(float param,
		       AnimateTree::BoxList& bl, int depth) = 0;

  static LayoutVisu * create_visu(LiteTreemap::Layout l, LiteTreemap *);
protected:
  LiteTreemap * tm_;
};



} // namespace infovis

#endif // TREEMAP2_LAYOUTVISU_HPP

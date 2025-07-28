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
#ifndef TREEMAP2_LAYOUTVISUSP_HPP
#define TREEMAP2_LAYOUTVISUSP_HPP

#include <LiteTreemap.hpp>
#include <LayoutVisu.hpp>
#include <BoxDrawer.hpp>

namespace infovis {

class LayoutVisuScatterPlot : public LayoutVisu
{
public:
  LayoutVisuScatterPlot(LiteTreemap * tm);
  virtual unsigned draw(float param);
  virtual unsigned pick(float param);
  virtual void boxlist(float param, AnimateTree::BoxList& bl, int depth);
  void renderFastScatterPlot(float param);
protected:
  const FloatColumn * cached_x_axis_;
  const FloatColumn * cached_y_axis_;
  const FloatColumn * cached_color_axis_;
  const FloatColumn * cached_weight_;
  float cached_float_range_;
  float * cached_vetices_;
  int cached_vetices_size_;
};

} // namespace infovis

#endif // TREEMAP2_LAYOUTVISUSP_HPP

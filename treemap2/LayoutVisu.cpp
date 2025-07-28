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

#include <LiteTreemap.hpp>
#include <BoxDrawer.hpp>
#include <LayoutVisu.hpp>
#include <LayoutVisuSQ.hpp>
#include <LayoutVisuSD.hpp>
#include <LayoutVisuSP.hpp>

namespace infovis {

LayoutVisu::~LayoutVisu()
{ }

static LayoutVisuSquarified layout_squarified(0);
static LayoutVisuSliceAndDice layout_slice_and_dice(0);
static LayoutVisuScatterPlot layout_scatter_plot(0);

LayoutVisu *
LayoutVisu::create_visu(LiteTreemap::Layout l, LiteTreemap * tm)
{
  switch(l) {
  case LiteTreemap::layout_squarified:
  case LiteTreemap::layout_strip:
    layout_squarified.tm_ =  tm;
    return &layout_squarified;
  case LiteTreemap::layout_slice_and_dice:
    layout_slice_and_dice.tm_ =  tm;
    return &layout_slice_and_dice;
  case LiteTreemap::layout_scatter_plot:
    layout_scatter_plot.tm_ =  tm;
    return &layout_scatter_plot;
  default:
    break;
  }
  return 0;
}

};

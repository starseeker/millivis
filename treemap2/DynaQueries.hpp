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
#ifndef TREEMAP_DYNAQUERIES_HPP
#define TREEMAP_DYNAQUERIES_HPP

#include <infovis/drawing/lite/LayoutTable.hpp>
#include <infovis/drawing/lite/LiteBackground.hpp>
#include <infovis/drawing/lite/LiteBox.hpp>
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/lite/LiteRangeSlider.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/notifiers/BeginEnd.hpp>
#include <infovis/drawing/notifiers/Change.hpp>
#include <infovis/drawing/notifiers/BoundedRange.hpp>

#include <types.hpp>
#include <LiteTreemap.hpp>

namespace infovis {

class DynamicQueries : public LiteBox,
		       public BoundedRangeObserver,
		       public BeginEndObserver
{
protected:
  LiteTreemap * tm_;
  const Tree& tree_;
  std::vector<LiteRangeSlider*> slider_;
  std::vector<const column*> column_;
  Font * label_font_;
  LiteBackground background_;
  FilterColumn * filter_;
public:
  DynamicQueries(LiteTreemap * tm, FilterColumn * col, Font * label_font);

  Lite * clone() const;

  void create_numeric(const FloatColumn * col);
  void create_string(const StringColumn * col);
  void filter(int i, const BoundedRange * range);
  void applyFilters();

  // BoundedRangeObserver
  virtual void valueDragged(BoundedRangeObservable * obs);  
  virtual void rangeDragged(BoundedRangeObservable * obs);  
  virtual void updateBoundedRange(BoundedRangeObservable * obs);

  // BeginEndObserver
  virtual void begin(BeginEndObservable *);
  virtual void end(BeginEndObservable *);

  virtual bool isVisible() const;
  virtual void setVisible(bool);

  LiteBackground * getBackground() { return &background_; }
};

} // namespace infovis

#endif // TREEMAP_DYNAQUERIES_HPP

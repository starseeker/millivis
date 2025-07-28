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
#include <DynaQueries.hpp>
#include <infovis/drawing/lite/LiteRangeSlider.hpp>
#include <infovis/drawing/lite/LiteMover.hpp>
#include <infovis/drawing/lite/LiteRect.hpp>
#include <iostream>

namespace infovis {

static LiteLabel *
create_label(const string& label, Font * font)
{
  return new LiteLabel(label, LiteLabel::just_left, font);
}

DynamicQueries::DynamicQueries(LiteTreemap * tm,
			       FilterColumn * col,
			       Font * label_font)
  : LiteBox(new LayoutTable(2)),
    tm_(tm),
    tree_(tm->tree_),
    label_font_(label_font),
    background_(this, color_white),
    filter_(col)
{
  for (Tree::names_iterator n = tree_.begin_names();
       n != tree_.end_names(); n++) {
    const string& prop = *n;
    const column * col = tree_.find_column(prop);
    if (prop[0] == '$') // skip local columns
      continue;
    const FloatColumn * flt = FloatColumn::cast(col);
    if (flt != nullptr) {
      create_numeric(flt);
    }
    else {
      const StringColumn* str = StringColumn::cast(col);
      if (str != nullptr) {
        create_string(str);
      }
    }
  }
  addChild(new LiteMover(new LiteRect(Box(0, 0, 100, 20), color_half_grey),
			 this));
  addChild(new LiteMover(new LiteRect(Box(100, 0, 200, 20), color_half_grey),
			 this));
}

Lite *
DynamicQueries::clone() const
{
  return new DynamicQueries(tm_, filter_, label_font_);
}

void
DynamicQueries::create_numeric(const FloatColumn * col)
{
  DefaultBoundedRangeObservable * bounds =
    new DefaultBoundedRangeObservable(col->min(),
				      col->max(),
				      col->min(),
				      (col->get_name() == "degree") ?
				      0 : col->max() - col->min());
  LiteRangeSlider * slider = new LiteRangeSlider(bounds,
						 Box(100, 0, 200, 10),
						 left_to_right);
  addChild(create_label(col->get_name()+":", label_font_));
  addChild(slider);
  slider_.push_back(slider);
  slider->addBeginEndObserver(this);
  slider->addBeginEndObserver(tm_);
  bounds->addBoundedRangeObserver(this);
  column_.push_back(col);
}

void
DynamicQueries::create_string(const StringColumn * col)
{
  DefaultBoundedRangeObservable * bounds =
    new DefaultBoundedRangeObservable(0, col->size(),
				      0, col->size());  
  LiteRangeSlider * slider = new LiteRangeSlider(Box(100, 0, 200, 10),
						 left_to_right);
  addChild(create_label(col->get_name()+":", label_font_));
  addChild(slider);
  slider_.push_back(slider);
  slider->addBeginEndObserver(this);
  slider->addBeginEndObserver(tm_);
  bounds->addBoundedRangeObserver(this);
  column_.push_back(col);
}

inline bool
in_range(float val, float min, float max)
{
  return val >= min && val <= max;
}

void
DynamicQueries::filter(int index, const BoundedRange * range)
{
  const FloatColumn * col = FloatColumn::cast(column_[index]);

  if (col == nullptr)
    return;
  int time = LiteWindow::time();

  float min = range->value();
  float max = min + range->range();
//   if (min <= range->min() && max >= range->max())	\
//     return;

  std::cerr << "Filtering " + col->get_name() +
    " in [" << min << ", " << max << "]\n";

  // special case for depth change
  if (col->get_name() == "depth") {
    tm_->getDrawer().set_max_depth(unsigned(max+0.5f));
  }
  unsigned filtered = 0;

  for (int i = 0; i < col->size(); i++) {
    if (filter_->fast_get(i)) {
      continue;			// already filtered out
    }
    else if (! col->defined(i) ||
	     ! in_range(col->fast_get(i), min , max)) {
      (*filter_)[i] = true;
      filtered++;
    }
    else {
      (*filter_)[i] = false;
    }
  }
#ifdef PRINT
  if (filtered != 0)
    std::cerr << "Filtered " << filtered << " items\n";
#endif
  std::cerr << "Time: "
	    << float(time / 1000.0f) << "s, "
	    << float(col->size() * 1000.0f / time) << "items/s\n";
    
}

void
DynamicQueries::applyFilters()
{
  int i;
  for (i = 0; i < filter_->size(); i++)
    filter_->fast_set(i, false);
  
  for (i = 0; i < slider_.size(); i++) {
    filter(i, slider_[i]->getObservable()->getBoundedRange());
  }
}

void
DynamicQueries::valueDragged(BoundedRangeObservable * obs)
{
  int i;
  for (i = 0; i < slider_.size(); i++) {
    if (slider_[i] == obs) {
      tm_->updateMinMax();
      return;
    }
  }
}

void
DynamicQueries::rangeDragged(BoundedRangeObservable * obs)
{
  valueDragged(obs);
}
  

void
DynamicQueries::updateBoundedRange(BoundedRangeObservable * obs)
{
}

// BeginEndObserver
void
DynamicQueries::begin(BeginEndObservable * observable)
{
  int i;
  for (i = 0; i < slider_.size(); i++) {
    if (slider_[i] != observable) {
      getChild(2*i)->setVisible(false);
      getChild(2*i+1)->setVisible(false);
    }
    else {
      slider_[i]->setFont(label_font_);
    }
  }
  getChild(2*slider_.size())->setVisible(false);
  getChild(2*slider_.size()+1)->setVisible(false);
  background_.setColor(color_none);
}

void
DynamicQueries::end(BeginEndObservable * observable)
{
  for (int i = 0; i < slider_.size(); i++) {
    getChild(2*i)->setVisible(true);
    getChild(2*i+1)->setVisible(true);
    if (slider_[i] == observable) {
      slider_[i]->setFont(0);
    }
  }
  getChild(2*slider_.size())->setVisible(true);
  getChild(2*slider_.size()+1)->setVisible(true);
  background_.setColor(color_white);
}

bool
DynamicQueries::isVisible() const
{
  return background_.isVisible();
}

void
DynamicQueries::setVisible(bool s)
{
  background_.setVisible(s);
}


} // namespace infovis

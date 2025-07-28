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
#include <infovis/drawing/lite/LiteThumb.hpp>

namespace infovis {

LiteThumb::LiteThumb(LiteShape * shape,
		     LiteSlider * slider,
		     LiteSlider::Zone zone)
  : LiteEnterLeaveShape(shape, color_red, color_black),
    shape_(shape),
    slider_(slider),
    zone_(zone)
{ }

Lite *
LiteThumb::clone() const
{
  return new LiteThumb(shape_, slider_, zone_);
}

Interactor *
LiteThumb::interactor(const string& name, int tool_id)
{
  if (Interactor3States::isA(name))
    return (Interactor3States*)this;
  if (LiteEnterLeaveShape::isA(name))
    return (LiteEnterLeaveShape*)this;
  return 0;
}

bool
LiteThumb::doStart(const Event& ev)
{
  slider_->notifyBegin();
  BoundedRangeObservable * obs = slider_->getObservable();
  saved_ = obs->getBoundedRange()->value();
  saved_range_ = obs->getBoundedRange()->range();
  doDrag(ev);
  return true;
}

void
LiteThumb::doDrag(const Event& ev)
{
  BoundedRangeObservable * obs = slider_->getObservable();
  const BoundedRange * br = obs->getBoundedRange();
  float val, range;
  Point pos(getFieldPosition(ev));

  switch(zone_) {
  case LiteSlider::zone_min:
    val = slider_->getValue(pos, zone_);
    obs->notifyValueDragged(val);
    break;
  case LiteSlider::zone_thumb:
    val = slider_->getValue(pos, zone_);
    range = br->range();
    obs->notifyValueRangeDragged(val, range);
    break;
  case LiteSlider::zone_max:
    val = slider_->getValue(pos, zone_);
    range = val - br->value();
    obs->notifyRangeDragged(range);
    break;
  }
  repaint();
}

void
LiteThumb::doFinish(const Event& ev)
{
  doDrag(ev);
  slider_->notifyEnd();
  slider_->getObservable()->notifyBoundedRange();
}

void
LiteThumb::doAbort(const Event&)
{
  BoundedRangeObservable * obs = slider_->getObservable();
  obs->notifyValueDragged(saved_);
  obs->notifyRangeDragged(saved_range_);
  obs->notifyBoundedRange();
  slider_->notifyEnd();  
}


} // namespace infovis

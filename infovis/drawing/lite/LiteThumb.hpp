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
#ifndef INFOVIS_DRAWING_LITE_THUMB_HPP
#define INFOVIS_DRAWING_LITE_THUMB_HPP

#include <infovis/drawing/lite/LiteEnterLeaveShape.hpp>
#include <infovis/drawing/lite/LiteSlider.hpp>
#include <infovis/drawing/inter/Interactor3States.hpp>

namespace infovis {

class LiteShape;

/**
 * Lite that implements the thumb of a slider
 */
class LiteThumb : public LiteEnterLeaveShape,
		  public Interactor3States
{
public:
  LiteThumb(LiteShape * shape, LiteSlider * slider, LiteSlider::Zone zone);

  virtual Lite * clone() const;
  virtual Interactor * interactor(const string& name, int tool_id);

  LiteSlider::Zone getZone() const { return zone_; }
  void setZone(LiteSlider::Zone z) { zone_ = z; }
  LiteSlider * getSlider() const { return slider_; }

  // Interactor3States
  virtual bool doStart(const Event& ev);
  virtual void doDrag(const Event& ev);
  virtual void doFinish(const Event& ev);
  virtual void doAbort(const Event& ev);
public:
  LiteShape * shape_;
  LiteSlider * slider_;
  LiteSlider::Zone zone_;
  float saved_;
  float saved_range_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_THUMB_HPP

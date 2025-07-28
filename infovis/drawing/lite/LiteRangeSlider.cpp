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
#include <infovis/drawing/lite/LiteRangeSlider.hpp>
#include <infovis/drawing/lite/LiteShape.hpp>
#include <infovis/drawing/lite/LiteThumb.hpp>
#include <infovis/drawing/Font.hpp>

#include <infovis/drawing/gl_support.hpp>

//#define PRINT
#ifdef PRINT
#include <infovis/drawing/point_vec_io.hpp>
#include <iostream>
#endif

namespace infovis {

class LiteTriangle : public LiteShape
{
public:
  LiteTriangle(direction dir, const Box& b, const Color& c)
    : LiteShape(b, c),
      dir_(dir)
  { }

  Lite * clone() const {
    return new LiteTriangle(dir_, getBounds(), getColor());
  }
  
  void doRender(const RenderContext& rc) {
    LiteShape::doRender(rc);
    glBegin(GL_TRIANGLES);
    switch(dir_) {
    case left_to_right:
      gl::vertex(xmin(bounds), ymax(bounds));
      gl::vertex(xmin(bounds), ymin(bounds));
      gl::vertex(xmax(bounds), (ymin(bounds)+ymax(bounds))/2);
      break;
    case right_to_left:
      gl::vertex(xmax(bounds), ymin(bounds));
      gl::vertex(xmax(bounds), ymax(bounds));
      gl::vertex(xmin(bounds), (ymin(bounds)+ymax(bounds))/2);
      break;
    case top_to_bottom:
      gl::vertex(xmax(bounds), ymax(bounds));
      gl::vertex(xmin(bounds), ymax(bounds));
      gl::vertex((xmin(bounds)+xmax(bounds))/2, ymin(bounds));
      break;
    case bottom_to_top:
      gl::vertex(xmax(bounds), ymin(bounds));
      gl::vertex(xmin(bounds), ymin(bounds));
      gl::vertex((xmin(bounds)+xmax(bounds))/2, ymax(bounds));
      break;
    }
    glEnd();
  }
protected:
  direction dir_;
};

LiteRangeSlider::LiteRangeSlider(BoundedRangeObservable * observable,
				 const Box& bounds,
				 direction dir,
				 const Color& fg,
				 const Color& bg,
				 Font * font,
				 float minmax_size)
  : LiteSlider(observable, bounds, dir, fg, bg, font),
    min_size_(minmax_size),
    max_size_(minmax_size),
    saved_range_(0)
{
  addChild(new LiteThumb(new LiteTriangle(turn180(dir),
					  bounds,
					  color_black),
			 this,
			 zone_min));
  addChild(new LiteThumb(new LiteTriangle(dir,
					  bounds,
					  color_black),
			 this,
			 zone_max));
}

LiteRangeSlider::LiteRangeSlider(const Box& bounds,
				 direction dir,
				 const Color& fg,
				 const Color& bg,
				 Font * font,
				 float minmax_size)
  : LiteSlider(bounds, dir, fg, bg, font),
    min_size_(minmax_size),
    max_size_(minmax_size),
    saved_range_(0)
{
  addChild(new LiteThumb(new LiteTriangle(turn180(dir),
					  bounds,
					  color_black),
			 this,
			 zone_min));
  addChild(new LiteThumb(new LiteTriangle(dir,
					  bounds,
					  color_black),
			 this,
			 zone_max));
}

Lite *
LiteRangeSlider::clone() const
{
  return new LiteRangeSlider(getObservable(),
			     getBounds(),
			     getDirection(),
			     getForegroundColor(),
			     getBackgroundColor(),
			     getFont(),
			     min_size_);
}


float
LiteRangeSlider::getMinSize() const { return min_size_; }

void
LiteRangeSlider::setMinSize(float v) { min_size_ = v; }

float
LiteRangeSlider::getMaxSize() const { return max_size_; }

void
LiteRangeSlider::setMaxSize(float v) { max_size_ = v; }

string
LiteRangeSlider::maxLabel()
{
  return formatLabel(
	    observable_->getBoundedRange()->value() +
	    observable_->getBoundedRange()->range());
}

#if 0
void
LiteRangeSlider::renderMin(const Box& b)
{
  set_color(foreground_color_);
  draw_triangle(b, turn180(direction_));
}

void
LiteRangeSlider::renderMax(const Box& b)
{
  set_color(foreground_color_);
  draw_triangle(b, direction_);
  if (font_ != 0) {
    char buffer[100];
    sprintf(buffer, "%.02g",
	    observable_->getBoundedRange()->value()+
	    observable_->getBoundedRange()->range());
    set_color(color_white);
    font_->paintGrown(buffer, xmin(b), ymin(b)+font_->getDescent());
    set_color(color_black);
    font_->paint(buffer, xmin(b), ymin(b)+font_->getDescent());
  }
}

bool
LiteRangeSlider::doStart(const Event& ev)
{
  Point pos(getFieldPosition(ev));
  notifyBegin();
  zone_dragged_ = getZone(pos);
  if (zone_dragged_ == zone_none)
    zone_dragged_ = zone_thumb;
  BoundedRangeObservable * obs = getObservable();
  saved_ = obs->getBoundedRange()->value();
  saved_range_ = obs->getBoundedRange()->range();
  doDrag(ev);
  return true;
}

void
LiteRangeSlider::doDrag(const Event& ev)
{
  BoundedRangeObservable * obs = getObservable();
  const BoundedRange * br = obs->getBoundedRange();
  float val, range;
  Point pos(getFieldPosition(ev));

  switch(zone_dragged_) {
  case zone_min:
    val = getValue(pos, zone_min);
    obs->notifyValueDragged(val);
    break;
  case zone_thumb:
    val = getValue(pos, zone_thumb);
    range = br->range();
    obs->notifyValueRangeDragged(val, range);
    break;
  case zone_max:
    val = getValue(pos, zone_max);
    range = val - br->value();
    obs->notifyRangeDragged(range);
    break;
  }
  repaint();
}

void
LiteRangeSlider::doFinish(const Event& ev)
{
  doDrag(ev);
  notifyEnd();
  zone_dragged_ = zone_none;
  getObservable()->notifyBoundedRange();
}

void
LiteRangeSlider::doAbort(const Event& ev)
{
  getObservable()->notifyRangeDragged(saved_range_);
  LiteSlider::doAbort(ev);
}
#endif

}

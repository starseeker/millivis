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
#include <infovis/drawing/lite/LiteRect.hpp>
#include <infovis/drawing/lite/LiteSlider.hpp>
#include <infovis/drawing/lite/LiteThumb.hpp>
#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <cstdio>

//#define PRINT
#ifdef PRINT
#include <infovis/drawing/point_vec_io.hpp>
#include <iostream>
#endif

namespace infovis {

LiteSlider::LabelFormater::~LabelFormater()
{
}

LiteSlider::LiteSlider(BoundedRangeObservable * observable,
		       const Box& bounds,
		       direction dir,
		       const Color& fg,
		       const Color& bg,
		       Font * font)
  : LiteGroup(bounds),
    observable_(observable),
    direction_(dir),
    foreground_color_(fg),
    background_color_(bg),
    font_(font),
    thumb_size_(10),
    saved_(0),
    formater_(0)
{
  observable_->addBoundedRangeObserver(this);
  addChild(new LiteThumb(new LiteRect(Box(min(bounds),
					  Vector(thumb_size_, thumb_size_)),
				      color_black),
			 this,
			 zone_thumb));
}

LiteSlider::LiteSlider(const Box& bounds,
		       direction dir,
		       const Color& fg,
		       const Color& bg,
		       Font * font)
  : LiteGroup(bounds),
    observable_(new DefaultBoundedRangeObservable(0, 100, 0)),
    direction_(dir),
    foreground_color_(fg),
    background_color_(bg),
    font_(font),
    thumb_size_(10),
    saved_(0),
    formater_(0)
{
  observable_->addBoundedRangeObserver(this);
  addChild(new LiteThumb(new LiteRect(Box(min(bounds),
					  Vector(thumb_size_, thumb_size_)),
				      color_black),
			 this,
			 zone_thumb));
}

Lite *
LiteSlider::clone() const
{
  return new LiteSlider(getObservable(),
			getBounds(),
			getDirection(),
			getForegroundColor(),
			getBackgroundColor(),
			getFont());
}

float
LiteSlider::getMinSize() const { return 0; }

void
LiteSlider::setMinSize(float) { }

float
LiteSlider::getThumbSize() const { return thumb_size_; }

void
LiteSlider::setThumbSize(float v) {thumb_size_ = v; }

float
LiteSlider::getMaxSize() const { return 0; }

void
LiteSlider::setMaxSize(float) { }

void
LiteSlider::computeZones(float length, float zones[zone_last]) const
{
  const BoundedRange * range = observable_->getBoundedRange();
  float draw_val, draw_range;
  float avail_length = length - getThumbSize() - getMinSize() - getMaxSize();
  if (range->max() == range->min()) {
    draw_val = 0;
    draw_range = 0;
  }
  else {
    draw_val =
      avail_length *
      (range->value() - range->min()) / (range->max() - range->min());
    draw_range = avail_length * range->range() / (range->max() - range->min());
  }
  zones[zone_min] = draw_val + getMinSize();
  zones[zone_thumb] = zones[zone_min] + draw_range +  getThumbSize();
  zones[zone_max] = length;
#ifdef PRINT
  std::cerr << "Slider zones: \n";
  std::cerr << "\tmin: " << zones[zone_min] << std::endl;
  std::cerr << "\trange_thumb: " << zones[zone_thumb] << std::endl;
  std::cerr << "\tmax: " << zones[zone_max] << std::endl;
#endif
}

static float
box_length(const Box& b, direction dir)
{
  switch(dir) {
  case left_to_right:
  case right_to_left:
    return width(b);
  case top_to_bottom:
  case bottom_to_top:
    return height(b);
  }
  return 0;
}

LiteSlider::Zone
LiteSlider::getZone(const Point& pos) const
{
  Box bounds(getBounds());
  if (! infovis::inside(bounds, x(pos), y(pos)))
    return zone_none;

  float zones[zone_last];
  computeZones(box_length(bounds, direction_), zones);

  switch(direction_) {
  case right_to_left:
  case left_to_right:
    if (x(pos) <= (xmin(bounds)+zones[zone_min]))
      return zone_min;
    else if (x(pos) <= (xmin(bounds)+zones[zone_thumb]))
      return zone_thumb;
    else
      return zone_max;
  case top_to_bottom:
  case bottom_to_top:
    if (y(pos) <= (ymin(bounds)+zones[zone_min]))
      return zone_min;
    else if (y(pos) <= (ymin(bounds)+zones[zone_thumb]))
      return zone_thumb;
    else
      return zone_max;
  }
  return zone_none;
}

static float
inter(float min, float max, float t)
{
  return min * (1 - t) + max * t;
}

float
LiteSlider::getValue(const Point& pos, Zone z) const
{
  float length = box_length(bounds, direction_)
    - getMinSize() - getThumbSize() - getMaxSize();
  float rel = box_length(Box(min(bounds),pos), direction_);
  
  const BoundedRange * range = observable_->getBoundedRange();

#ifdef PRINT
  std::cerr << "value for rel=" << rel << " length=" << length << std::endl;
#endif  
  if (z == zone_thumb || z == zone_max) {
    rel -= getMinSize();
    if (rel < 0)
      return range->min();
  
    if (z == zone_max) {
      rel -= getThumbSize();
      if (rel < 0)
	return range->min();
    }
  }
  float val = inter(range->min(), range->max(), rel / length);
  if (val <= range->min())
    val = range->min();
  else if (val >= range->max())
    val = range->max();
#ifdef PRINT
  std::cerr << "Value for position " << pos << " is " << val << std::endl;
#endif  
  return val;
}

void
LiteSlider::positionParts()
{
  float zones[zone_last];
  computeZones(box_length(bounds, direction_), zones);

  switch(direction_) {
  case right_to_left:
  case left_to_right:
    positionMin(Box(xmin(bounds)+zones[zone_min]-getMinSize(), ymin(bounds)+1,
		    xmin(bounds)+zones[zone_min], ymax(bounds)-1));
    positionThumb(Box(xmin(bounds)+zones[zone_min], ymin(bounds)+1,
		      xmin(bounds)+zones[zone_thumb], ymax(bounds)-1));
    positionMax(Box(xmin(bounds)+zones[zone_thumb], ymin(bounds)+1,
		    xmin(bounds)+zones[zone_thumb]+getMaxSize(), ymax(bounds)-1));
    break;
  case top_to_bottom:
  case bottom_to_top:
    positionMin(Box(xmin(bounds)+1, ymin(bounds)+zones[zone_min]-getMinSize(),
		    xmax(bounds)-1, ymin(bounds)+zones[zone_min]));
    positionThumb(Box(xmin(bounds)+1, ymin(bounds)+zones[zone_min],
		      xmax(bounds)-1, ymin(bounds)+zones[zone_thumb]));
    positionMax(Box(xmin(bounds)+1, ymin(bounds)+zones[zone_thumb],
		    xmax(bounds)-1, ymin(bounds)+zones[zone_thumb]+getMaxSize()));
    break;
  }
}

void
LiteSlider::doRender(const RenderContext& rc)
{
  Box bounds(getBounds());
  if (rc.is_picking) {
    set_color(background_color_);
    draw_box(bounds);
    return;
  }
  positionParts();
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  renderBackground(bounds);
  glPopAttrib();
}

void
LiteSlider::render(const RenderContext& c)
{
  LiteGroup::render(c);
  if (isVisible())
    renderOverlay(getBounds());
}

void
LiteSlider::renderBackground(const Box& b)
{
  if (background_color_[Color::alpha] != 0) {
    set_color(background_color_);
    draw_box(bounds);
  }
  else {
    set_color(foreground_color_);
    stroke_box(bounds);
  }
}

void
LiteSlider::positionThumb(const Box& b)
{
  Lite * thumb = getChild(0);
  if (thumb == 0)
    return;
  thumb->setBounds(b);
#if 0  
  set_color(foreground_color_);
  draw_box(b);
  if (font_ != 0) {
    char buffer[100];
    sprintf(buffer, "%.02g", observable_->getBoundedRange()->value());
    set_color(color_white);
    font_->paintGrown(buffer, xmin(b), ymin(b)+font_->getDescent());
    set_color(color_black);
    font_->paint(buffer, xmin(b), ymin(b)+font_->getDescent());
  }
#endif
}

void
LiteSlider::positionMin(const Box& b)
{
  Lite * l = getChild(1);
  if (l == 0)
    return;
  l->setBounds(b);
}

void
LiteSlider::positionMax(const Box& b)
{
  Lite * l = getChild(2);
  if (l == 0)
    return;
  l->setBounds(b);
}

string
LiteSlider::minLabel()
{
  return formatLabel(observable_->getBoundedRange()->value());
}

string
LiteSlider::maxLabel()
{
  return string();
}

string
LiteSlider::formatLabel(float value)
{
  if (formater_ == 0) {
    char buffer[100];
    sprintf(buffer, "%.02g", value);
    return buffer;
  }
  else
    return formater_->formatLabel(value);
}

void
LiteSlider::renderOverlay(const Box& b)
{
  if (font_ != 0) {
    string label;
    label = minLabel();
    if (! label.empty()) {
      float x = xmin(b) - font_->stringWidth(label);
      set_color(color_white);
      font_->paintGrown(label, x, ymin(b)+font_->getDescent());
      set_color(color_black);
      font_->paint(label, x, ymin(b)+font_->getDescent());
    }
    label = maxLabel();
    if (! label.empty()) {
      float x = xmax(b);
      set_color(color_white);
      font_->paintGrown(label, x, ymin(b)+font_->getDescent());
      set_color(color_black);
      font_->paint(label, x, ymin(b)+font_->getDescent());
    }
  }
}

BoundedRangeObservable*
LiteSlider::getObservable() const { return observable_; }

void
LiteSlider::setObservable(BoundedRangeObservable * v) {
  if (v != observable_) {
    observable_->removeBoundedRangeObserver(this);
    observable_ = v;
    observable_->addBoundedRangeObserver(this);
  }
}

direction
LiteSlider::getDirection() const { return direction_; }

void
LiteSlider::setDirection(direction v) {direction_ = v; }

const Color&
LiteSlider::getBackgroundColor() const { return background_color_; }

void
LiteSlider::setBackgroundColor(const Color& v) {background_color_ = v; }

const Color&
LiteSlider::getForegroundColor() const { return foreground_color_; }

void
LiteSlider::setForegroundColor(const Color& v) {foreground_color_ = v; }

Font *
LiteSlider::getFont() const { return font_; }

void
LiteSlider::setFont(Font * font) { font_ = font; }

void
LiteSlider::valueDragged(BoundedRangeObservable *) 
{
  repaint();
}
void
LiteSlider::rangeDragged(BoundedRangeObservable *)
{
  repaint();
}

void
LiteSlider::updateBoundedRange(BoundedRangeObservable *)
{
  repaint();
}

void
LiteSlider::setLabelFormater(LabelFormater * f)
{
  formater_ = f;
}  

LiteSlider::LabelFormater *
LiteSlider::getLabelFormater() const
{
  return formater_;
}

#if 0
bool
LiteSlider::doStart(const Event& ev)
{
  notifyBegin();
  BoundedRangeObservable * obs = getObservable();
  saved_ = obs->getBoundedRange()->value();
  obs->notifyValueDragged(getValue(getFieldPosition(ev)));
  repaint();
  return true;
}

void
LiteSlider::doDrag(const Event& ev)
{
  BoundedRangeObservable * obs = getObservable();
  obs->notifyValueDragged(getValue(getFieldPosition(ev)));
  repaint();
}

void
LiteSlider::doFinish(const Event& ev)
{
  BoundedRangeObservable * obs = getObservable();
  obs->notifyValueDragged(getValue(getFieldPosition(ev)));
  notifyEnd();
  obs->notifyBoundedRange();
  repaint();
}

void
LiteSlider::doAbort(const Event&)
{
  BoundedRangeObservable * obs = getObservable();
  obs->notifyValueDragged(saved_);
  obs->notifyBoundedRange();
}
#endif

} // namespace infovis

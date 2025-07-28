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
#include <infovis/drawing/lite/LiteColorChooser.hpp>
#include <infovis/drawing/SaveUnder.hpp>
#include <infovis/drawing/colors/hsb.hpp>
#include <infovis/drawing/colors/hvc.hpp>
#include <infovis/drawing/gl_support.hpp>

#undef DBG
#ifndef NDEBUG
#include <iostream>
#define DBG {						\
  int err = glGetError();				\
  if (err)						\
    std::cerr << __FILE__ << "(" << __LINE__ << ") : "	\
	      << "gl error: " << gl::glErrorString(err)	\
	      << std::endl;				\
}
#else
#define DBG
#endif

namespace infovis {
ColorScheme::ColorScheme()
  : x_observable_(0),
    y_observable_(0),
    z_observable_(0),
    x_(0),
    y_(0),
    z_(0)
{ }

ColorScheme *
ColorScheme::clone() const
{
  return new ColorScheme();
}

void
ColorScheme::fillColorRamp(Color * ramp, int n)
{
  //color_hsb hsb(1.0f, 1.0f, 1.0f);
  color_hvc hvc(0.0f, 0.0f, 1.0f);
  color_rgb<float> rgb;

  for (int i = 0; i < n; i++) {
    //hsb[color_hsb::hue] = float(i) / (n-1);
    hvc[color_hsb::hue] = float(i) * 360 / (n-1);
    //convert(rgb, hsb);
    convert(rgb, hvc);
    convert(ramp[i], rgb);
  }
}

void
ColorScheme::renderColorValue(const Box& b)
{
  color_hsb hsb(z_, 1, 1);
  color_rgb<float> rgb;

  glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);
  //glDisable(GL_BLEND);
  glShadeModel(GL_SMOOTH);
  {
  gl::begin_quads rect;

  hsb[color_hsb::saturation] = 1;
  hsb[color_hsb::brighness] = 1;
  convert(rgb, hsb);
  set_color(rgb);
  gl::vertex(xmax(b), ymax(b));
  
  hsb[color_hsb::saturation] = 0;
  hsb[color_hsb::brighness] = 1;
  convert(rgb, hsb);
  set_color(rgb);
  gl::vertex(xmin(b), ymax(b));

  hsb[color_hsb::saturation] = 0;
  hsb[color_hsb::brighness] = 0;
  convert(rgb, hsb);
  set_color(rgb);
  gl::vertex(xmin(b), ymin(b));
  
  hsb[color_hsb::saturation] = 1;
  hsb[color_hsb::brighness] = 0;
  convert(rgb, hsb);
  set_color(rgb);
  gl::vertex(xmax(b), ymin(b));
  }
  DBG;
  glPopAttrib();
  DBG;
}

void
ColorScheme::setXObservable(BoundedRangeObservable * obs)
{
  if (x_observable_ != 0)
    x_observable_->removeBoundedRangeObserver(this);
  x_observable_ = obs;
  if (obs != 0) {
    x_observable_->addBoundedRangeObserver(this);
    x_ = obs->getBoundedRange()->value();
  }
}

void
ColorScheme::setYObservable(BoundedRangeObservable * obs)
{
  if (y_observable_ != 0)
    y_observable_->removeBoundedRangeObserver(this);
  y_observable_ = obs;
  if (obs != 0) {
    y_observable_->addBoundedRangeObserver(this);
    y_ = obs->getBoundedRange()->value();
  }
}

void
ColorScheme::setZObservable(BoundedRangeObservable * obs)
{
  if (z_observable_ != 0)
    z_observable_->removeBoundedRangeObserver(this);
  z_observable_ = obs;
  if (obs != 0) {
    z_observable_->addBoundedRangeObserver(this);
    z_ = obs->getBoundedRange()->value();
  }
}

void
ColorScheme::valueDragged(BoundedRangeObservable * obs) 
{
  if (obs == x_observable_)
    x_ = obs->getBoundedRange()->value();
  else if (obs == y_observable_)
    y_ = obs->getBoundedRange()->value();
  else if (obs == z_observable_)
    z_ = obs->getBoundedRange()->value();
}

void
ColorScheme::rangeDragged(BoundedRangeObservable *obs)
{
  valueDragged(obs);
}

void
ColorScheme::updateBoundedRange(BoundedRangeObservable *obs)
{
  valueDragged(obs);
}


LiteColorSlider::LiteColorSlider(ColorScheme * scheme,
				 const Box& bounds,
				 direction dir,
				 const Color& fg,
				 const Color& bg)
  : LiteSlider(bounds, dir, fg, bg),
    texture_(0),
    scheme_(scheme)
{
  scheme_->setZObservable(getObservable());
}


LiteColorSlider::~LiteColorSlider()
{
  if (texture_ != 0) {
    glDeleteTextures(1, &texture_);
    DBG;
    texture_ = 0;
  }
}

Lite *
LiteColorSlider::clone() const
{
  return new LiteColorSlider(scheme_->clone(),
			     getBounds(),
			     getDirection(),
			     getForegroundColor(),
			     getBackgroundColor());
}

void
LiteColorSlider::renderBackground(const Box& b)
{
  glPushAttrib(GL_ENABLE_BIT);
  DBG;
  glEnable(GL_TEXTURE_1D);
  glDisable(GL_BLEND);
  DBG;
  if (texture_ == 0) {
    //std::cerr << "Creating 1D texture\n";
    Color ramp[256];
    fillColors(ramp, 256);
    glGenTextures(1, &texture_);
    DBG;
    glBindTexture(GL_TEXTURE_1D, texture_);
    DBG;
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0,
		 GL_RGBA, GL_UNSIGNED_BYTE, ramp);
    DBG;
  }
  else {
    //std::cerr << "Using 1D texture\n";
    glBindTexture(GL_TEXTURE_1D, texture_);
    DBG;
  }
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  {
    gl::begin_quads rect;
    switch(direction_) {
    case right_to_left:
    case left_to_right:
      gl::tex_coord(0.0f);
      gl::vertex(xmin(b), ymax(b));
      gl::vertex(xmin(b), ymin(b));
      gl::tex_coord(1.0f);
      gl::vertex(xmax(b), ymin(b));
      gl::vertex(xmax(b), ymax(b));
      break;
    case top_to_bottom:
      gl::tex_coord(0.0f);
      gl::vertex(xmax(b), ymax(b));
      gl::vertex(xmin(b), ymax(b));
      gl::tex_coord(1.0f);
      gl::vertex(xmin(b), ymin(b));
      gl::vertex(xmax(b), ymin(b));
    case bottom_to_top:
      break;
    }
  }
  glPopAttrib();
  DBG;
  glBindTexture(GL_TEXTURE_1D, 0);
  DBG;
}

void
LiteColorSlider::renderThumb(const Box& b)
{
  set_color(foreground_color_);
  draw_box(b);
  set_color(background_color_);
  stroke_box(b);
}

void
LiteColorSlider::fillColors(Color * ramp, int n)
{
  scheme_->fillColorRamp(ramp, n);
}

LiteColorSurface::LiteColorSurface(ColorScheme * scheme,
				   const Box& bounds,
				   const Color& fg,
				   const Color& bg)
  : LiteBounded(bounds),
    x_observable_(new DefaultBoundedRangeObservable(0, 1, 0)),
    y_observable_(new DefaultBoundedRangeObservable(0, 1, 0)),
    foreground_color_(fg),
    background_color_(bg),
    z_(1),
    scheme_(scheme)
{
  x_observable_->addBoundedRangeObserver(this);
  y_observable_->addBoundedRangeObserver(this);
  scheme_->setXObservable(x_observable_);
  scheme_->setYObservable(y_observable_);
}

Lite *
LiteColorSurface::clone() const
{
  return new LiteColorSurface(scheme_->clone(),
			      getBounds(),
			      getForegroundColor(),
			      getBackgroundColor());
}

Interactor *
LiteColorSurface::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id) {
    return this;
  }
  return 0;
}

void
LiteColorSurface::doRender(const RenderContext& rc)
{
  Box bounds(getBounds());
  if (rc.is_picking) {
    set_color(background_color_);
    draw_box(bounds);
    return;
  }
  renderBackground(bounds);
  float x = xmin(bounds) +
    getXObservable()->getBoundedRange()->value() * width(bounds);
  float y = ymin(bounds) +
    getYObservable()->getBoundedRange()->value() * height(bounds);
  renderValue(bounds, x, y);
  renderOverlay(bounds);
}

float
LiteColorSurface::getXValue(const Event& ev) const
{
  return (getFieldX(ev) - xmin(bounds)) / width(bounds);
}

float
LiteColorSurface::getYValue(const Event& ev) const
{
  return (getFieldY(ev) - ymin(bounds)) / height(bounds);
}

float
LiteColorSurface::getZ() const
{
  return z_;
} 

void
LiteColorSurface::setZ(float f)
{
  z_ = f;
}

void
LiteColorSurface::renderBackground(const Box& b)
{
  scheme_->renderColorValue(b);
}

void
LiteColorSurface::renderValue(const Box& b, float x, float y)
{
  set_color(foreground_color_);
  gl::begin_lines lines;
  gl::vertex(xmin(b), y);
  gl::vertex(xmax(b), y);
  gl::vertex(x, ymin(b));
  gl::vertex(x, ymax(b));
}

void
LiteColorSurface::renderOverlay(const Box& b)
{ }

BoundedRangeObservable*
LiteColorSurface::getXObservable() const { return x_observable_; }

void
LiteColorSurface::setXObservable(BoundedRangeObservable * v) {
  if (v != x_observable_) {
    x_observable_->removeBoundedRangeObserver(this);
    x_observable_ = v;
    x_observable_->addBoundedRangeObserver(this);
  }
}

BoundedRangeObservable*
LiteColorSurface::getYObservable() const { return y_observable_; }

void
LiteColorSurface::setYObservable(BoundedRangeObservable * v) {
  if (v != y_observable_) {
    y_observable_->removeBoundedRangeObserver(this);
    y_observable_ = v;
    y_observable_->addBoundedRangeObserver(this);
  }
}

const Color&
LiteColorSurface::getBackgroundColor() const { return background_color_; }

void
LiteColorSurface::setBackgroundColor(const Color& v) {background_color_ = v; }

const Color&
LiteColorSurface::getForegroundColor() const { return foreground_color_; }

void
LiteColorSurface::setForegroundColor(const Color& v) {foreground_color_ = v; }

void
LiteColorSurface::valueDragged(BoundedRangeObservable *) 
{
  repaint();
}
void
LiteColorSurface::rangeDragged(BoundedRangeObservable *)
{
  repaint();
}

void
LiteColorSurface::updateBoundedRange(BoundedRangeObservable *)
{
  repaint();
}

bool
LiteColorSurface::doStart(const Event& ev)
{
  notifyBegin();
  BoundedRangeObservable * obs = getXObservable();
  x_saved_ = obs->getBoundedRange()->value();
  obs->notifyValueDragged(getXValue(ev));
  obs = getYObservable();
  y_saved_ = obs->getBoundedRange()->value();
  obs->notifyValueDragged(getYValue(ev));
  repaint();
  return true;
}

void
LiteColorSurface::doDrag(const Event& ev)
{
  BoundedRangeObservable * obs = getXObservable();
  obs->notifyValueDragged(getXValue(ev));
  obs = getYObservable();
  obs->notifyValueDragged(getYValue(ev));
  repaint();
}

void
LiteColorSurface::doFinish(const Event& ev)
{
  getXObservable()->notifyValueDragged(getXValue(ev));
  getYObservable()->notifyValueDragged(getYValue(ev));
  notifyEnd();
  getXObservable()->notifyBoundedRange();
  getYObservable()->notifyBoundedRange();
  repaint();
}

void
LiteColorSurface::doAbort(const Event&)
{
  BoundedRangeObservable * obs = getXObservable();
  obs->notifyValueDragged(x_saved_);
  obs->notifyBoundedRange();
  obs = getYObservable();
  obs->notifyValueDragged(y_saved_);
  obs->notifyBoundedRange();
}

void
LiteColorSurface::fillColors(Color * surface, int width, int height)
{
  
}
} // namespace infovis

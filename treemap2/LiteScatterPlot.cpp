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
#include <LiteScatterPlot.hpp>
#include <BoxDrawer.hpp>
#include <Properties.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/tree/numeric_prop_min_max.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <iostream>

#undef DBG
#ifndef NDEBUG
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

LiteScatterPlot::LiteScatterPlot(const Box& b,
				 SaveUnder& save_under,
				 FastDrawer& drawer,
				 Font * lab,
				 Tree& t,
				 const string& name_prop,
				 const string& x_axis_prop,
				 const string& y_axis_prop,
				 const string& color_prop,
				 const string& size_prop,
				 BoundedRangeObservable& param
				 )
  : LiteGroup(b),
    tex_action_(save_texture),
    save_under_(save_under),
    drawer_(drawer),
    label_font_(lab),
    tree_(t),

    //name_prop_(name_prop),
    //x_axis_prop_(x_axis_prop),
    //y_axis_prop_(y_axis_prop),
    //color_prop_(color_prop),
    //size_prop_(size_prop),

    name_(0),
    x_axis_(0),
    y_axis_(0),
    color_(0),
    size_(0),

    param_(param),
    fps_(0),
    animation_start_time_(0),
    animation_end_time_(0),
    animate_(0),
    animate_reversed_(false)
{
  DBG;
  setNameProp(name_prop);
  setXAxisProp(x_axis_prop);
  setYAxisProp(y_axis_prop);
  setColorProp(color_prop);
  setSizeProp(size_prop);
}

LiteScatterPlot::~LiteScatterPlot()
{ }

void
LiteScatterPlot::disableSaveUnder()
{
  tex_action_ = no_texture;
}
void
LiteScatterPlot::enableSaveUnder(bool reuse_cache)
{
  if (reuse_cache)
    tex_action_ = use_texture;
  else
    tex_action_ = save_texture;
}

Interactor *
LiteScatterPlot::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id)
    return this;
  return 0;
}

inline Box
LiteScatterPlot::computeBox(int i) const
{
  float sz = (*size_)[i];
  return Box();
}

void
LiteScatterPlot::doRender(const RenderContext& c)
{
  if ( c.is_picking) {
      set_color(color_white);
      draw_box(bounds);
      return;
    }
  DBG;
  int t = LiteWindow::time();

  float param = param_.getBoundedRange()->value();
  bool was_animating = false;
  if (isAnimating()) {
    was_animating = true;
    param = animationParam(t);
  }
  if (tex_action_ == use_texture) {
      save_under_.restore();
      DBG;
  }
  else if (param != 0.0f && animate_ != 0) {
    if (animate_reversed_)
      param = 1.0f - param;
    animate_->render(param);
    repaint();			// force last repaint
  }
  else {
    glPushMatrix();
    float x_scale = x_axis_max_ - x_axis_min_;
    if (x_scale == 0)
      x_scale = 1;
    float y_scale = y_axis_max_ - y_axis_min_;
    if (y_scale == 0)
      y_scale = 1;

    glScalef(width(bounds) / x_scale,
	     height(bounds) / y_scale,
	     -1);
    Vector one(x_scale / width(bounds),
	       y_scale / height(bounds));

    glTranslatef(x_axis_min_, y_axis_min_, 0);
    drawer_.start();

    for(int i = 0; i < x_axis_->size(); i++) {
      Box b(Point((*x_axis_)[i], (*y_axis_)[i]), one);
      drawer_.push(computeBox(i), i, (*color_)[i]);
    }

    drawer_.finish();
    glPopMatrix();
    DBG;
    if (tex_action_ == save_texture) {
      save_under_.save(int(xmin(bounds)),
		       int(ymin(bounds)),
		       int(width(bounds)),
		       int(height(bounds)));
      tex_action_ = use_texture;
      if (animate_ != 0 && animate_reversed_) {
	animate_->swap();
	//animate_->computeTexCoords(getTextureSize());
	animate_reversed_ = false;
      }
      DBG;
    }
  }
  glFlush();
  t = LiteWindow::time() - t;
  fps_ = 1000.0f / t;
  if (was_animating)
    repaint();
}

// Interactor3States
void
LiteScatterPlot::doMove(const Point& pos)
{
}

void
LiteScatterPlot::doStart(const Point& pos)
{
}

void
LiteScatterPlot::doDrag(const Point& pos)
{
}

void
LiteScatterPlot::doFinish(const Point& pos)
{
}

void
LiteScatterPlot::setNameProp(const string& name_prop)
{
  if (name_prop_ == name_prop)
    return;
  name_prop_ = name_prop;
  name_ = &tree_.get_prop_string(name_prop_);
}

void
LiteScatterPlot::setXAxisProp(const string& x_axis_prop)
{
  if (x_axis_prop_ == x_axis_prop)
    return;
  x_axis_prop_ = x_axis_prop;
  x_axis_ = &tree_.get_prop_numeric(x_axis_prop_);
  numeric_prop_min_max(*x_axis_, x_axis_min_, x_axis_max_);
}

void
LiteScatterPlot::setYAxisProp(const string& y_axis_prop)
{
  if (y_axis_prop_ == y_axis_prop)
    return;
  y_axis_prop_ = y_axis_prop;

  y_axis_ = &tree_.get_prop_numeric(y_axis_prop_);
  numeric_prop_min_max(*y_axis_, y_axis_min_, y_axis_max_);
}

void
LiteScatterPlot::setColorProp(const string& prop)
{
  if (color_prop_ == prop)
    return;

  color_prop_ = prop;
  color_ = &tree_.get_prop_numeric(color_prop_);
  numeric_prop_min_max(*color_, color_min_, color_max_);
}

void
LiteScatterPlot::setSizeProp(const string& prop)
{
  if (size_prop_ == prop)
    return;

  size_prop_ = prop;
  size_ = &tree_.get_prop_numeric(size_prop_);
  numeric_prop_min_max(*size_, size_min_, size_max_);
}

// BoundedRangeObserver
void
LiteScatterPlot::valueDragged(BoundedRangeObservable * obs)
{
}

void
LiteScatterPlot::rangeDragged(BoundedRangeObservable * obs)
{
  //repaint();
}

void
LiteScatterPlot::updateBoundedRange(BoundedRangeObservable * obs)
{
}

// BeginEndObserver
void
LiteScatterPlot::begin(BeginEndObservable *)
{
  disableSaveUnder();
}
void
LiteScatterPlot::end(BeginEndObservable *)
{
  enableSaveUnder();
}

void
LiteScatterPlot::beginAnimation(int duration)
{
  std::cerr << "Starting animation for " << duration << "ms\n";
  animation_start_time_ = LiteWindow::time();
  animation_end_time_ = animation_start_time_ + duration;
  disableSaveUnder();
}

void
LiteScatterPlot::endAnimation()
{
  if (isAnimating()) {
    std::cerr << "Ending animation\n";
    animation_start_time_ = animation_end_time_;
    enableSaveUnder();
  }
}

bool
LiteScatterPlot::isAnimating() const
{
  return animation_start_time_ < animation_end_time_;
}

float
LiteScatterPlot::animationParam(int time)
{
  if (isAnimating()) {
    if (time == 0)
      time = LiteWindow::time();
    //std::cerr << "animation at t = " <<
    //time - animation_start_time_ << std::endl;
    if (time >= animation_end_time_) {
      endAnimation();
      return 0.0f;
    }
    return float(animation_end_time_ - time) /
      float(animation_end_time_ - animation_start_time_);
  }
  return 0.0f;
}

void
LiteScatterPlot::setAnimate(Animate * a, bool rev)
{
  if (animate_ != 0)
    delete animate_;
  animate_ = a;
  animate_reversed_ = rev;
}

const Animate::BoxList
LiteScatterPlot::computeBoxList(unsigned depth) const
{
  return Animate::BoxList();
}


} // namespace infovis

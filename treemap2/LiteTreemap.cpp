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
#include <LayoutVisu.hpp>
#include <Properties.hpp>
#include <LabelTreemap.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/inter/KeyCodes.hpp>
#include <infovis/drawing/Image.hpp>
#include <infovis/tree/numeric_prop_min_max.hpp>
#include <iostream>
#include <GL/glu.h>

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

#define USE_SAVE_UNDER

static inline Lite::Path
operator + (const Lite::Path& p1, const Lite::Path& p2)
{
  Lite::Path ret(p1);
  ret.insert(ret.end(), p2.begin(), p2.end());
  return ret;
}

static Font *
get_prop_font(const string& prop_name)
{
  return Properties::instance()->get_font(prop_name, Font::create());
}

LiteTreemap::LiteTreemap(const Box& b,
			 SaveUnder& save_under,
			 Drawer& drawer,
			 Font * lab,
			 Tree& t,
			 const string& name,
			 const string& weight,
			 const string& weight2,
			 const string& color_prop,
			 BoundedRangeObservable& p,
			 BoundedRangeObservable& transparency,
			 BoundedRangeObservable& plot_range
			 )
  : LiteGroup(b),
    tex_action_(save_texture),
    save_under_(save_under),
    label_font_(lab),
    tree_(t),
    name_prop_(name),
    weight_prop_(weight),
    weight2_prop_(weight2),
    color_prop_(color_prop),
    x_axis_prop_(weight_prop_),
    y_axis_prop_(weight2_prop_),
    weight_(FloatColumn::find(weight_prop_, t)),
    color_(FloatColumn::find(color_prop_, t)),
    x_axis_(FloatColumn::find(x_axis_prop_, t)),
    y_axis_(FloatColumn::find(y_axis_prop_, t)),
    
    drawer_(drawer),
    orient_(),
    picker_(t, drawer_),
    menu_(root(tree_),
	  tree_,
	  *StringColumn::find(name_prop_, t),
	  get_prop_font("menu.font")),
    labels_(new LabelTreemap(this, name, get_prop_font("menu.font"))),
    param_(p),
    transparency_(transparency),
    plot_range_(plot_range),
    fps_(0),
    displayed_items_(0),
    layout_(layout_squarified),
    visu_(0),
    animation_start_time_(0),
    animation_end_time_(0),
    animation_current_time_(0),
    animate_(0),
    overlaps_(0),
    show_overlaps_(false),
    list_(0),
    shift_(false),
    inhibit_dynamic_labels_(false)
{
  DBG;
  current_root_ = root(tree_);
  weight_min_ = weight_->min();
  weight_max_ = weight_->max();

  color_min_ = color_->min();
  color_max_ = color_->max();

  x_axis_min_ = x_axis_->min();
  x_axis_max_ = x_axis_->max();

  y_axis_min_ = y_axis_->min();
  y_axis_max_ = y_axis_->max();

  visu_ = LayoutVisu::create_visu(layout_, this);
  // force creation of texture before everything else to avoid a
  // strange bug, probably due to texture proxy.
  //std::cerr << "Reshape " << std::endl;
  save_under_.save(0, 0, 10, 10);
  menu_.setVisible(false);
  addChild(&menu_);
  menu_.addBoundedRangeObserver(this);
  addChild(labels_);
  //list_ = glGenLists(1);
}

LiteTreemap::~LiteTreemap()
{
  if (overlaps_ != 0)
    delete overlaps_;
  overlaps_ = 0;
  glDeleteLists(list_, 1);
}

void
LiteTreemap::disableSaveUnder()
{
  tex_action_ = no_texture;
}

void
LiteTreemap::enableSaveUnder(bool reuse_cache)
{
  if (reuse_cache)
    tex_action_ = use_texture;
  else
    tex_action_ = save_texture;
}

void
LiteTreemap::update_root(node_descriptor n)
{
  if (n != -1 &&  n != current_root_)
    {
      current_root_ = n;
      //current_path_ = menu_.getSelectedPath();
      //menu_.setReference(menu_.getSelected());
      //menu_.setSelected(-1);
#ifdef USE_SAVE_UNDER
      //std::cerr << "update_root setting tex_action to save_under\n";
      if (tex_action_ != no_texture)
	tex_action_ = save_texture;
#endif
      picker_.set_label_level(1);
      picker_.set_labels_clip(Box());
      labels_->setVisible(false);
      
    }
}


Interactor *
LiteTreemap::interactor(const string& name, int tool_id)
{
  if (InteractorEnterLeave::isA(name))
    return (InteractorEnterLeave*)this;
  else if (InteractorIdle::isA(name))
    return (InteractorIdle*)this;
  else if (Interactor3States::isA(name))
    return (Interactor3States*)this;
  return 0;
}

void

LiteTreemap::doRender(const RenderContext& c)
{
  //std::cerr << "Render " << std::endl;
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
    if (show_overlaps_ && overlaps_ != 0)
      overlaps_->render(xmin(bounds), ymin(bounds));
    else {
      save_under_.restore();
      //std::cerr << "restoring texture\n";
    }
    DBG;
  }
  else if (was_animating && animate_ != 0) {
    animate_->render(1.0f - param);
    repaint();			// force last repaint
  }
  else {
    displayed_items_ = visu_->draw(param);
    if (tex_action_ == save_texture) {
#ifdef USE_SAVE_UNDER
      save_under_.save(int(xmin(bounds)),
		       int(ymin(bounds)),
		       int(width(bounds)),
		       int(height(bounds)));
      //std::cerr << "Saving texture with param = " << param << std::endl;
      tex_action_ = use_texture;
#endif
      DBG;
    }
  }
  if (! empty(hilite_box_)) {
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::color(0.3, 0.3, 0.3, 0.8);
    gl::rect(xmin(bounds), ymin(bounds),
	     xmin(hilite_box_), ymax(bounds));
    gl::rect(xmin(hilite_box_), ymin(bounds),
	     xmax(hilite_box_), ymin(hilite_box_));
    gl::rect(xmin(hilite_box_), ymax(hilite_box_),
	     xmax(hilite_box_), ymax(bounds));
    gl::rect(xmax(hilite_box_), ymin(bounds),
	     xmax(bounds), ymax(bounds));
    glPopAttrib();
    DBG;
  }
  //std::cerr << "line_alpha = " << line_alpha << std::endl;
  if (param == 0.0f && ! was_animating && tex_action_ != no_texture) {
    visu_->pick(param);
  }
  DBG;
  glFlush();
  t = LiteWindow::time() - t;
  if (t == 0) t = 1;
  fps_ = 1000.0f / t;
  if (was_animating) {
    repaint();
  }
  glShadeModel(GL_FLAT);
}

bool
LiteTreemap::doIdle(const Event& ev)
{
#if 1
  if (layout_ == layout_scatter_plot ||
      labels_->isVisible() ||
      isAnimating() ||
      inhibit_dynamic_labels_)
    return false;
  enableDynamicLabels();
#else
  if (menu_.isVisible())	// ignore idle when manipulating menus
    return false;
  menu_.setPosition(getFieldPosition(ev));
  menu_.setVisible(true);
#endif
  repaint();
  return true;
}

void
LiteTreemap::doActive(const Event& ev)
{
}

static float dist2(float dx, float dy) {
  return dx*dx + dy*dy;
}

void
LiteTreemap::doMove(const Event& ev)
{
  static float old_x, old_y;

  float x = getFieldX(ev);
  float y = getFieldY(ev);
  if (! shift_ && ! menu_.isDragged()) {
    current_boxes_ = picker_.getPathBoxes();
    node_descriptor top = picker_.getHit();
    if (top != root(tree_)) {
      menu_.setPath(top);
      labels_->setHit(top);
      labels_->setBoxes(current_boxes_);
      labels_->layoutLabels();
    }
    picker_.set_pos(int(x), int(y)); // track hit
    if (menu_.isVisible() &&
	! getFieldMod(ev)) {
      if (dist2(x - old_x, y - old_y) > 750) {
	//std::cerr << "distance = " << dist2(x - old_x, y - old_y) << std::endl;
	menu_.setVisible(false);
      }
      else {
	menu_.setPosition(getFieldPosition(ev));
      }
    }
    if (labels_->isVisible() &&
	dist2(x - old_x, y - old_y) > 750) {
      //std::cerr << "distance = " << dist2(x - old_x, y - old_y) << std::endl;
      disableDynamicLabels();
    }
    repaint();
  }
  old_x = x;
  old_y = y;
}

void
LiteTreemap::enableDynamicLabels()
{
  labels_->setVisible(true);
  picker_.set_label_level(-1);
}

void
LiteTreemap::disableDynamicLabels(bool inhibit)
{
  labels_->setVisible(false);
  picker_.set_label_level(1);
  inhibit_dynamic_labels_ = inhibit;
}


void
LiteTreemap::setCurrentRoot(node_descriptor n)
{
  current_root_ = n;
}

void
LiteTreemap::setColorProp(const string& prop)
{
  if (prop == color_prop_)
    return;
  FloatColumn * np = FloatColumn::cast(tree_.find_column(prop));
  if (np == 0)
    return;
  color_prop_ = prop;
  color_ = np;
  color_min_ = color_->min();
  color_max_ = color_->max();
  drawer_.set_color_prop(color_);
  updateMinMax();
}

void
LiteTreemap::setXAxisProp(const string& prop)
{
  if (x_axis_prop_ == prop)
    return;
  x_axis_prop_ = prop;

  x_axis_ = FloatColumn::find(x_axis_prop_, tree_);
  x_axis_min_ = x_axis_->min();
  x_axis_max_ = x_axis_->max();
  updateMinMax();
}

void
LiteTreemap::setYAxisProp(const string& prop)
{
  if (y_axis_prop_ == prop)
    return;
  y_axis_prop_ = prop;

  y_axis_ = FloatColumn::find(y_axis_prop_, tree_);
  y_axis_min_ = y_axis_->min();
  y_axis_max_ = y_axis_->max();
  updateMinMax();
}

void
LiteTreemap::setWeightProp(const string& prop)
{
  if (prop == weight_prop_)
    return;
  weight2_prop_ = weight_prop_;
  weight_prop_ = prop;

  weight_ = FloatColumn::find(weight_prop_, tree_);
  weight_min_ = weight_->min();
  weight_max_ = weight_->max();
}

static inline int updateColumn(const FloatColumn * col,
			       const FilterColumn * filter,
			       float& min_val, float& max_val)
{
  int i;
  int cnt = 0;
  for (i = 0; i < col->size(); i++) {
    if (filter->fast_get(i) == 0) {
      min_val = max_val = col->fast_get(i);
      break;
    }
  }
  for (; i < col->size(); i++) {
    if (filter->fast_get(i) == 0) {
      float val = col->fast_get(i);
      if (val < min_val)
	min_val = val;
      else if (val > max_val)
	max_val = val;
    }
  }
  if (min_val == max_val) {
    min_val = max_val-1;	// avoids division by 0
  }
  return cnt;
}

void
LiteTreemap::updateMinMax()
{
  FilterColumn * filter = FilterColumn::find("$filter", tree_);

  updateColumn(weight_, filter, weight_min_, weight_max_);
  updateColumn(x_axis_, filter, x_axis_min_, x_axis_max_);
  updateColumn(y_axis_, filter, y_axis_min_, y_axis_max_);
  updateColumn(color_, filter, color_min_, color_max_);
  //drawer_.set_color_range(color_min_, color_max_ - color_min_);
}

// BoundedRangeObserver
void
LiteTreemap::valueDragged(BoundedRangeObservable * obs)
{
  if (obs == &menu_) {
      //std::cerr << "valueDragged(" << value << std::endl;
      float value = menu_.value();
      value -= node_depth(current_root_,tree_);
      if (value < 0 || value > current_boxes_.size()) {
	  hilite_box_ = Box();
	  picker_.set_labels_clip(hilite_box_); // redundant
	  picker_.set_label_level(1);
	  return;
	}
      hilite_box_ = current_boxes_[int(value)];
      picker_.set_labels_clip(hilite_box_);	// redundant
      picker_.set_pos(int(xmin(hilite_box_)+width(hilite_box_)/2),
		     int(ymin(hilite_box_)+height(hilite_box_)/2));
      picker_.set_label_level(int(value+1));
    }
}

void
LiteTreemap::rangeDragged(BoundedRangeObservable * obs)
{
  //repaint();
}

void
LiteTreemap::updateBoundedRange(BoundedRangeObservable * obs)
{
  if (obs == &menu_) {
    hilite_box_ = Box();
    picker_.set_labels_clip(hilite_box_);	// redundant
    update_root(menu_.getSelectedRoot());
  }
}

// BeginEndObserver
void
LiteTreemap::begin(BeginEndObservable *)
{
  disableSaveUnder();
}
void
LiteTreemap::end(BeginEndObservable *)
{
  enableSaveUnder();
}

void
LiteTreemap::changed(ChangeObservable * o)
{
  picker_.toggle_show_all_labels();
}

const StringColumn& 
LiteTreemap::getNames() const
{
  return *StringColumn::find(name_prop_, tree_);
}

void
LiteTreemap::setStrip(bool b)
{
  orient_.set_strip(b);
  enableSaveUnder();
}

void
LiteTreemap::setLayout(Layout l)
{
  if (layout_ == l)
    return;
  layout_ = l;
  if (layout_ == layout_scatter_plot)
    labels_->setVisible(false);
  visu_ = LayoutVisu::create_visu(layout_, this);
  param_.notifyValueDragged(0);
  param_.notifyBoundedRange();
  updateMinMax();
  enableSaveUnder();
  repaint();
}

void
LiteTreemap::beginAnimation(int duration)
{
  //std::cerr << "Starting animation for " << duration << "ms\n";
  animation_start_time_ = LiteWindow::time();
  animation_current_time_ = animation_start_time_;
  animation_end_time_ = animation_start_time_ + duration;
  disableSaveUnder();
}

void
LiteTreemap::endAnimation()
{
  if (isAnimating()) {
    std::cerr << "Ending animation\n";
    animation_current_time_ = animation_end_time_;
    animation_start_time_ = animation_end_time_;
    enableSaveUnder();
  }
}

bool
LiteTreemap::isAnimating() const
{
  return animation_current_time_ < animation_end_time_;
}

float
LiteTreemap::animationParam(int time)
{
  if (isAnimating()) {
    //if (time == 0)
      //time = LiteWindow::time();
      time = animation_current_time_;
    //std::cerr << "animation at t = " <<
    //time - animation_start_time_ << std::endl;
    if (animation_current_time_ >= animation_end_time_) {
      endAnimation();
      return 0.0f;
    }
    float ret =
      float(animation_end_time_ - animation_current_time_) /
      float(animation_end_time_ - animation_start_time_);
    animation_current_time_ += 100; // assume 100ms redisplay time
    return ret;
  }
  return 1.0f;
}

void
LiteTreemap::setAnimate(Animate * a)
{
  animate_ = a;
}

void
LiteTreemap::computeBoxList(float param,
			    unsigned depth, AnimateTree::BoxList& bl) const
{
  int time = LiteWindow::time();
  //float param = param_.getBoundedRange()->value();
  visu_->boxlist(param, bl, depth);
  time = LiteWindow::time() - time;
  std::cerr << "computeBoxList:: " << time / 1000.0f << " seconds\n";
}

void
LiteTreemap::doKeyboard(int key, bool down, int x, int y)
{
  if (key == 'd' && down) {
    drawer_.set_dryrun(true);
    disableSaveUnder();
    repaint();
  }
  else if (key == 'D' && down) {
    drawer_.set_dryrun(false);
    enableSaveUnder();
    repaint();
  }
  else if (key == 'o' && layout_ == layout_scatter_plot) {
    show_overlaps_ = down;
    repaint();
  }
  else if (key == keycode_shift) {
    shift_ = down;
  }
}

} // namespace infovis

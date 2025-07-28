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
#include <infovis/drawing/lite/LiteComboBox.hpp>
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/lite/LiteMenuSimple.hpp>
#include <infovis/drawing/lite/LiteMover.hpp>
#include <infovis/drawing/lite/LiteRangeSlider.hpp>
#include <infovis/drawing/lite/LiteSliderExt.hpp>
#include <infovis/drawing/lite/LiteRect.hpp>
#include <infovis/drawing/lite/LiteTabbed.hpp>
#include <infovis/drawing/lite/LiteToggleButton.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/lite/LayoutTable.hpp>

#include <infovis/table/metadata.hpp>

#include <infovis/drawing/AnimateCompose.hpp>
#include <ControlsTab.hpp>
#include <BarGraph.hpp>
#include <LiteRangeSliderGraph.hpp>
#include <LayoutVisu.hpp>

#include <iostream>

#include <time.h>

namespace infovis {

class DateFormatter : public LiteSlider::LabelFormater {
public:
  string formatLabel(float value) {
    char buffer[100];
    time_t time = time_t(value);
    strftime(buffer, 100, "%x %X", localtime(&time));
    return buffer;
  }
};

static DateFormatter date_formater;

class AnimateVisu : public Animate
{
public:
  AnimateVisu(LayoutVisu * visu, ControlsTab * win)
    : visu_(visu), win_(win) { }

  void render(float param);
protected:
  LayoutVisu * visu_;
  ControlsTab * win_;
};

class AnimateContinue : public Animate
{
public:
  AnimateContinue(ControlsTab * win)
    : win_(win) { }

  void render(float param);
protected:
  ControlsTab * win_;
};


static LiteLabel *
create_label(const string& label, Font * font)
{
  return new LiteLabel(label, LiteLabel::just_left, font, color_black, color_white);
}

ControlsTab::ControlsTab(LiteTreemap * tm,
			 const Box& screen,
			 Font * label_font,
			 BoundedRangeObservable * param,
			 BoundedRangeObservable * transparency,
			 BoundedRangeObservable * plot_range)
  : LiteBackground(0, color_white),
    treemap_(tm),
    tree_(tm->tree_),
    label_font_(label_font),
    param_(param),
    transparency_(transparency),
    plot_range_(plot_range),
    color_range_(0, 7, 0, 7),
    recorder_(),
    filter_(FilterColumn::find("$filter", tree_)),
    animate_(num_nodes(tree_), tree_, treemap_->getDrawer()),
    animation_duration_(1, 20, 5)
{
  box_ = new LiteBox();
  
  tab_ = new LiteTabbed();
  box_->addChild(tab_);

  queries_ = new LiteBox(new LayoutTable(2));

  for (auto n = tree_.begin_names(); n != tree_.end_names(); ++n) {
    const string& prop = *n;
    const column* col = tree_.find_column(prop);
    if (prop[0] == '$') // skip local columns
      continue;
    const FloatColumn* flt = FloatColumn::cast(col);
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

  controls_ = new LiteBox(new LayoutTable(2));
  tab_->setSelected(0);

  replay_slider_ = new LiteSliderExt(&recorder_,
				     Box(100, 0, 200, 10),
				     screen,
				     left_to_right);
  controls_->addChild(create_label("Replay:", label_font_));
  controls_->addChild(replay_slider_);
  replay_slider_->setFont(label_font_);
  recorder_.addBoundedRangeObserver(this);

  animation_duration_slider_ = new LiteSliderExt(&animation_duration_,
					   Box(100, 0, 200, 10),
					   screen,
					   left_to_right);
  controls_->addChild(create_label("Animation:", label_font_));
  controls_->addChild(animation_duration_slider_);
  animation_duration_slider_->setFont(label_font_);

  label_toggle_ = new LiteToggleButton(20);
  controls_->addChild(create_label("Label All:", label_font_));
  controls_->addChild(label_toggle_);
  label_toggle_->addChangeObserver(treemap_);

  border_slider_ = new LiteSliderExt(//&border,
				     Box(100, 0, 200, 10),
				     screen,
				     left_to_right);
  controls_->addChild(create_label("Border Thickness:", label_font_));
  controls_->addChild(border_slider_);
  border_slider_->setObservable(&treemap_->getDrawer());
  border_slider_->addBeginEndObserver(treemap_);
  border_slider_->addBeginEndObserver(this);
  border_slider_->setFont(label_font_);

  blend_slider_ = new LiteSliderExt(param_,
				    Box(100, 0, 200, 10),
				    screen,
				    left_to_right);
  controls_->addChild(create_label("Blend Sizes:", label_font_));
  controls_->addChild(blend_slider_);
  blend_slider_->addBeginEndObserver(treemap_);
  blend_slider_->addBeginEndObserver(this);
  blend_slider_->setFont(label_font_);

  transparency_slider_ = new LiteSliderExt(transparency_,
					   Box(100, 0, 200, 10),
					   screen,
					   left_to_right);
  controls_->addChild(create_label("Transparency", label_font_));
  controls_->addChild(transparency_slider_);
  transparency_slider_->addBeginEndObserver(treemap_);
  transparency_slider_->addBeginEndObserver(this);
  transparency_slider_->setFont(label_font_);

  sort_by_menu_ = new LiteMenuSimple(label_font_);
  sort_by_combo_ = new LiteComboBox(sort_by_menu_);
  current_sort_by_ = "";
  controls_->addChild(create_label("Sort By:", label_font_));
  controls_->addChild(sort_by_combo_);
  sort_by_menu_->addBoundedRangeObserver(this);
  sort_by_menu_->addBeginEndObserver(this);

  plot_range_slider_ = new LiteRangeSlider(plot_range_,
					   Box(100, 0, 200, 10),
					   left_to_right);
  controls_->addChild(create_label("Plot Range", label_font_));
  controls_->addChild(plot_range_slider_);
  plot_range_slider_->addBeginEndObserver(treemap_);
  plot_range_slider_->addBeginEndObserver(this);
  plot_range_slider_->setFont(label_font_);

  x_attributes_menu_ = new LiteMenuSimple(label_font_);
  x_attributes_combo_ = new LiteComboBox(x_attributes_menu_);
  controls_->addChild(create_label("X Attribute:", label_font_));
  controls_->addChild(x_attributes_combo_);
  x_attributes_menu_->addBoundedRangeObserver(this);
  x_attributes_menu_->addBeginEndObserver(this);
  //treemap_->addChild(x_attributes_combo_);
  //x_attributes_combo_->setBounds(Box(100, 0, 200, 20)); // FIXME+++
  //x_attributes_combo_->invalidate();
  //x_attributes_combo_->setVisible(false);

  y_attributes_menu_ = new LiteMenuSimple(label_font_);
  y_attributes_combo_ = new LiteComboBox(y_attributes_menu_);
  controls_->addChild(create_label("Y Attribute:", label_font_));
  controls_->addChild(y_attributes_combo_);
  y_attributes_menu_->addBoundedRangeObserver(this);
  y_attributes_menu_->addBeginEndObserver(this);
//   treemap_->addChild(y_attributes_combo_);
//   y_attributes_combo_->setBounds(Box(0, 100, 100, 120));// FIXME+++
//   y_attributes_combo_->invalidate();
//   y_attributes_combo_->setVisible(false);

  color_ramp_menu_ = new LiteMenuSimple(label_font_);
  color_ramp_combo_ = new LiteComboBox(color_ramp_menu_);
  controls_->addChild(create_label("Color Ramp:", label_font_));
  controls_->addChild(color_ramp_combo_);
  color_ramp_menu_->addBoundedRangeObserver(this);
  color_ramp_menu_->addBeginEndObserver(this);
  current_ramp_ = ramp_categorical1;

  color_bargraph_ = new BarGraph(100);
  color_range_slider_ = new LiteRangeSliderGraph(&color_range_,
						 Box(100, 0, 200, 10),
						 color_bargraph_,
						 left_to_right);
  controls_->addChild(create_label("Color Range:", label_font_));
  controls_->addChild(color_range_slider_);
  color_range_.addBoundedRangeObserver(this);
  color_range_slider_->addBeginEndObserver(this);
  
  color_attributes_menu_ = new LiteMenuSimple(label_font_);
  color_attributes_combo_ = new LiteComboBox(color_attributes_menu_);
  controls_->addChild(create_label("Color Attribute:", label_font_));
  controls_->addChild(color_attributes_combo_);
  color_attributes_menu_->addBoundedRangeObserver(this);
  color_attributes_menu_->addBeginEndObserver(this);

  weight_attributes_menu_ = new LiteMenuSimple(label_font_);
  weight_attributes_combo_ = new LiteComboBox(weight_attributes_menu_);
  controls_->addChild(create_label("Size Attribute:", label_font_));
  controls_->addChild(weight_attributes_combo_);
  weight_attributes_menu_->addBoundedRangeObserver(this);
  weight_attributes_menu_->addBeginEndObserver(this);

  layout_menu_ = new LiteMenuSimple(label_font_);
  fill_layout_menu();
  layout_combo_ = new LiteComboBox(layout_menu_);
  current_layout_ = LiteTreemap::layout_squarified;
  controls_->addChild(create_label("Layout:", label_font_));
  controls_->addChild(layout_combo_);
  layout_menu_->addBoundedRangeObserver(this);
  layout_menu_->addBeginEndObserver(this);

  create_menus();
  LiteMover * mover = new LiteMover(new LiteRect(Box(0, 0, 250, 20),
						 color_half_grey),
				    this);
  box_->addChild(mover);
  tab_->addTab("Queries", queries_);
  tab_->addTab("Controls", controls_);
  setLite(box_);
  invalidate();
  mover->setSize(Vector(width(getBounds()), 20));
}

Lite *
ControlsTab::clone() const
{
  return 0;
}


Interactor *
ControlsTab::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id)
    return this;
  return 0;
}

void
ControlsTab::doRender(const RenderContext& rc) 
{
  LiteBackground::doRender(rc);
  if (! isEntered()) {
    set_color(color_white);
    glLineWidth(3);
    stroke_box(getBounds());
    set_color(color_black);
    glLineWidth(1);
    stroke_box(getBounds());
  }
}

void
ControlsTab::create_numeric(const FloatColumn * col)
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
  if (col->has_metadata(metadata::user_type) &&
      col->get_metadata(metadata::user_type) == metadata::user_type_unix_time) {
    slider->setLabelFormater(&date_formater);
  }
  queries_->addChild(create_label(col->get_name()+":", label_font_));
  queries_->addChild(slider);
  slider_.push_back(slider);
  slider->addBeginEndObserver(this);
  slider->addBeginEndObserver(treemap_);
  bounds->addBoundedRangeObserver(this);
  column_.push_back(col);
  applyFilters(column_.size()-1);
}

void
ControlsTab::create_string(const StringColumn * col)
{
#if 0
  DefaultBoundedRangeObservable * bounds =
    new DefaultBoundedRangeObservable(0, col->size(),
				      0, col->size());  
  LiteRangeSlider * slider = new LiteRangeSlider(Box(100, 0, 200, 10),
						 left_to_right,
						 color_black,
						 color_none);
  queries_->addChild(create_label(col->get_name()+":", label_font_));
  queries_->addChild(slider);
  slider_.push_back(slider);
  slider->addBeginEndObserver(this);
  slider->addBeginEndObserver(treemap_);
  bounds->addBoundedRangeObserver(this);
  column_.push_back(col);
#endif
}

bool
ControlsTab::doEnter(const Event&)
{
  enableAllControls();
  treemap_->disableDynamicLabels(true);
  repaint();
  return true;
}

void
ControlsTab::doLeave(const Event&)
{
  disableControls_Except(0);
  treemap_->enableDynamicLabels();
  repaint();
}

inline bool
in_range(float val, float min, float max)
{
  return val >= min && val <= max;
}

void
ControlsTab::filter(int index)
{
  const FloatColumn * col = FloatColumn::cast(column_[index]);
  const BoundedRange * range = slider_[index]->getObservable()->getBoundedRange();

  if (col == nullptr)
    return;
  int time = LiteWindow::time();
  float min = range->value();
  float max = min + range->range();
//   if (min <= range->min() && max >= range->max())	\
//     return;

#ifdef PRINT
  std::cerr << "Filtering " + col->get_name() +
    " in [" << min << ", " << max << "]\n";
#endif

  // special case for depth change
  if (col->get_name() == "depth") {
    treemap_->getDrawer().set_max_depth(unsigned(max+0.5f));
  }
  unsigned filtered = 0;
  const unsigned mask = 1 << index;
  const unsigned not_mask = ~mask;

  for (size_t i = 0; i < col->size(); i++) {
    if (!col->defined(i) ||
        !in_range(col->fast_get(i), min, max)) {
      (*filter_)[i] |= mask;
      filtered++;
    }
    else {
      (*filter_)[i] &= not_mask;
    }
  }
#ifdef PRINT
  if (filtered != 0)
    std::cerr << "Filtered " << filtered << " items\n";
#endif
  time = LiteWindow::time() - time;
#ifdef PRINT
  std::cerr << "Filter Time: "
	    << float(time / 1000.0f) << "s for "
	    << col->size() << " items ="
	    << float(col->size() * 1000.0f / time) << "items/s\n";
#endif
}

void
ControlsTab::applyFilters(unsigned index)
{
  filter(index);
}


  // BoundedRangeObserver
void
ControlsTab::valueDragged(BoundedRangeObservable * obs)
{
  int i;
  for (i = 0; i < slider_.size(); i++) {
    if (slider_[i]->getObservable() == obs) {
      applyFilters(i);
      treemap_->updateMinMax();
      return;
    }
  }
}

void
ControlsTab::rangeDragged(BoundedRangeObservable * obs)
{
  valueDragged(obs);
}
  
void
ControlsTab::updateBoundedRange(BoundedRangeObservable * obs)
{
  int sel;

  if (obs == weight_attributes_menu_) {
    sel = weight_attributes_menu_->getSelected();
    if (sel < 0)
      return;

    setWeightAttribute(weight_attributes_menu_->getItem(sel));
  }
  else if (obs == x_attributes_menu_) {
    sel = x_attributes_menu_->getSelected();
    if (sel < 0)
      return;

    setXAttribute(x_attributes_menu_->getItem(sel));
  }
  else if (obs == y_attributes_menu_) {
    sel = y_attributes_menu_->getSelected();
    if (sel < 0)
      return;

    setYAttribute(y_attributes_menu_->getItem(sel));
  }
  else if (obs == color_attributes_menu_) {
    sel = color_attributes_menu_->getSelected();
    if (sel < 0)
      return;

    setColorAttribute(color_attributes_menu_->getItem(sel));
  }
  else if (obs == color_ramp_menu_) {
    sel = color_ramp_menu_->getSelected();
    if (sel < 0 || sel > ramp_max)
      return;
    setRamp(Ramp(sel));
  }
  else if (obs == &color_range_) {
    set_color_range(color_range_.value(), color_range_.range());
  }
  else if (obs == sort_by_menu_) {
    sel = sort_by_menu_->getSelected();
    if (sel < 0)
      return;
    const string& order = sort_by_menu_->getItem(sel);
    sortBy(order);
  }
  else if (obs == layout_menu_) {
    sel = layout_menu_->getSelected();
    if (sel < 0 || sel >= LiteTreemap::layout_max)
      return;
    setLayout(LiteTreemap::Layout(sel));
  }
  else if (obs == &recorder_) {
    replay(recorder_.current());
  }
}

static void
set_children_visible(Lite * l, bool v)
{
  for (size_t i = 0; i < l->childCount(); i++) {
    Lite* child = l->getChild(i);
    child->setVisible(v);
    if (v) {
      LiteSlider* slider = dynamic_cast<LiteSlider*>(child);
      if (slider != nullptr)
        slider->setFont(nullptr);
    }
  }
}

static bool
set_label_visible(Lite * container, Lite * l)
{
  for (size_t i = 0; i < container->childCount(); i++) {
    Lite* child = container->getChild(i);
    if (child == l) {
      child->setVisible(true);
      if (i != 0) {
        child = container->getChild(i-1);
        child->setVisible(true);
      }
      return true;
    }
  }
  return false;
}

void
ControlsTab::disableControls_Except(BeginEndObservable * obs)
{
  set_children_visible(box_, false);
  tab_->setVisible(true);	// repair
  tab_->getChild(1)->setVisible(false);	// remove the buttons
  set_children_visible(queries_, false);
  set_children_visible(controls_, false);
  setColor(color_none);
  /*
  if (obs == border_slider_) {
  }
  else if (obs == blend_slider_) {
  }
  else if (obs == transparency_slider_) {
  }
  else if (obs == plot_range_slider_) {
  }
  else
  */
  if (obs == weight_attributes_menu_) {
    set_label_visible(controls_,weight_attributes_combo_);
  }
  else if (obs == x_attributes_menu_) {
    set_label_visible(controls_, x_attributes_combo_);
  }
  else if (obs == y_attributes_menu_) {
    set_label_visible(controls_, y_attributes_combo_);
  }
  else if (obs == sort_by_menu_) {
    set_label_visible(controls_, sort_by_combo_);
  }
  else if (obs == color_attributes_menu_) {
    set_label_visible(controls_, color_attributes_combo_);
  }
  else if (obs == color_ramp_menu_) {
    set_label_visible(controls_, color_ramp_combo_);
  }
  else if (obs == layout_menu_) {
    set_label_visible(controls_, layout_combo_);
  }
  else {
    LiteSlider * l = dynamic_cast<LiteSlider*>(obs);
    if (l != 0) {
      if (! set_label_visible(controls_, l))
	set_label_visible(queries_, l);
      l->setFont(label_font_);
    }
  }
}

void
ControlsTab::enableAllControls()
{
  set_children_visible(box_, true);
  tab_->getChild(1)->setVisible(true);	// reset the buttons
  set_children_visible(queries_, true);
  set_children_visible(controls_, true);
  setColor(color_white);
}

  // BeginEndObserver
void
ControlsTab::begin(BeginEndObservable * obs)
{
  disableControls_Except(obs);
}

void
ControlsTab::end(BeginEndObservable *)
{
  enableAllControls();
}

void
ControlsTab::create_menus()
{
  fill_weight_attributes_menu();
  fill_x_attributes_menu();
  fill_y_attributes_menu();
  fill_color_attributes_menu();
  fill_sort_by_menu();
  fill_color_ramp_menu();
}

void
ControlsTab::fill_weight_attributes_menu()
{
  weight_attributes_menu_->removeAll();
  for (auto n = tree_.begin_names(); n != tree_.end_names(); ++n) {
    const string& prop = *n;
    if (prop[0] == '$')
      continue;
    FloatColumn* flt = FloatColumn::cast(tree_.find_column(prop));
    if (flt != nullptr) {
      if (flt->get_metadata(metadata::aggregate) != metadata::aggregate_sum)
        weight_attributes_menu_->addItem("*"+prop);
      else
        weight_attributes_menu_->addItem(prop);
    }
  }
}

void
ControlsTab::setWeightAttribute(const string& prop)
{
  //if (prop == treemap_->getWeightProp()) return;
  string prop_name;
  if (prop[0] == '*')
    prop_name = prop.substr(1);
  else
    prop_name = prop;
  if (tree_.find_column(prop_name) == nullptr)
    return;
  for (size_t i = 0; i < weight_attributes_menu_->childCount(); i++) {
    if (prop == weight_attributes_menu_->getItem(i)) {
      weight_attributes_combo_->setSelectedMenuItem(static_cast<int>(i));
      break;
    }
  }
  treemap_->setWeightProp(prop_name);
  invalidate();
  treemap_->enableSaveUnder();
}

void
ControlsTab::fill_x_attributes_menu()
{
  x_attributes_menu_->removeAll();

  for (auto n = tree_.begin_names(); n != tree_.end_names(); ++n) {
    const string& prop = *n;
    if (prop[0] == '$')
      continue;
    FloatColumn* flt = FloatColumn::cast(tree_.find_column(prop));
    if (flt != nullptr) {
      //std::cerr << "adding " << prop << " to x_attributes_menu_\n";
      x_attributes_menu_->addItem(prop);
    }
  }
  x_attributes_combo_->setSelectedMenuItem(0);
}

void
ControlsTab::setXAttribute(const string& prop)
{
//   if (prop == treemap_->getXAxisProp())
//     return;
  //std::cerr << "setting x_attribute to " << prop << std::endl;
  for (size_t i = 0; i < x_attributes_menu_->childCount(); i++) {
    if (prop == x_attributes_menu_->getItem(i)) {
      x_attributes_combo_->setSelectedMenuItem(static_cast<int>(i));
      //std::cerr << "x_attribute found at index " << i << std::endl;
      break;
    }
  }
  treemap_->setXAxisProp(prop);
  invalidate();
  treemap_->enableSaveUnder();
  repaint();
}

void
ControlsTab::fill_y_attributes_menu()
{
  y_attributes_menu_->removeAll();

  for (auto n = tree_.begin_names(); n != tree_.end_names(); ++n) {
    const string& prop = *n;
    if (prop[0] == '$')
      continue;
    FloatColumn* flt = FloatColumn::cast(tree_.find_column(prop));
    if (flt != nullptr) {
      //std::cerr << "adding " << prop << " to y_attributes_menu_\n";
      y_attributes_menu_->addItem(prop);
    }
  }
  y_attributes_combo_->setSelectedMenuItem(0);
}

void
ControlsTab::setYAttribute(const string& prop)
{
//   if (prop == treemap_->getYAxisProp())
//     return;
  //std::cerr << "setting y_attribute to " << prop << std::endl;
  for (size_t i = 0; i < y_attributes_menu_->childCount(); i++) {
    if (prop == y_attributes_menu_->getItem(i)) {
      y_attributes_combo_->setSelectedMenuItem(static_cast<int>(i));
      //std::cerr << "y_attribute found at index " << i << std::endl;
      break;
    }
  }
  treemap_->setYAxisProp(prop);
  invalidate();
  treemap_->enableSaveUnder();
  repaint();
}

void
ControlsTab::fill_color_attributes_menu()
{
  color_attributes_menu_->removeAll();
  for (auto n = tree_.begin_names(); n != tree_.end_names(); ++n) {
    const string& prop = *n;
    if (prop[0] == '$')
      continue;
    FloatColumn* flt = FloatColumn::cast(tree_.find_column(prop));
    if (flt == nullptr)
      continue;
    color_attributes_menu_->addItem(prop);
    if (prop == treemap_->getColorProp())
      color_attributes_menu_->setSelected(color_attributes_menu_->childCount()-1);
  }
}
void
ControlsTab::setColorAttribute(const string& prop)
{
  if (tree_.find_column(prop) == nullptr)
    return;
  for (size_t i = 0; i < color_attributes_menu_->childCount(); i++) {
    if (prop == color_attributes_menu_->getItem(i)) {
      color_attributes_combo_->setSelectedMenuItem(static_cast<int>(i));
      break;
    }
  }
  float min_val, max_val;
  const FloatColumn& values = *FloatColumn::cast(tree_.find_column(prop));
  min_val = values.min();
  max_val = values.max();
  color_bargraph_->computeDistribution(values, min_val, max_val);
#ifdef PRINT
  std::cerr << "For color attribute " << prop
	    << " min=" << min_val
	    << ", max=" << max_val << std::endl;
#endif
  color_range_.set_min(min_val);
  color_range_.set_max(max_val);
  set_color_range(min_val, max_val - min_val);
  treemap_->setColorProp(prop);
  invalidate();
  treemap_->enableSaveUnder();
  repaint();
}

void
ControlsTab::fill_color_ramp_menu()
{
  color_ramp_menu_->removeAll();
  color_ramp_menu_->addItem("Categorical1");
  color_ramp_menu_->addItem("Categorical2");
  color_ramp_menu_->setSelected(0);
  color_ramp_menu_->addItem("Sequential1");
  color_ramp_menu_->addItem("Sequential2");
}
void
ControlsTab::setRamp(Ramp r)
{
  if (r == current_ramp_) return;
  color_ramp_combo_->setSelectedMenuItem(r);
  current_ramp_ = r;
  treemap_->getDrawer().set_color_ramp(getRamp(r));
  treemap_->enableSaveUnder();
  repaint();
}

void
ControlsTab::fill_color_range_slider()
{
  float min, range;
  treemap_->getDrawer().get_color_range(min, range);
  color_range_.set_range(range);
  color_range_.set_value(min);
}
void
ControlsTab::set_color_range(float min, float range)
{
  color_range_.set_range(range);
  color_range_.set_value(min);
  treemap_->getDrawer().set_color_range(min, range);
  treemap_->enableSaveUnder();
  repaint();
}

void
ControlsTab::fill_sort_by_menu()
{
  sort_by_menu_->addItem("original");
  for (auto n = tree_.begin_names(); n != tree_.end_names(); ++n) {
    const string& prop = *n;
    if (FloatColumn::cast(tree_.find_column(prop)) == nullptr)
      continue;
    sort_by_menu_->addItem("<"+prop);
    sort_by_menu_->addItem(">"+prop);
  }
}

struct less_weight {
  const WeightMap& weight;

  less_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] < weight[n2];
  }
};

struct greater_weight {
  const WeightMap& weight;

  greater_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] > weight[n2];
  }
};

struct compare_order {
  bool operator() (node_descriptor n1, node_descriptor n2) const {
    return n1 < n2;
  }
};

void
ControlsTab::sortBy(const string& order)
{
  if (order == current_sort_by_) return;
  current_sort_by_ = order;
  if (order == "original") {
    sort(tree_, compare_order());
  }
  else {
    if (order[0] == '<')
      sort(tree_, less_weight(*FloatColumn::find(order.substr(1), tree_)));
    else if (order[0] == '>')
      sort(tree_, greater_weight(*FloatColumn::find(order.substr(1), tree_)));
    else
      sort(tree_, greater_weight(*FloatColumn::find(order, tree_)));
  }
  for (size_t i = 0; i < sort_by_menu_->childCount(); i++) {
    if (order == sort_by_menu_->getItem(i)) {
      sort_by_combo_->setSelectedMenuItem(static_cast<int>(i));
      break;
    }
  }
  invalidate();
  treemap_->enableSaveUnder();
  repaint();
}

void
ControlsTab::fill_layout_menu()
{
  layout_menu_->addItem("squarified");
  layout_menu_->addItem("strip");
  layout_menu_->addItem("slice&dice");
  layout_menu_->addItem("scatter plot");
}

void
ControlsTab::setLayout(LiteTreemap::Layout l)
{
  if (l == current_layout_) return;
  current_layout_ = l;
  switch(l) {
  case LiteTreemap::layout_squarified:
    treemap_->setStrip(false);
    treemap_->setLayout(LiteTreemap::layout_squarified);
//     x_attributes_combo_->setVisible(false);
//     y_attributes_combo_->setVisible(false);
    break;
  case LiteTreemap::layout_strip:
    treemap_->setStrip(true);
    treemap_->setLayout(LiteTreemap::layout_strip);
//     x_attributes_combo_->setVisible(false);
//     y_attributes_combo_->setVisible(false);
    break;
  case LiteTreemap::layout_slice_and_dice:
    treemap_->setLayout(LiteTreemap::layout_slice_and_dice);
//     x_attributes_combo_->setVisible(false);
//     y_attributes_combo_->setVisible(false);
    break;
  case LiteTreemap::layout_scatter_plot:
    treemap_->setLayout(LiteTreemap::layout_scatter_plot);
//     x_attributes_combo_->setVisible(true);
//     y_attributes_combo_->setVisible(true);
    break;
  default:
    return;
  }
  layout_combo_->setSelectedMenuItem(int(l));
  repaint();
}


void
ControlsTab::record(RecordItem& item)
{
  item.weight = treemap_->getWeightProp();
  item.color = treemap_->getColorProp();
  item.name = treemap_->getNameProp();
  item.ramp = current_ramp_;
  item.color_smooth = treemap_->getDrawer().get_color_smooth();
  treemap_->getDrawer().get_color_range(item.color_min, item.color_range);
  item.order = current_sort_by_;
  item.layout = current_layout_;
  item.x_axis = treemap_->getXAxisProp();
  item.y_axis = treemap_->getYAxisProp();
}

void
ControlsTab::record()
{
  //std::cerr << "Recording\n";
  RecordItem item;
  record(item);
  recorder_.add(item);
}

void
ControlsTab::replaySimple(const RecordItem& item)
{
  setWeightAttribute(item.weight);
  setColorAttribute(item.color);
  setRamp(item.ramp);
  set_color_range(item.color_min, item.color_range);
  sortBy(item.order);
  setLayout(item.layout);
  setXAttribute(item.x_axis);
  setYAttribute(item.y_axis);
  repaint();
}

bool
ControlsTab::replayOneSquarified(const RecordItem& cur, const RecordItem& target)
{
  if (target.layout == cur.layout &&
      target.order == cur.order &&
      target.weight == cur.weight) {
    replaySimple(target);
    return true;
  }

  if (target.layout == LiteTreemap::layout_squarified ||
      target.layout == LiteTreemap::layout_strip) {
    int max_depth = 5;

    animate_.clear();
    treemap_->computeBoxList(0.0f, max_depth, animate_.getStartList());
    animate_.computeTexCoords(treemap_->getTextureSize());
    setWeightAttribute(target.weight);
    sortBy(target.order);
    setLayout(target.layout);

    animate_.setTexture(treemap_->getTexture());
    if (cur.weight != target.weight) {
      treemap_->computeBoxList(1.0f, max_depth, animate_.getEndList());
      treemap_->setAnimate(new AnimateCompose(&animate_,
					      new AnimateVisu(treemap_->visu_,
							      this)
					      ));
      if (! treemap_->isAnimating())
	treemap_->beginAnimation(int(1000*animation_duration_.value()));
    }
    else {
      treemap_->computeBoxList(0.0f, max_depth, animate_.getEndList());
      treemap_->setAnimate(&animate_);
      if (! treemap_->isAnimating())
	treemap_->beginAnimation(int(1000*animation_duration_.value()));
    }
    return false;
  }
  if (target.layout != cur.layout) {
    int max_depth = 100;
    animate_.clear();
    treemap_->computeBoxList(0.0f, max_depth, animate_.getStartList());
    sortBy(target.order);
    setLayout(target.layout);
    setXAttribute(target.x_axis);
    setYAttribute(target.y_axis);
    treemap_->computeBoxList(0.0f, max_depth, animate_.getEndList());
    treemap_->setAnimate(&animate_);
    if (! treemap_->isAnimating())
      treemap_->beginAnimation(int(1000*animation_duration_.value()));
    return false;
  }
  replaySimple(target);
  return false;
}

bool
ControlsTab::replayOneStrip(const RecordItem& cur, const RecordItem& target)
{
  replayOneSquarified(cur, target);
  return true;
}

bool
ControlsTab::replayOneSliceAndDice(const RecordItem& cur, const RecordItem& target)
{
  if (target.layout == cur.layout &&
      target.order == cur.order &&
      target.weight == cur.weight) {
    replaySimple(target);
    return true;
  }

  if (target.layout == cur.layout &&
      cur.weight != target.weight) {
    setWeightAttribute(target.weight);
    treemap_->setAnimate(new AnimateVisu(treemap_->visu_, this));
    if (! treemap_->isAnimating())
      treemap_->beginAnimation(int(1000*animation_duration_.value()));
    return false;
  }
  if (target.layout != cur.layout) {
    int max_depth = 100;
    animate_.clear();
    treemap_->computeBoxList(0.0f, max_depth, animate_.getStartList());
    sortBy(target.order);
    setLayout(target.layout);
    setXAttribute(target.x_axis);
    setYAttribute(target.y_axis);
    treemap_->computeBoxList(0.0f, max_depth, animate_.getEndList());
    treemap_->setAnimate(&animate_);
    if (! treemap_->isAnimating())
      treemap_->beginAnimation(int(1000*animation_duration_.value()));
    return false;
  }
  replaySimple(target);
  return false;
}

bool
ControlsTab::replayOneScatterPlot(const RecordItem& cur, const RecordItem& target)
{
  if (target.layout == cur.layout &&
      target.order == cur.order &&
      target.x_axis == cur.x_axis &&
      target.y_axis == cur.y_axis) {
    replaySimple(target);
    return true;
  }

  if (target.layout != cur.layout ||
      target.order != cur.order ||
      target.x_axis != cur.x_axis ||
      target.y_axis != cur.y_axis) {
    int max_depth = 100;
    animate_.clear();
    treemap_->computeBoxList(0.0f, max_depth, animate_.getStartList());
    sortBy(target.order);
    setLayout(target.layout);
    setXAttribute(target.x_axis);
    setYAttribute(target.y_axis);
    treemap_->computeBoxList(0.0f, max_depth, animate_.getEndList());
    treemap_->setAnimate(&animate_);
    if (! treemap_->isAnimating())
      treemap_->beginAnimation(int(1000*animation_duration_.value()));
    return false;
  }

  replaySimple(target);
  return true;
}

bool
ControlsTab::replayOne(const RecordItem& target)
{
  RecordItem current;
  record(current);
  if (current == target)
    return true;		// done
  switch (current.layout) {
  case LiteTreemap::layout_squarified:
    return replayOneSquarified(current, target);
  case LiteTreemap::layout_strip:
    return replayOneStrip(current, target);
  case LiteTreemap::layout_slice_and_dice:
    return replayOneSliceAndDice(current, target);
  case LiteTreemap::layout_scatter_plot:
    return replayOneScatterPlot(current, target);
  }
  replaySimple(target);
  return true;		// stop anyway
}

void
ControlsTab::replayCurrent()
{
  replay(recorder_.current());
}

void
ControlsTab::replay(const RecordItem& item)
{
  replayOne(item);
  return;
}

void
AnimateVisu::render(float param)
{
  if (param >= 1.0f)
    win_->replaySimple(win_->recorder_.current());
  else
    visu_->draw(1.0f-param);
}

void
AnimateContinue::render(float param)
{
  win_->replayCurrent();
}
    

} // namespace infovis

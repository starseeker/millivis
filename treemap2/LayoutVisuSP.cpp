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
#include <LayoutVisuSP.hpp>
#include <iostream>

namespace infovis {

LayoutVisuScatterPlot::LayoutVisuScatterPlot(LiteTreemap * tm)
  : LayoutVisu(tm),
    cached_x_axis_(0),
    cached_y_axis_(0),
    cached_color_axis_(0),
    cached_weight_(0),
    cached_float_range_(0),
    cached_vetices_(0),
    cached_vetices_size_(0)
{ }

template <class DRAWER>
static void
renderScatterPlotBackward(LiteTreemap * tm_, DRAWER& drawer)
{
  float plot_range = tm_->plot_range_.getBoundedRange()->range();
  float plot_min  = tm_->plot_range_.getBoundedRange()->value();
  float plot_max = plot_min + plot_range;
  FilterColumn * filter =
    FilterColumn::cast(tm_->tree_.find_column("$filter"));

  Box bounds(tm_->getBounds());
  float x_scale = tm_->x_axis_max_ - tm_->x_axis_min_;
  if (x_scale == 0)
    x_scale = 1;
  float y_scale = tm_->y_axis_max_ - tm_->y_axis_min_;
  if (y_scale == 0)
    y_scale = 1;
      
  x_scale = (width(bounds)-plot_max*2) / x_scale;
  y_scale = (height(bounds)-plot_max*2) / y_scale;

  float weight_scale = (plot_range-1) / (tm_->weight_max_ - tm_->weight_min_);

  for (int n = tm_->x_axis_->size()-1; n >= 0; n--) {
    if (filter->fast_get(n) != 0)
      continue;
    Point p(((*tm_->x_axis_)[n] - tm_->x_axis_min_) * x_scale +
	    plot_range + xmin(bounds),
	    ((*tm_->y_axis_)[n] - tm_->y_axis_min_) * y_scale +
	    plot_range + ymin(bounds));
    float w = 1.0f+((*tm_->weight_)[n] - tm_->weight_min_) * weight_scale + plot_min;
    Box b(x(p)-w,y(p)-w, x(p)+w,y(p)+w);
    drawer.draw_box(b, n, 1);
  }
}

template <class DRAWER>
static void
renderScatterPlotForward(LiteTreemap * tm_, DRAWER& drawer)
{
  float plot_range = tm_->plot_range_.getBoundedRange()->range();
  float plot_min  = tm_->plot_range_.getBoundedRange()->value();
  float plot_max = plot_min + plot_range;
  FilterColumn * filter =
    FilterColumn::cast(tm_->tree_.find_column("$filter"));

  Box bounds(tm_->getBounds());
  float x_scale = tm_->x_axis_max_ - tm_->x_axis_min_;
  if (x_scale == 0)
    x_scale = 1;
  float y_scale = tm_->y_axis_max_ - tm_->y_axis_min_;
  if (y_scale == 0)
    y_scale = 1;
      
  x_scale = (width(bounds)-plot_max*2) / x_scale;
  y_scale = (height(bounds)-plot_max*2) / y_scale;

  float weight_scale = (plot_range-1) / (tm_->weight_max_ - tm_->weight_min_);

  for (int n = 0; n < tm_->x_axis_->size(); n++) {
    if (filter->fast_get(n) != 0)
      continue;
    Point p(((*tm_->x_axis_)[n] - tm_->x_axis_min_) * x_scale +
	    plot_range + xmin(bounds),
	    ((*tm_->y_axis_)[n] - tm_->y_axis_min_) * y_scale +
	    plot_range + ymin(bounds));
    float w = 1.0f+((*tm_->weight_)[n] - tm_->weight_min_) * weight_scale + plot_min;
    Box b(x(p)-w,y(p)-w, x(p)+w,y(p)+w);
    drawer.draw_box(b, n, 1);
  }
}


struct VertexInfo {
  float x_, y_, z_, tex_;
  VertexInfo() { }
  VertexInfo(float x, float y, float z, float t)
    : x_(x), y_(y), z_(z), tex_(t) { }
};

using VertexTable = std::vector<VertexInfo>;

void
LayoutVisuScatterPlot::renderFastScatterPlot(float param)
{
#if 0
  glPushAttrib(GL_COLOR_BUFFER_BIT
	       | GL_FOG_BIT 
	       | GL_STENCIL_BUFFER_BIT
	       );

  glShadeModel(GL_SMOOTH);
  glPushMatrix();
  glScalef(1, 1, -1);
  tm_->drawer_.start();
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_INCR, GL_INCR);

  float trans = (1.0-(tm_->transparency_.getBoundedRange()->value()/100));
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glStencilFunc(GL_GREATER, 100, (unsigned int)(-1));
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glStencilFunc(GL_ALWAYS, 1, (unsigned int)(-1));

  int plot_range = tm_->plot_range_.getBoundedRange()->value();
  Box bounds(tm_->getBounds());

  float weight_scale = max_rect_range / (weight_max_ - weight_min_);
  std::vector<VertexTable> table_by_size;
  table_by_size.resize(int(max_plot_size));

  for (int n = 0; n < x_axis_->size(); n++) {
    Point p((*x_axis_)[n], (*y_axis_)[n]);
    float w = ((*weight_)[n] - weight_min_) * weight_scale;
    table_by_size[int(w)].push_back(VertexInfo(x(p), y(p), n+1,
					       drawer_.compute_color((*type_)[n])));
  }
  for (int i = 0; i < table_by_size.size(); i++) {
    VertexTable& vt = table_by_size[i];
    if (vt.size() != 0) {
      glPointSize(i+1);
      glVertexPointer(3, GL_FLOAT, sizeof(VertexInfo),
		      &(*vt.begin()));
      glTexCoordPointer(1, GL_FLOAT, sizeof(VertexInfo),
			&((*vt.begin()).tex_));
      glDrawArrays(GL_POINTS, 0, vt.size());
    }
  }
  glPointSize(1);
  tm_->drawer_.finish();
  glPopMatrix();
  glPopAttrib();
#endif
}

unsigned
LayoutVisuScatterPlot::draw(float param)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT
	       | GL_FOG_BIT 
	       | GL_STENCIL_BUFFER_BIT
	       );

  glShadeModel(GL_SMOOTH);
  glPushMatrix();
  glScalef(1, 1, -1);
  tm_->drawer_.start();
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_INCR, GL_INCR);

  glFogi(GL_FOG_START, 0);
  glFogi(GL_FOG_END, 3);

  float trans = (1.0-(tm_->transparency_.getBoundedRange()->value()/100));
  glColor4f(1.0f, 1.0f, 1.0f, trans);
  if (trans != 1.0f) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glStencilFunc(GL_GREATER, 100, (unsigned int)(-1));
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glStencilFunc(GL_ALWAYS, 1, (unsigned int)(-1));
    renderScatterPlotForward(tm_, tm_->drawer_);
  }
  else {
    glStencilFunc(GL_GREATER, 1, (unsigned int)(-1));
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    renderScatterPlotBackward(tm_, tm_->drawer_);
  }
  tm_->drawer_.finish();
  glPopMatrix();
  glPopAttrib();
  return (*tm_->x_axis_).size();
}

static void
push_parents(FastPicker& picker_,
	     const Box& b,
	     node_descriptor n,
	     const Tree& tree_)
{
  if (n == root(tree_))
    return;
  push_parents(picker_, b, parent(n, tree_), tree_);
  picker_.enter(b, n, 0);
}

struct ScatterPlotPicker
{
  Picker& picker_;
  Point pos_;
  int hit_count_;

  ScatterPlotPicker(Picker& p)
    : picker_(p),
      pos_(p.get_x_pos(), p.get_y_pos()),
      hit_count_(0)
  { }

  void draw_box(const Box& b,
		node_descriptor n,
		unsigned depth) {
    if (infovis::inside(b, x(pos_), y(pos_))) {
//       if (hit_count_ == 0) {
// 	push_parents(picker_, b, n, picker_.get_tree());
//       }
      picker_.add_label(Point(xmin(b) + width(b)/2,
			      ymin(b) + height(b)/2), n);
    }
  }
};

unsigned
LayoutVisuScatterPlot::pick(float param)
{
  int plot_range = int(tm_->plot_range_.getBoundedRange()->value());
  ScatterPlotPicker picker(tm_->picker_);

  tm_->picker_.start();
  renderScatterPlotBackward(tm_, picker);

  Point& pos_ = picker.pos_;
  tm_->picker_.set_labels_clip(Box(x(pos_)-plot_range, y(pos_)-plot_range,
				   x(pos_)+plot_range, y(pos_)+plot_range));

#ifdef VECTOR_AS_TREE
  Tree::string_prop& name = tm_->tree_.get_prop_string(tm_->name_prop_);
#else
  StringColumn& name = *StringColumn::find(tm_->name_prop_, tm_->tree_);
#endif
  float line_alpha = tm_->menu_.getLeftRight().getBoundedRange()->value();
  tm_->picker_.finish(tm_->getBounds(),
		      tm_->label_font_,
		      line_alpha,
		      name);

  return (*tm_->x_axis_).size();
}

void
LayoutVisuScatterPlot::boxlist(float param,
			       AnimateTree::BoxList& bl, int depth)
{
  BoxDrawer drawer(tm_->drawer_, bl);
  drawer.set_depth(depth);
  drawer.start();
  renderScatterPlotBackward(tm_, drawer);
  drawer.finish();
}

} // namespace infovis

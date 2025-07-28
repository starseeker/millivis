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
#include <LitePath.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LitePath::LitePath(node_descriptor node,
		   const Tree& tree,
		   const StringColumn& name,
		   Font * font)
  : node_(node),
    tree_(tree),
    name_(name),
    font_(font),
    depth_(node_depth(node_, tree_)),
    selected_(-1),
    size_(0,0),
    //was_inside_(true),
    was_inside_(false),
    was_visible_(false),
    reference_(0),
    border_(5),
    left_right_(0, 1, 0)
{ }

float
LitePath::min() const { return 0; }
float
LitePath::max() const { return depth_; }
float
LitePath::value() const { return selected_; }
float
LitePath::range() const { return 0; }

const BoundedRange *
LitePath::getBoundedRange() const { return this; }

void
LitePath::setReference(int i)
{
  set_y(position_, y(position_) + reference_ * font_->getHeight());
  reference_ = i;
  set_y(position_, y(position_) - reference_ * font_->getHeight());
}

void
LitePath::setPath(node_descriptor n)
{
  node_ = n;
  depth_ = node_depth(node_, tree_);
  updateBounds();
}

void
LitePath::setVisible(bool b)
{
  bool was_visible = isVisible();
  Lite::setVisible(b);
  if (! was_visible && isVisible()) {
    updateBounds();
  }
}

Box
LitePath::getBounds() const
{
  return border_.growBox(getInBounds());
}

Box
LitePath::getInBounds() const
{
  return Box(x(position_),
	     y(position_),
	     x(position_) + dx(size_),
	     y(position_) + dy(size_));
}

void
LitePath::setBounds(const Box& b)
{
  Box inBounds(border_.shrinkBox(b));
  position_ = Point(infovis::xmin(inBounds),
		    infovis::ymax(inBounds));
}

Lite *
LitePath::clone() const
{
  return new LitePath(node_, tree_, name_, font_);
}

node_descriptor
LitePath::getSelectedRoot() const
{
  if (selected_ < 0 || selected_ > depth_)
    return node_descriptor(-1);
  int sel = depth_ - selected_;
  node_descriptor n = node_;
  while (sel-- != 0)
    n = parent(n, tree_);
  return n;
}

Interactor *
LitePath::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id)
    return this;
  return 0;
}

void
LitePath::moveSmart(const Point& pos)
{
  Box b(getBounds());
  if (infovis::inside(b, x(pos), y(pos)))
    return;
  if (x(pos) < (xmin(b))) {
    set_x(position_, x(position_)+ (x(pos) - xmin(b)));
  }
  else if (x(pos) > xmax(b)) {
    set_x(position_, x(position_) + (x(pos) - xmax(b)));
  }
  if (y(pos) < ymin(b)) {
    set_y(position_, y(position_) + (y(pos) - ymin(b)));
  }
  else if (y(pos) > ymax(b)) {
    set_y(position_, y(position_) + (y(pos) - ymax(b)));
  }
  repaint();
}

int
LitePath::selectedFromPos(const Point& pos)
{
  Box b(getInBounds());
  if (infovis::inside(b, x(pos), y(pos))) {
    was_inside_ = true;
    left_right_.notifyValueDragged((x(pos) - xmin(b)) / width(b));
    return int((y(pos) - ymin(b)) / font_->getHeight());
  }
  if (! was_inside_)
    return -1;
  moveSmart(pos);
  b = getInBounds();
  if (infovis::inside(b, x(pos), y(pos)))
    return int((y(pos) - ymin(b)) / font_->getHeight());
  return -1;
}

void
LitePath::doMove(const Event& ev)
{
  selected_ = selectedFromPos(getFieldPosition(ev));
}

bool
LitePath::doStart(const Event& ev)
{
  Point pos(getFieldPosition(ev));
  setPosition(pos);
  setVisible(true);
  selected_ = selectedFromPos(pos);
  notifyValueDragged(selected_);
  repaint();
  return true;
}

void
LitePath::doDrag(const Event& ev)
{
  Point pos(getFieldPosition(ev));
  selected_ = selectedFromPos(pos);
  notifyValueDragged(selected_);
  repaint();
}

void
LitePath::doFinish(const Event& ev)
{
  Point pos(getFieldPosition(ev));
  //was_inside_ = false;
  selected_ = selectedFromPos(pos);
  notifyBoundedRange();
  setVisible(false);
  repaint();
}

void
LitePath::doAbort(const Event&)
{
  //was_inside_ = false;
  //selected_ = -1;
  //notifyBoundedRange();
  setVisible(false);
  repaint();
}

void
LitePath::updateBounds()
{
  float w = 0;
  for (node_descriptor n = node_; n != root(tree_); n = parent(n, tree_)) {
    w = std::max(w, font_->stringWidth(name_[n]));
  }
  size_ = Vector(w, font_->getHeight() * (depth_+1));
}

void
LitePath::doRender(const RenderContext& rc)
{
  if (depth_ == 0) {
    return;
  }
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1, 1, 1, 0.9f);
  Box bounds(getInBounds());
  border_.render(rc, bounds);
  draw_box(bounds);
  if (! rc.is_picking) {
    int pos_x = int(xmin(bounds));
    int pos_y = int(ymax(bounds) - font_->getAscent());
    node_descriptor n = node_;
    for (int i = 0; i <= depth_; i++) {
      if (i == (depth_ - selected_))
	set_color(color_red);
      else if (i == (depth_ - reference_))
	set_color(color_blue);
      else
	set_color(color_black);
      font_->paint(name_[n], pos_x, pos_y);
      n = parent(n, tree_);
      pos_y -= int(font_->getHeight());
    }
  }
  glPopAttrib();
}  

} // namespace infovis

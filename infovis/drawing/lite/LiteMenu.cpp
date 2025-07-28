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
#include <infovis/drawing/lite/LiteMenu.hpp>
#include <infovis/drawing/gl_support.hpp>

//#define PRINT
#ifdef PRINT
#include <iostream>
#endif

namespace infovis {

LiteMenu::LiteMenu(const Color& c)
  : color_(c),
    max_size_(0, 0),
    selected_(-1),
    was_visible_(false),
    border_(c)
{ }

LiteMenu::LiteMenu(const Color& c, float max_width, float max_height)
  : color_(c),
    max_size_(max_width, max_height),
    selected_(-1),
    was_visible_(false)
{ }

Box
LiteMenu::getInBounds() const
{
  Box bounds = LiteGroup::getBounds();
  if (dx(max_size_) > 0 &&
      width(bounds) > dx(max_size_)) {
    set_width(bounds, dx(max_size_));
  }
  if (dy(max_size_) > 0 &&
      height(bounds) > dy(max_size_)) {
    set_height(bounds, dy(max_size_));
  }
  return bounds;
}

Box
LiteMenu::getBounds() const
{
  return border_.growBox(getInBounds());
}

Lite *
LiteMenu::clone() const
{
  return new LiteMenu(color_, dx(max_size_), dy(max_size_));
}

Interactor *
LiteMenu::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id)
    return this;
  return 0;
}

// BoundedRange
float
LiteMenu::min() const
{
  return 0;
}

float
LiteMenu::max() const
{
  return childCount();
}

float
LiteMenu::value() const
{
  return selected_;
}

float
LiteMenu::range() const
{
  return 0;
}

// AbstractBoundedRangeObservable
const BoundedRange *
LiteMenu::getBoundedRange() const
{
  return this;
}

  // Interactor3States
void
LiteMenu::doMove(const Event& ev)
{
  selected_ = selectedFromPos(getFieldPosition(ev));
#ifdef PRINT
  std::cerr << " selected = " << selected_ << std::endl;
#endif
}

bool
LiteMenu::doStart(const Event& ev)
{
  Point pos(getFieldPosition(ev));
#ifdef PRINT
  std::cerr << "Starting menu\n";
#endif
  notifyBegin();
  was_visible_ = isVisible();
  if (! was_visible_) {
#ifdef PRINT
  std::cerr << "Setting menu visible\n";
#endif
    setVisible(true);
    setPosition(pos);
  }
#ifdef PRINT
  else
    std::cerr << "Menu already visible\n";
#endif
  selected_ = selectedFromPos(pos);
#ifdef PRINT
  std::cerr << " selected = " << selected_ << std::endl;
#endif
  notifyValueDragged(selected_);
  repaint();
  return true;
}


void
LiteMenu::doDrag(const Event& ev)
{
  selected_ = selectedFromPos(getFieldPosition(ev));
#ifdef PRINT
  std::cerr << " selected = " << selected_ << std::endl;
#endif
  notifyValueDragged(selected_);
  repaint();
}


void
LiteMenu::doFinish(const Event& ev)
{
  selected_ = selectedFromPos(getFieldPosition(ev));
#ifdef PRINT
  std::cerr << " selected = " << selected_ << std::endl;
#endif
  notifyEnd();
  notifyBoundedRange();
  setVisible(was_visible_);
  repaint();
}

void
LiteMenu::doAbort(const Event& ev)
{
  selected_ = -1;
  notifyBoundedRange();
  setVisible(was_visible_);
}

void
LiteMenu::setSelected(int s)
{
  selected_ = s;
}

int
LiteMenu::getSelected() const
{
  return selected_;
}

int
LiteMenu::selectedFromPos(const Point& pos)
{
  Box b;
  for (int i = 0; i < childCount(); i++) {
    if (infovis::inside(getChild(i)->getBounds(), x(pos), y(pos)))
      return i;
  }
  return -1;
}

void
LiteMenu::doRender(const RenderContext& c)
{
  Box bounds(getInBounds());
  set_color(color_);
  draw_box(bounds);
  border_.render(c, bounds);
}

void
LiteMenu::render(const RenderContext& c)
{
  if (! is_visible)
    return;
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  doRender(c);
  if (c.is_picking) {
  // don't pick in children
    glPopAttrib();
    return;
  }
  for (int i = 0; i < childCount(); i++) {
    getChild(i)->render(c);
    if (i == selected_) {
      //gl::color(1.0f, 0.0f, 0.0f, 1.0f);
      gl::color(1.0f, 0.0f, 0.0f, 0.3f);
      draw_box(getChild(i)->getBounds());
    }
  }
  glPopAttrib();
}

const Color& 
LiteMenu::getColor() const
{
  return color_;
}

void
LiteMenu::setColor(const Color& c)
{
  color_ = c;
}

} // namespace infovis


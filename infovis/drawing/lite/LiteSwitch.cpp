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
#include <infovis/drawing/lite/LiteSwitch.hpp>
#include <infovis/drawing/lite/LayoutStack.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LiteSwitch::LiteSwitch(bool change_bounds)
  : LiteBox(new LayoutStack()),
    selected_(-1),
    change_bounds_(change_bounds)
{
}

void
LiteSwitch::render(const RenderContext& c)
{
  if (! is_visible)
    return;
  doRender(c);
  if (selected_ < 0 || selected_ >= childCount())
    return;
  if (c.is_picking)
    glPushName(selected_);
  getChild(selected_)->render(c);
  if (c.is_picking)
    glPopName();
}

Lite *
LiteSwitch::clone() const
{
  LiteBox * ret = new LiteSwitch(change_bounds_);
  for (List::const_iterator i = begin(); i != end(); i++)
    ret->addChild((*i)->clone());
  return ret;
}

int
LiteSwitch::getSelected() const { return selected_; }

void
LiteSwitch::setSelected(int v)
{
  selected_ = v;
  computeBounds();
}

void
LiteSwitch::setChangeBounds(bool v)
{
  change_bounds_ = v;
  computeBounds();
}

bool
LiteSwitch::getChangeBounds() const
{
  return change_bounds_;
}

void
LiteSwitch::computeBounds()
{
  bounds = layout_->doLayoutIn(this); // ignore the result
  if (change_bounds_) {
    if (selected_ >= 0 && selected_ < childCount())
      bounds = getChild(selected_)->getBounds();
    else
      bounds = Box();
  }
}

} // namespace infovis

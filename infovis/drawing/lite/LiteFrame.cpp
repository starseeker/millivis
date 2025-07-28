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
#include <infovis/drawing/lite/LiteFrame.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LiteFrame::LiteFrame(Lite * l, const Color& c, float width)
  : LiteProxy(l),
    color_(c),
    border_(c, width),
    width_(width)
{ }


Box
LiteFrame::getBounds() const
{
  return border_.growBox(LiteProxy::getBounds());
}

void
LiteFrame::setBounds(const Box& b)
{
  LiteProxy::setBounds(border_.shrinkBox(b));
}

void
LiteFrame::doRender(const RenderContext& rc)
{
  border_.render(rc, LiteProxy::getBounds());
  LiteProxy::doRender(rc);
}

Lite *
LiteFrame::clone() const
{
  return new LiteFrame(lite_->clone(), color_, border_.getWidth());
}

Interactor *
LiteFrame::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id)
    return this;
  return 0;
}

bool
LiteFrame::doEnter(const Event&)
{
  border_.setInside(color_red);
  repaint();
  return true;
}

void
LiteFrame::doLeave(const Event&)
{
  border_.setInside(color_);
  repaint();
}

} // namespace infovis

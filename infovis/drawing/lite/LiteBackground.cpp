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
#include <infovis/drawing/lite/LiteBackground.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LiteBackground::LiteBackground(Lite * l, const Color& c)
  : LiteProxy(l),
    color_(c)
{ }


void
LiteBackground::doRender(const RenderContext& rc)
{
  if (color_[Color::alpha] != 0 || rc.is_picking) {
    set_color(color_);
    draw_box(LiteProxy::getBounds());
  }
  LiteProxy::doRender(rc);
}

Lite *
LiteBackground::clone() const
{
  return new LiteBackground(lite_->clone(), color_);
}

void
LiteBackground::setColor(const Color& c)
{
  color_ = c;
}

const Color&
LiteBackground::getColor() const
{
  return color_;
}

} // namespace infovis

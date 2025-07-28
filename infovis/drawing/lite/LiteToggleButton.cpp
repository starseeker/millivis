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
#include <infovis/drawing/lite/LiteToggleButton.hpp>
#include <infovis/drawing/lite/LiteRect.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

class LiteCross : public LiteBounded
{
public:
  LiteCross(const Box& b) : LiteBounded(b) { }

  void doRender(const RenderContext& c) {
    set_color(color_red);
    gl::begin_lines lines;
    gl::vertex(xmin(bounds), ymin(bounds));
    gl::vertex(xmax(bounds), ymax(bounds));
    gl::vertex(xmin(bounds), ymax(bounds));
    gl::vertex(xmax(bounds), ymin(bounds));
  }

  Lite * clone() const { return new LiteCross(bounds); }
};

LiteToggleButton::LiteToggleButton(int size)
  : LiteToggle(new LiteCross(Box(0, 0, size, size)),
	       new LiteRect(Box(0, 0, size, size), color_white))
{ }

Lite *
LiteToggleButton::clone() const
{
  return new LiteToggleButton();
}

Interactor *
LiteToggleButton::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id)
    return this;
  return 0;
}

void
LiteToggleButton::doFinish(const Event& ev)
{
  if (infovis::inside(getBounds(), getFieldX(ev), getFieldY(ev)))
    toggle();
  repaint();
}

} // namespace infovis

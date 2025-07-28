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
#include <infovis/drawing/lite/LiteTextField.hpp>
#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LiteTextField::LiteTextField(const string& text,
			     float width,
			     justification j,
			     Font * font,
			     const Color& fg,
			     const Color& bg,
			     const Color& gfg)
  : LiteLabel(text, j, font, fg, bg, gfg),
    caret_position_(text.length())
{
}

int
LiteTextField::getCaretPosition() const
{
  return caret_position_;
}

void
LiteTextField::setCaretPosition(unsigned pos)
{
  if (pos > getLabel().length())
    pos = getLabel().length();
  caret_position_ = pos;
}

void
LiteTextField::doRender(const RenderContext& c)
{
  LiteLabel::doRender(c);
  Point p = LiteLabel::getStartPos();
  if (caret_position_ != 0) {
    string s = getLabel().substr(0, caret_position_);
    set_x(p, x(p)+ font_->stringWidth(s));
  }
  set_color(foreground_color_);
  glBegin(GL_LINES);
  gl::vertex(x(p), y(p)-font_->getDescent());
  gl::vertex(x(p), y(p)-font_->getAscent());
  glEnd();
}

Lite *
LiteTextField::clone() const
{
  return new LiteTextField(label_,
			   width(bounds),
			   justif_,
			   font_,
			   foreground_color_,
			   background_color_,
			   grown_color_);
}

Interactor *
LiteTextField::interactor(const string& name, int toolid)
{
  if (Interactor3States::isA(name) && toolid == mouse_id) {
    Interactor3States* me = this;
    return me;
  }
  else if (KeyCodeInteractor::isA(name) && toolid == keyboard_id) {
    KeyCodeInteractor * me = this;
    return me;
  }
  return LiteLabel::interactor(name, toolid);
}
  
  // Interactor3States
bool
LiteTextField::doStart(const Event& ev)
{
  return false;
}

void
LiteTextField::doDrag(const Event& ev)
{
}

void
LiteTextField::doFinish(const Event& ev)
{
}

  // KeyCodeInteractor
void
LiteTextField::doKeyDown(KeyCode code)
{
}


} // namespace infovis

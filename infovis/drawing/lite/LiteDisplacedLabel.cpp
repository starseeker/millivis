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
#include <infovis/drawing/lite/LiteDisplacedLabel.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

LiteDisplacedLabel::LiteDisplacedLabel(const string& label,
				       const Point& orig,
				       justification j,
				       Font * font,
				       const Color& fg,
				       const Color& bg,
				       const Color& gfg)
  : LiteLabel(label, j, font, fg, bg, gfg),
    orig_(orig),
    line_alpha_(1.0f)
{
  setPosition(orig_);
  path_.push_back(orig_);
}

void
LiteDisplacedLabel::doRender(const RenderContext& c)
{
  if (grown_color_[Color::alpha] != 0 &&
      line_alpha_ != 0) {
    glColor4ub(grown_color_[Color::red],
	       grown_color_[Color::green],
	       grown_color_[Color::blue],
	       GLubyte(grown_color_[Color::alpha] * line_alpha_));
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for (Path::const_iterator i = path_.begin();
	 i != path_.end(); i++) {
      set_vertex(*i);
    }
    //set_vertex(getPosition());
    glEnd();
  }
  if (line_alpha_ != 0) {
    glColor4ub(foreground_color_[Color::red],
	       foreground_color_[Color::green],
	       foreground_color_[Color::blue],
	       GLubyte(foreground_color_[Color::alpha] * line_alpha_));
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    for (Path::const_iterator i = path_.begin();
	 i != path_.end(); i++) {
      set_vertex(*i);
    }
    //set_vertex(getPosition());
    glEnd();
  }
  LiteLabel::doRender(c);
}

Lite *
LiteDisplacedLabel::clone() const
{
  return new LiteDisplacedLabel(getLabel(),
				getOrig(),
				getJustification(),
				getFont(),
				getForegroundColor(),
				getBackgroundColor(),
				getGrownColor());
}

void
LiteDisplacedLabel::setLineAlpha(float alpha)
{
  line_alpha_ = alpha;
}

} // namespace infovis

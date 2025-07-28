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
#ifndef INFOVIS_DRAWING_LITE_LITELABEL_HPP
#define INFOVIS_DRAWING_LITE_LITELABEL_HPP

#include <infovis/drawing/lite/LiteBounded.hpp>

namespace infovis {
class Font;

/**
 * Lite displaying a one-line text.
 */
class LiteLabel : public LiteBounded
{
public:
  enum justification {
    just_left,
    just_center,
    just_right,
    just_fill
  };
  LiteLabel(const string& label, 
	    justification j = just_left,
	    Font * font = 0,
	    const Color& fg = color_black,
	    const Color& bg = color_white,
	    const Color& gfg = color_none);

  virtual Box getMinBounds() const;

  virtual Point getPosition() const;

  virtual void doRender(const RenderContext& c);
  virtual Lite * clone() const;

  virtual const string& getLabel() const;
  virtual void setLabel(const string& lab);

  virtual justification getJustification() const;
  virtual void setJustification(justification j);

  virtual Font * getFont() const;
  virtual void setFont(Font * font);

  virtual const Color& getForegroundColor() const;
  virtual void setForegroundColor(const Color& color);

  virtual const Color& getBackgroundColor() const;
  virtual void setBackgoundColor(const Color& color);

  virtual const Color& getGrownColor() const;
  virtual void setGrownColor(const Color& color);

  virtual Point getStartPos() const;

protected:
  string label_;
  justification justif_;
  Font * font_;
  Color foreground_color_;
  Color background_color_;
  Color grown_color_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_LITELABEL_HPP

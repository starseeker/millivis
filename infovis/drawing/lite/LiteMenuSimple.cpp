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
#include <infovis/drawing/lite/LiteMenuSimple.hpp>
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/Font.hpp>

namespace infovis {

LiteMenuSimple::LiteMenuSimple(Font * font,
			       const Color& fg,
			       const Color& c,
			       const Color& gfg)
  : LiteMenu(c),
    font_(font == 0 ? Font::create() : font),
    foreground_color_(fg),
    grown_color_(gfg)
{ }

Lite *
LiteMenuSimple::clone() const
{
  LiteMenuSimple * ret = new LiteMenuSimple(font_,
					    foreground_color_,
					    getColor(),
					    grown_color_);
  for (int i =0; i < labels_.size(); i++) {
    ret->addItem(getItem(i));
  }
  return ret;
}

Lite *
LiteMenuSimple::createLabel(const string& label)
{
  return new LiteLabel(label,
		       LiteLabel::just_left,
		       font_,
		       foreground_color_,
		       color_none,
		       grown_color_);
}

void
LiteMenuSimple::addItem(const string& str)
{
  labels_.push_back(str);
  LiteMenu::addChild(createLabel(str));
}

const string&
LiteMenuSimple::getItem(int index) const
{
  return labels_[index];
}

void
LiteMenuSimple::setChild(int index, Lite * l)
{
  Lite::setChild(index, l);
}

void
LiteMenuSimple::addChild(Lite * l)
{
  Lite::addChild(l);
}

void
LiteMenuSimple::insertChild(int pos, Lite * l)
{
  Lite::insertChild(pos, l);
}

void
LiteMenuSimple::eraseChild(int pos)
{
  if (pos >= 0 && pos < labels_.size()) {
    labels_.erase(labels_.begin()+pos);
    LiteMenu::eraseChild(pos);
  }
}


} // namespace infovis

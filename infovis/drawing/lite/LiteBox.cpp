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
#include <infovis/drawing/lite/LiteBox.hpp>

namespace infovis {

LiteBox::LiteBox(Layout * l) : layout_(l) { is_fixed_ = true; }

Lite *
LiteBox::clone() const
{
  LiteBox * ret = new LiteBox(layout_->clone());
  for (List::const_iterator i = begin(); i != end(); i++)
    ret->addChild((*i)->clone());
  return ret;
}

void
LiteBox::setChild(int index, Lite * l)
{
  getLayout()->setLite(index, l);
  LiteGroup::setChild(index, l);
}

void
LiteBox::addChild(Lite * l)
{
  layout_->addLite(l);
  LiteGroup::addChild(l);
}

void
LiteBox::removeChild(Lite * l)
{
  layout_->removeLite(l);
  LiteGroup::removeChild(l);
}

void
LiteBox::insertChild(int pos, Lite * l)
{
  getLayout()->insertLite(pos, l);
  LiteGroup::insertChild(pos, l);
}

void
LiteBox::eraseChild(int pos)
{
  getLayout()->eraseLite(pos);
  LiteGroup::eraseChild(pos);
}

Layout *
LiteBox::getLayout() const { return layout_; }

void
LiteBox::setLayout(Layout* v)
{
  //delete layout_;
  layout_ = v;
  for (int i = 0; i < childCount(); i++)
    layout_->addLite(getChild(i));
}

void
LiteBox::computeBounds()
{
  bounds = layout_->doLayoutIn(this);
}

void
LiteBox::pack()
{
  computeBounds();
}

} // namespace infovis

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
#include <infovis/drawing/lite/LiteButton.hpp>
#include <infovis/drawing/lite/LiteFrame.hpp>
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/lite/LiteTabbed.hpp>

namespace infovis {

LiteTabbed::LiteTabbed()
  : tabs_(new LayoutBox(LayoutBox::horizontal)),
    switch_(false)
{
  addChild(&switch_);
  addChild(&tabs_);
}

Lite *
LiteTabbed::clone() const
{
  LiteTabbed * ret = new LiteTabbed();
  // FIXME +++
  return ret;
}

void
LiteTabbed::addTab(Lite * icon, Lite * tab)
{
  LiteButton * button = new LiteButton(new LiteFrame(icon, color_black, 2));
  button->addChangeObserver(this);
  tabs_.addChild(button);
  switch_.addChild(tab);
  tabs_.invalidate();
  switch_.invalidate();
  invalidate();
}

void
LiteTabbed::addTab(const std::string& name, Lite * tab)
{
  addTab(new LiteLabel(name, LiteLabel::just_center, 0, color_black, color_none), tab);
}

void
LiteTabbed::removeTab(int index)
{
  LiteButton * b = getButton(index);
  if (b == 0)
    return;
  b->removeChangeObserver(this);
  tabs_.eraseChild(index);
  switch_.eraseChild(index);
}

LiteButton *
LiteTabbed::getButton(int index) const
{
  return dynamic_cast<LiteButton*>(tabs_.getChild(index));
}

int
LiteTabbed::getSelected() const
{
  return switch_.getSelected();
}

void
LiteTabbed::setSelected(int v)
{
  switch_.setSelected(v);
}


void
LiteTabbed::changed(ChangeObservable * obs)
{
  for (int i = 0; i < tabs_.childCount(); i++) {
    LiteButton * b = getButton(i);
    if (b == obs) {
      setSelected(i);
      return;
    }
  }
}


} // namespace infovis

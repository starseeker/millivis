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
#include <infovis/drawing/lite/LiteComboBox.hpp>

namespace infovis {

LiteComboBox::LiteComboBox(LiteMenu * menu, int selected)
  : LiteSwitch(),
    menu_(menu),
    selected_item_(selected)
{
  Lite * l = menu->getChild(selected);
  if (l != 0)
    addChild(l->clone());
  else
    addChild(new LiteGroup());
  setSelected(0);
  menu_->setVisible(false);
  menu_->setSelected(selected);
  menu_->addBoundedRangeObserver(this);
  addChild(menu_);
}

Interactor *
LiteComboBox::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id)
    return this;
  return 0;
}

Lite *
LiteComboBox::clone() const
{
  return new LiteComboBox(dynamic_cast<LiteMenu*>(menu_->clone()),
			  menu_->getSelected());
}

LiteMenu *
LiteComboBox::getMenu() const { return menu_; }

void
LiteComboBox::setMenu(LiteMenu * menu)
{
  removeChild(menu_);
  menu_ = menu;
  addChild(menu_);
  selected_item_ = -1;		// force re-creation of visible item
  updateBoundedRange(menu_);
}

int
LiteComboBox::getSelectedMenuItem() const
{
  return selected_item_;
}

void
LiteComboBox::setSelectedMenuItem(int i)
{
  if (i >= 0 && i < menu_->max() &&
      selected_item_ != i) {
    menu_->setSelected(i);
    selected_item_ = menu_->getSelected();
    Lite * l = menu_->getChild(selected_item_);
    if (l != 0) {
      l = l->clone();
      setChild(0, l);
    }
  }
}

bool
LiteComboBox::doStart(const Event& ev)
{
  Event nev(ev);
  setFieldPosition(nev, getPosition());

  setSelected(1);
  return menu_->start(nev);
}

void
LiteComboBox::doDrag(const Event& ev)
{
  menu_->drag(ev);
}

void
LiteComboBox::doFinish(const Event& ev)
{
  menu_->finish(ev);
  setSelected(0);
  repaint();
}

void
LiteComboBox::updateBoundedRange(BoundedRangeObservable *)
{
  if (menu_->getSelected() >= 0 &&
      selected_item_ != menu_->getSelected()) {
    selected_item_ = menu_->getSelected();
    Lite * l = menu_->getChild(selected_item_);
    if (l != 0) {
      l = l->clone();
      setChild(0, l);
    }
  }
}


} // namespace infovis

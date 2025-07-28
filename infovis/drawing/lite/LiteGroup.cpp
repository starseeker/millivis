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
#include <infovis/drawing/lite/LiteGroup.hpp>
#include <algorithm>

namespace infovis {

LiteGroup::LiteGroup()
  : is_fixed_(false)
{ }

LiteGroup::LiteGroup(const Box& b)
  : LiteBounded(b),
    is_fixed_(true)
{ }

LiteGroup::~LiteGroup() { }

void
LiteGroup::setPosition(const Point& p)
{
  Vector v = p - getPosition();

  for (List::iterator i = group_.begin(); i != group_.end(); i++) {
    (*i)->setPosition((*i)->getPosition() + v);
  }
  LiteBounded::setPosition(p);
}

Lite *
LiteGroup::clone() const
{
  LiteGroup * ret = new LiteGroup();

  for (List::const_iterator i = group_.begin(); i != group_.end(); i++)
    ret->addChild((*i)->clone());
  return ret;
}
int
LiteGroup::childCount() const { return group_.size(); }
Lite *
LiteGroup::getChild(int index) const
{
  if (index >= 0 && index < group_.size())
    return group_[index];
  return 0;
}

void
LiteGroup::setChild(int index, Lite *l )
{
  if (index >= 0 && index < group_.size()) {
    group_[index] = l;
    computeBounds();		// should be optimized
  }

}

void
LiteGroup::addChild(Lite * c)
{
  group_.push_back(c);
  computeBounds();		// should be optimized
}

void
LiteGroup::insertChild(int i, Lite * l)
{
  if (i >= 0 && i <= childCount()) {
    group_.insert(group_.begin()+i, l);
    computeBounds();		// should be optimized
  }
}

void
LiteGroup::eraseChild(int i)
{
  if (i >= 0 && i < childCount()) {
    group_.erase(group_.begin()+i);
    computeBounds();		// should be optimized
  }
}

Lite::iterator
LiteGroup::begin()
{
  return group_.begin();
}

Lite::const_iterator
LiteGroup::begin() const
{
  return group_.begin();
}

Lite::iterator
LiteGroup::end()
{
  return group_.end();
}

Lite::const_iterator
LiteGroup::end() const
{
  return group_.end();
}

void
LiteGroup::setFixed(bool f)
{
  is_fixed_ = f;
  if (! is_fixed_)
    computeBounds();
}

void
LiteGroup::computeBounds()
{
  if (isFixed())
    return;
  if (group_.size() == 0) {
    bounds = Box();
    return;
  }
  List::const_iterator i = group_.begin();
  bounds = (*i)->getBounds();
  for (++i; i != group_.end(); ++i) {
    bounds = box_union(bounds, (*i)->getBounds());
  }
}

bool
LiteGroup::isFixed() const
{
  return is_fixed_;
}

} // namespace infovis

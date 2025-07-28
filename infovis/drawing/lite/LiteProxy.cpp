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
#include <infovis/drawing/lite/LiteProxy.hpp>

namespace infovis {

LiteProxy::~LiteProxy() { }

void
LiteProxy::setLite(Lite * l)
{
  lite_ = l;
}

Lite *
LiteProxy::getLite() const
{
  return lite_;
}

Box
LiteProxy::getBounds() const
{
  return lite_->getBounds();
}

void
LiteProxy::setBounds(const Box& b)
{
  lite_->setBounds(b);
}

Box
LiteProxy::getMinBounds() const
{
  return lite_->getMinBounds();
}

Point
LiteProxy::getPosition() const
{
  return lite_->getPosition();
}

void
LiteProxy::setPosition(const Point& p)
{
  lite_->setPosition(p);
}

Vector
LiteProxy::getSize() const
{
  return lite_->getSize();
}

void
LiteProxy::setSize(const Vector& s)
{
  lite_->setSize(s);
}

bool
LiteProxy::intersects(const Box& b) const
{
  return lite_->intersects(b);
}

bool
LiteProxy::contains(const Box& b) const
{
  return lite_->contains(b);
}

bool
LiteProxy::inside(const Point& p) const
{
  return lite_->inside(p);
}

void
LiteProxy::doRender(const RenderContext& c)
{
  //lite_->render(c);
}

Lite *
LiteProxy::clone() const
{
  return new LiteProxy(lite_->clone());
}

Interactor *
LiteProxy::interactor(const string& name, int toolid)
{
  return lite_->interactor(name, toolid);
}

int
LiteProxy::childCount() const
{
  return lite_ == 0 ? 0 : 1;
}

Lite *
LiteProxy::getChild(int i) const
{
  if (i == 0)
    return lite_;
  return Lite::getChild(i);
}

void
LiteProxy::setChild(int index, Lite * l)
{
  if (index == 0)
    lite_ = l;
  else
    Lite::setChild(index, l);
}

int
LiteProxy::indexOf(Lite * l) const
{
  return l == lite_ ? 0 : -1;
}

Lite *
LiteProxy::getPath(const Path& p)
{
  return Lite::getPath(p);
}

void
LiteProxy::addChild(Lite * l)
{
  if (lite_ == 0)
    lite_ = l;
  else
    Lite::addChild(l);
}

void
LiteProxy::removeChild(Lite * l)
{
  if (lite_ == l)
    lite_ = 0;
}

void
LiteProxy::insertChild(int pos, Lite * l)
{
  if (lite_ == 0 && pos == 0)
    lite_ = l;
  else
    Lite::insertChild(pos, l);
}

void
LiteProxy::eraseChild(int pos)
{
  if (pos == 0)
    lite_ = 0;
  else
    Lite::eraseChild(pos);
}

void
LiteProxy::removeAll()
{
  lite_ = 0;
}

Lite::iterator
LiteProxy::begin()
{
  return Lite::iterator(&lite_);
}

Lite::const_iterator
LiteProxy::begin() const
{
  return Lite::const_iterator(&lite_);
}

Lite::iterator
LiteProxy::end()
{
  return Lite::iterator((&lite_)+1);
}

Lite::const_iterator
LiteProxy::end() const
{
  return Lite::const_iterator((&lite_)+1);
}

void
LiteProxy::repaint() const
{
  Lite::repaint();
}
void
LiteProxy::setVisible(bool b)
{
  Lite::setVisible(b);		// don't sent it to lite
}

void
LiteProxy::invalidate()
{
  lite_->invalidate();
}

void
LiteProxy::pack()
{
  lite_->pack();
}


} // namespace infovis

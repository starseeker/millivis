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
#include <infovis/drawing/lite/LayoutStack.hpp>
#include <algorithm>

namespace infovis {

LayoutStack::LayoutStack()
{ }

LayoutStack::~LayoutStack()
{ }

Layout *
LayoutStack::clone() const
{
  return new LayoutStack();	// don't copy contents
}

void
LayoutStack::setLite(int index, Lite* l)
{
  if (index >= 0 && index < lite_.size())
    lite_[index] = l;
}

void
LayoutStack::addLite(Lite * l)
{
  lite_.push_back(l);
}

void
LayoutStack::removeLite(Lite * l)
{
  Lite::List::iterator i = std::find(lite_.begin(), lite_.end(), l);
  if (i != lite_.end())
    lite_.erase(i);
}

void
LayoutStack::insertLite(int i, Lite * l)
{
  if (i >= 0 && i <= lite_.size())
    lite_.insert(lite_.begin()+i, l);
}

void
LayoutStack::eraseLite(int i)
{
  if (i >= 0 && i < lite_.size())
    lite_.erase(lite_.begin()+i);
}

Box
LayoutStack::doLayoutIn(Lite * container)
{
  Vector size(0, 0);

  Box b(container->getBounds());
  Point p(min(b));
  for (Lite::List::const_iterator i = lite_.begin();
       i != lite_.end(); i++) {
    Lite * l = *i;
    Box b(l->getBounds());
    Vector v(extent(b));

    l->movePosition(p-min(b));
    set_dx(size, std::max(dx(size),dx(v)));
    set_dy(size, std::max(dy(size),dy(v)));
  }
  container->setSize(size);
  return Box(min(b),size);
}

} // namespace infovis

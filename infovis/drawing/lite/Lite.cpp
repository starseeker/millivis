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
#include <infovis/drawing/lite/Lite.hpp>
#include <infovis/drawing/inter/Interactor.hpp>
#include <infovis/drawing/gl.hpp>
#include <GLFW/glfw3.h>
#include <stdexcept>
//#include <iostream>

namespace infovis {

Lite::~Lite()
{
  //std::cerr << "Freeing lite " << this << std::endl;
}

Box
Lite::getMinBounds() const
{
  return getBounds();
}

void
Lite::movePosition(const Vector& v)
{
  setPosition(getPosition()+v);
}

Vector
Lite::getSize() const
{
  return extent(getBounds());
}

void
Lite::setSize(const Vector& v)
{
  setBounds(Box(min(getBounds()), v));
}

bool
Lite::intersects(const Box& b) const
{
  return infovis::intersects(getBounds(), b);
}

bool
Lite::contains(const Box& b) const
{
  return infovis::contains(getBounds(), b);
}

bool
Lite::inside(const Point& p) const
{
  return infovis::inside(getBounds(), x(p), y(p));
}

void
Lite::doRender(const RenderContext& c)
{ }

void
Lite::render(const RenderContext& c)
{
  if (! is_visible)
    return;
  doRender(c);
  if (c.is_picking)
    glPushName(unsigned(-1));
  for (int i = 0; i < childCount(); i++) {
    if (c.is_picking)
      glLoadName(i);
    getChild(i)->render(c);
  }
  if (c.is_picking)
    glPopName();
}

Interactor *
Lite::interactor(const string& name, int toolid)
{
  return 0;
}

int
Lite::childCount() const { return 0; }

Lite *
Lite::getChild(int ) const { return 0; }

void
Lite::setChild(int, Lite* )
{
  throw std::overflow_error("Lite is not a container");
}

int
Lite::indexOf(Lite * l) const
{
  for (int i = 0; i < childCount(); i++) {
    if (getChild(i) == l)
      return i;
  }
  return -1;
}

Lite *
Lite::getPath(const Path& path)
{
  Lite * l = this;
  for (Path::const_iterator i = path.begin()+1;	// we always start with 0
       l != 0 && i != path.end();
       i++)
    l = l->getChild(*i);
  return l;
}

void
Lite::addChild(Lite *)
{
  throw std::overflow_error("Lite is not a container");
}

void
Lite::removeChild(Lite * l)
{
  eraseChild(indexOf(l));
}

void
Lite::insertChild(int, Lite *)
{
  throw std::overflow_error("Lite is not a container");
}

void
Lite::eraseChild(int)
{
  throw std::overflow_error("Lite is not a container");
}

void
Lite::removeAll()
{
  while (childCount() != 0)
    eraseChild(childCount()-1);
}

Lite::iterator
Lite::begin()
{
  return iterator(0);
}

Lite::const_iterator
Lite::begin() const
{
  return iterator(0);
}

Lite::iterator
Lite::end()
{
  return iterator(0);
}

Lite::const_iterator
Lite::end() const
{
  return iterator(0);
}

void
Lite::repaint() const
{
  // TODO: GLFW migration - glutPostRedisplay() equivalent
  // In GLFW, the application needs to manage redraws through the main event loop
  // This is a placeholder - proper GLFW integration requires restructuring the Lite framework
  // glutPostRedisplay();
}

void
Lite::setVisible(bool b)
{
  is_visible = b;
}

void
Lite::invalidate()
{
  pack();
}

void
Lite::pack()
{
  for (int i = 0; i < childCount(); i++) {
    getChild(i)->pack();
  }
}

} // namespace infovis

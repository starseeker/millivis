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
#include <infovis/drawing/inter/InteractorManager.hpp>

namespace infovis {

InteractorManager::InteractorManager()
  : window_(0),
    event_("event")
{ }

InteractorManager::~InteractorManager()
{ }
  
void
InteractorManager::connect(LiteWindow * win)
{
  window_ = win;
}

void
InteractorManager::disconnect(LiteWindow * win)
{
  window_ = 0;
}

Interactor *
InteractorManager::pick(const Event& ev)
{
  if (window_ == 0)
    return 0;
  Interactor * inter = 0;
  if (! ev.hasField(field_x) || ! ev.hasField(field_y))
    return 0;

  float x = getFieldX(ev);
  float y = getFieldY(ev);
  const string& iname = interactorName();
  Pick pick;

  window_->pick(Box(x-1, y-1, x+1, y+1), pick);

  for (Pick::reverse_iterator i = pick.rbegin(); i != pick.rend(); i++) {
    inter = (*i)->interactor(iname, mouse_id);
    if (inter != 0) {
      break;
    }
  }
  return inter;
}

LiteWindow *
InteractorManager::getWindow() const
{
  return window_;
}


} // namespace infovis 

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
#include <infovis/drawing/inter/DefaultManagerIdle.hpp>
#include <infovis/drawing/inter/Interactor3States.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>

namespace infovis {

DefaultManagerIdle::DefaultManagerIdle()
  : timeout_(1000),
    received_(0),
    interactor3states_(0)
{ }

void
DefaultManagerIdle::connect(LiteWindow * w)
{
  InteractorManager::connect(w);
  w->addTimerHandler(timeout_/4, this);
  w->addMouseHandler(this);
}

void
DefaultManagerIdle::disconnect(LiteWindow * w)
{
  w->removeMouseHandler(this);
  w->removeTimerHandler(this);
  InteractorManager::disconnect(w);
}

void
DefaultManagerIdle::motion(float x, float y)
{
  setFieldX(event_, x);
  setFieldY(event_, y);
  setFieldMod(event_, window_->getModifiers());
  if (isIdle() && ! inhibit()) {
    active(event_);
  }
  received_ = 0;
}

void
DefaultManagerIdle::timer()
{
  received_++;
  if (received_ == 4 && ! inhibit()) {
      idle(event_);
  }
  window_->addTimerHandler(timeout_/4, this);
}

bool
DefaultManagerIdle::inhibit() const
{
  if (interactor3states_ == 0)
    return false;
  return interactor3states_->isDragged();
}

} // namespace infovis

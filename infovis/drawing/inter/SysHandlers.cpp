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
#include <infovis/drawing/inter/SysHandlers.hpp>

namespace infovis {

SystemMouseHandler::SystemMouseHandler()
  : Manager3State()
{ }

void
SystemMouseHandler::button(int button, bool down, float x, float y)
{
  if (button == 0) {
    event_.nextGlobalTimeStamp();
    setFieldX(event_, x);
    setFieldY(event_, y);
    setFieldMod(event_, window_->getModifiers());
    
    if (down)
      start(event_);
    else
      finish(event_);
  }
}

void
SystemMouseHandler::motion(float x, float y)
{
  event_.nextGlobalTimeStamp();
  setFieldX(event_, x);
  setFieldY(event_, y);
  setFieldMod(event_, window_->getModifiers());
  if (isDragged())
    drag(event_);
  else
    move(event_);
}

void
SystemMouseHandler::connect(LiteWindow * win)
{
  Manager3State::connect(win);
  win->addMouseHandler(this);
}

void
SystemMouseHandler::disconnect(LiteWindow * win)
{
  win->removeMouseHandler(this);
  Manager3State::disconnect(win);
}


SystemEnterLeaveHandler::SystemEnterLeaveHandler()
{ }

void
SystemEnterLeaveHandler::motion(float x, float y)
{
  event_.nextGlobalTimeStamp();
  setFieldX(event_, x);
  setFieldY(event_, y);
  setFieldMod(event_, window_->getModifiers());
  move(event_);
}

void
SystemEnterLeaveHandler::connect(LiteWindow * win)
{
  ManagerEnterLeave::connect(win);
  win->addMouseHandler(this);
}

void
SystemEnterLeaveHandler::disconnect(LiteWindow * win)
{
  win->removeMouseHandler(this);
  ManagerEnterLeave::disconnect(win);
}

} // namespace infovis

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
#ifndef INFOVIS_DRAWING_INTERACTOR_SYSHANDLERS_HPP
#define INFOVIS_DRAWING_INTERACTOR_SYSHANDLERS_HPP

#include <infovis/drawing/inter/MouseHandler.hpp>
#include <infovis/drawing/inter/KeyboardHandler.hpp>
#include <infovis/drawing/inter/Manager3State.hpp>
#include <infovis/drawing/inter/ManagerEnterLeave.hpp>

namespace infovis {

/**
 * The system mouse handler
 */
class SystemMouseHandler : public MouseHandler,
			   public Manager3State
{
public:
  SystemMouseHandler();
  virtual void button(int button, bool down, float x, float y);
  virtual void motion(float x, float y);
  virtual void connect(LiteWindow *);
  virtual void disconnect(LiteWindow *);
};

/**
 * The system EnterLeave handler
 */ 
class SystemEnterLeaveHandler : public MouseHandler,
				public ManagerEnterLeave
{
public:
  SystemEnterLeaveHandler();
  virtual void motion(float x, float y);
  virtual void connect(LiteWindow *);
  virtual void disconnect(LiteWindow *);
};

#if 0
class SystemKeyboardHandler : public KeyboardHandler,
			      public ManagerKeyboard
{
};
#endif

} // namespace infovis

#endif // INFOVIS_DRAWING_INTERACTOR_SYSHANDLERS_HPP

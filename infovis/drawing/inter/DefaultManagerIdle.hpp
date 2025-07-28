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
#ifndef INFOVIS_DRAWING_INTERACTOR_DEFAULTMANAGERIDLE_HPP
#define INFOVIS_DRAWING_INTERACTOR_DEFAULTMANAGERIDLE_HPP

#include <infovis/drawing/inter/ManagerIdle.hpp>
#include <infovis/drawing/inter/MouseHandler.hpp>
#include <infovis/drawing/inter/TimerHandler.hpp>

namespace infovis {

class Interactor3States;

/**
 * Default implementation of Idle manager
 */
class DefaultManagerIdle : public ManagerIdle,
			   public MouseHandler,
			   public TimerHandler
{
public:
  DefaultManagerIdle();

  virtual void motion(float x, float y);
  virtual void timer();

  // InteractorManager
  virtual void connect(LiteWindow *);
  virtual void disconnect(LiteWindow *);

  void setTimeout(unsigned long millis) { timeout_ = millis; }
  unsigned long getTimeout() { return timeout_; }

  Interactor3States * getInteractor3States() const { return interactor3states_; }
  void setInteractor3States(Interactor3States * i) { interactor3states_ = i; }
  bool inhibit() const;
protected:
  unsigned long timeout_;
  int received_;
  Interactor3States * interactor3states_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_INTERACTOR_DEFAULTMANAGERIDLE_HPP

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
#ifndef INFOVIS_DRAWING_INTERACTOR_MANAGERENTERLEAVE_HPP
#define INFOVIS_DRAWING_INTERACTOR_MANAGERENTERLEAVE_HPP

#include <infovis/drawing/inter/InteractorManager.hpp>
#include <infovis/drawing/inter/InteractorEnterLeave.hpp>

#include <vector>

namespace infovis {

/**
 * Manages EnterLeave interactors 
 */
class ManagerEnterLeave : public InteractorEnterLeave,
			  public InteractorManager
{
public:
  typedef std::vector<InteractorEnterLeave *> Stack;
  ManagerEnterLeave();

  // InteractorManager
  virtual const string& interactorName() const;

  // Interactor
  virtual void doDesactivate(const Event& ev); // active->inactive
  virtual void doAbort(const Event& ev);	// active->inactive

  Stack pick(const Event& ev) const;

  virtual bool doEnter(const Event& ev);
  virtual void doLeave(const Event& ev);
  virtual void doMove(const Event& ev);

  void initFieldEvent(Event& ev); // not virtual
protected:
  Stack inter_stack_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_INTERACTOR_MANAGERENTERLEAVE_HPP

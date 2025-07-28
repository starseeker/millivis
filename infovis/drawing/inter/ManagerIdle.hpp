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
#ifndef INFOVIS_DRAWING_INTERACTOR_MANAGERIDLE_HPP
#define INFOVIS_DRAWING_INTERACTOR_MANAGERIDLE_HPP

#include <infovis/drawing/inter/InteractorManager.hpp>
#include <infovis/drawing/inter/InteractorIdle.hpp>

namespace infovis {

/**
 * Manages idle events
 */ 
class ManagerIdle : public InteractorIdle,
		    public InteractorManager
{
public:
  ManagerIdle();

  // InteractorManager
  virtual const string& interactorName() const;

  // Interactor
  virtual void doDesactivate(const Event& ev); // active->inactive
  virtual void doAbort(const Event& ev);	// active->inactive

  virtual bool doIdle(const Event& ev);
  virtual void doActive(const Event& ev);

  void initFieldEvent(Event& ev); // not virtual
protected:
  InteractorIdle * inter_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_INTERACTOR_MANAGERIDLE_HPP

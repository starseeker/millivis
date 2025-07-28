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
#ifndef INFOVIS_DRAWING_INTERACTOR_HPP
#define INFOVIS_DRAWING_INTERACTOR_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/inter/Event.hpp>
#include <infovis/drawing/inter/EventCodes.hpp>

namespace infovis {

/**
 * Base interactor class.
 */
class Interactor : public virtual gc,
		   virtual public EventCodes
{
public:
  enum State {
    state_inactive,
    state_active
  };

  Interactor(const string& n) : name_(n), state_(state_inactive) { }
  virtual ~Interactor();

  const string& getInteractorName() const { return name_; }
  // transitions
  virtual bool activate(const Event&); // inactive->active
  virtual void desactivate(const Event&); // active->inactive
  virtual void abort(const Event&); // active->inactive

  virtual bool doActivate(const Event&); // inactive->active
  virtual void doDesactivate(const Event&); // active->inactive
  virtual void doAbort(const Event&); // active->inactive

  virtual bool isA(const string& n) const;
  State getState() const { return state_; }
  bool isActive() const { return state_ == state_active; }
  bool isInactive() const {return state_ == state_inactive; }
protected:
  string name_;
  State state_;
};


/**
 * Interactor for managing String events
 */
class StringInteractor : virtual public Interactor
{
public:
  StringInteractor() : Interactor("String") { }

  void receive(const Event&);

  virtual void doReceive(const Event&);
};

} // namespace infovis 

#endif // INFOVIS_DRAWING_INTERACTOR_HPP

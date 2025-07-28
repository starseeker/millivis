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
#ifndef INFOVIS_DRAWING_INTERACTOR_INTERACTOR_3STATE_HPP
#define INFOVIS_DRAWING_INTERACTOR_INTERACTOR_3STATE_HPP

#include <infovis/drawing/inter/Interactor.hpp>

namespace infovis {

/**
 * Base class for Interactor managing 3 States.
 */
class Interactor3States : public Interactor
{
public:
  enum State {
    state_away,
    state_present,
    state_dragged
  };

  Interactor3States()
    : Interactor("3States"), state(state_present) {}
  Interactor3States(const Interactor3States& other)
    : Interactor("3States"), state(state_present) { }

  void desactivate(const Event&);
  void abort(const Event&);

  void proximity_in(const Event& ev); // away->present
  void proximity_out(const Event& ev); // present->away
  void move(const Event& ev); // present->present
  bool start(const Event& ev); // present->dragged
  void drag(const Event& ev); // dragged->dragged
  void finish(const Event& ev); // dragged->present

  virtual void doProximityIn(const Event& ev);
  virtual void doProximityOut(const Event& ev);
  virtual void doMove(const Event& ev);
  virtual bool doStart(const Event& ev);
  virtual void doDrag(const Event& ev);
  virtual void doFinish(const Event& ev);

  State getState() const { return state; }
  bool isAway() const { return state == state_away; }
  bool isPresent() const { return state == state_present; }
  bool isDragged() const { return state == state_dragged; }

  const Point& getStartPos() const { return start_pos; }
  const Point& getCurPos() const { return current_pos; }
protected:
  State state;
  Point start_pos;
  Point current_pos;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_INTERACTOR_INTERACTOR_3STATE_HPP

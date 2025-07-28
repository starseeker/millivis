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
#include <infovis/drawing/inter/Interactor3States.hpp>

namespace infovis {

void
Interactor3States::proximity_in(const Event& ev)
{
  if (state != state_away) return;
  doProximityIn(ev);
  state = state_present;
}

void
Interactor3States::proximity_out(const Event& ev)
{
  if (state != state_present) return;
  doProximityOut(ev);
  state = state_away;
}

void
Interactor3States::move(const Event& ev)
{
  if (state != state_present) return;
  doMove(ev);
}

bool
Interactor3States::start(const Event& ev)
{
  if (state != state_present) return false;
  if (doStart(ev)) {
    state = state_dragged;
    start_pos = Point(getFieldX(ev),getFieldY(ev));
    return true;
  }
  else
    return false;
}

void
Interactor3States::drag(const Event& ev)
{
  if (state != state_dragged) return;
  doDrag(ev);
  current_pos = Point(getFieldX(ev),getFieldY(ev));
}

void
Interactor3States::finish(const Event& ev)
{
  if (state != state_dragged) return;
  doFinish(ev);
  state = state_present;
  current_pos = Point(getFieldX(ev),getFieldY(ev));
}

void
Interactor3States::desactivate(const Event& ev)
{
  Interactor::desactivate(ev);
}

void
Interactor3States::abort(const Event& ev)
{
  state = state_present;
  Interactor::abort(ev);
}

void
Interactor3States::doProximityIn(const Event& ev) { }
void
Interactor3States::doProximityOut(const Event& ev) { }
void
Interactor3States::doMove(const Event& ev) { }
bool
Interactor3States::doStart(const Event& ev) { return true; }
void
Interactor3States::doDrag(const Event& ev) { }
void
Interactor3States::doFinish(const Event& ev) { }
} // namespace infovis


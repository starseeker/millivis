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
#include <infovis/drawing/inter/ManagerEnterLeave.hpp>
#include <infovis/drawing/inter/InteractorEnterLeave.hpp>
//#define PRINT
#ifdef PRINT
#include <iostream>
#endif

namespace infovis {

ManagerEnterLeave::ManagerEnterLeave()
{
  entered_ = true;
  initFieldEvent(event_);
}

void
ManagerEnterLeave::initFieldEvent(Event& ev)
{
  ev.clear();
  ev.addField(new FieldFloat(field_x));
  ev.addField(new FieldFloat(field_y));
  ev.addField(new FieldInt(field_mod));
}

const string&
ManagerEnterLeave::interactorName() const
{
  return getInteractorName();
}

ManagerEnterLeave::Stack
ManagerEnterLeave::pick(const Event& ev) const
{
  Stack stack;
  if (window_ == 0)
    return stack;

  if (! ev.hasField(field_x) || ! ev.hasField(field_y))
    return stack;

  float x = getFieldX(ev);
  float y = getFieldY(ev);
  const string& iname = interactorName();
  Pick pick;
  InteractorEnterLeave * inter;

  window_->pick(Box(x-1, y-1, x+1, y+1), pick);

  for (Pick::iterator i = pick.begin(); i != pick.end(); i++) {
    inter = dynamic_cast<InteractorEnterLeave*>((*i)->interactor(iname, mouse_id));
    if (inter != 0)
      stack.push_back(inter);
  }
  return stack;
}


void
ManagerEnterLeave::doDesactivate(const Event& ev)
{
  for (Stack::iterator i = inter_stack_.begin();
       i != inter_stack_.end(); i++) {
    (*i)->desactivate(ev);
  }
  inter_stack_.clear();
}

void
ManagerEnterLeave::doAbort(const Event& ev)
{
  for (Stack::iterator i = inter_stack_.begin();
       i != inter_stack_.end(); i++) {
    (*i)->abort(ev);
  }
  inter_stack_.clear();
}

struct not_entered {
  bool operator() (const InteractorEnterLeave * inter) const {
    return ! inter->isEntered();
  }
};

bool
ManagerEnterLeave::doEnter(const Event& ev)
{
  return true;
}

void
ManagerEnterLeave::doLeave(const Event& ev)
{
}

static bool find(const ManagerEnterLeave::Stack& stack,
		 const InteractorEnterLeave * item)
{
  return std::find(stack.begin(), stack.end(), item) != stack.end();
}

void
ManagerEnterLeave::doMove(const Event& ev)
{
  Stack stack = pick(ev);

  if (stack == inter_stack_) {
    for (Stack::iterator i = stack.begin();
	 i != stack.end(); i++) {
      InteractorEnterLeave * inter = *i;
      inter->move(ev);
    }
    return;
  }
  for (Stack::reverse_iterator ri = inter_stack_.rbegin();
       ri != inter_stack_.rend(); ri++) {
    InteractorEnterLeave * inter = *ri;
    if (! find(stack, inter)) {
      inter->leave(ev);
#ifdef PRINT
      std::cerr << "leaving " << inter << std::endl;
#endif
    }
  }
  inter_stack_.clear();

  for (Stack::iterator i = stack.begin();
       i != stack.end(); i++) {
    InteractorEnterLeave * inter = *i;
    if (inter->isEntered()) {
      inter_stack_.push_back(inter);
      inter->move(ev);
    }
    else if (inter->enter(ev)) {
      inter_stack_.push_back(inter);
#ifdef PRINT
      std::cerr << "entering " << inter << std::endl;
#endif
    }
  }
}

} // namespace infovis

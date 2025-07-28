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
#include <infovis/drawing/inter/ManagerIdle.hpp>
#include <infovis/drawing/inter/InteractorIdle.hpp>

namespace infovis {

ManagerIdle::ManagerIdle()
  : inter_(0)
{
  initFieldEvent(event_);
}

void
ManagerIdle::initFieldEvent(Event& ev)
{
  ev.clear();
  ev.addField(new FieldFloat(field_x));
  ev.addField(new FieldFloat(field_y));
  ev.addField(new FieldInt(field_mod));
}

const string&
ManagerIdle::interactorName() const
{
  return getInteractorName();
}

void
ManagerIdle::doDesactivate(const Event& ev)
{
  if (inter_ != 0) {
    inter_->desactivate(ev);
    inter_ = 0;
  }
}

void
ManagerIdle::doAbort(const Event& ev)
{
  if (inter_ != 0) {
    inter_->abort(ev);
    inter_ = 0;
  }
}

bool
ManagerIdle::doIdle(const Event& ev)
{
  inter_ = dynamic_cast<InteractorIdle*>(pick(ev));

  if (inter_ == 0) return false;
  if (inter_->idle(ev))
    return true;
  inter_ = 0;
  return false;
}

void
ManagerIdle::doActive(const Event& ev)
{
  if (inter_ == 0) return;
  inter_->active(ev);
  desactivate(ev);
}

} // namespace infovis

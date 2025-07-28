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
#ifndef INFOVIS_DRAWING_NOTIFIERS_CHANGE_HPP
#define INFOVIS_DRAWING_NOTIFIERS_CHANGE_HPP

#include <infovis/alloc.hpp>
#include <vector>

namespace infovis {

class ChangeObservable;

/**
 * Observer for Changed events
 */
class ChangeObserver
{
public:
  virtual ~ChangeObserver();

  virtual void changed(ChangeObservable *);
};

/**
 * Observable for Changed events
 */
class ChangeObservable
{
public:
  virtual ~ChangeObservable();
  virtual void addChangeObserver(ChangeObserver * obs) = 0;
  virtual void removeChangeObserver(ChangeObserver * obs) = 0;
  virtual void notifyChanged() = 0;
};

/**
 * Default implementation of Changed observable
 */
class DefaultChangeObservable
  : public std::vector<ChangeObserver*,gc_alloc<ChangeObserver*> >,
    public ChangeObservable
{
public:
  virtual void addChangeObserver(ChangeObserver * obs);
  virtual void removeChangeObserver(ChangeObserver * obs);
  virtual void notifyChanged();
};

} // namespace infovis

#endif // INFOVIS_DRAWING_NOTIFIERS_CHANGE_HPP

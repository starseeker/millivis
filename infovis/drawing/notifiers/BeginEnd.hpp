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
#ifndef INFOVIS_DRAWING_NOTIFIERS_BEGINEND_HPP
#define INFOVIS_DRAWING_NOTIFIERS_BEGINEND_HPP

#include <infovis/alloc.hpp>
#include <vector>

namespace infovis {

class BeginEndObservable;

/**
 * Observer for begin/end sequences
 */ 
class BeginEndObserver
{
public:
  virtual ~BeginEndObserver();

  virtual void begin(BeginEndObservable *);
  virtual void end(BeginEndObservable *);
};

/**
 *Observable generating begin/end sequences
 */ 
class BeginEndObservable
{
public:
  virtual ~BeginEndObservable();
  virtual void addBeginEndObserver(BeginEndObserver * obs) = 0;
  virtual void removeBeginEndObserver(BeginEndObserver * obs) = 0;
  virtual void notifyBegin() = 0;
  virtual void notifyEnd() = 0;
};

/**
 * Default implementation of BeginEndObservable
 */
class DefaultBeginEndObservable
  : public std::vector<BeginEndObserver*, gc_alloc<BeginEndObserver*> >,
    public BeginEndObservable
{
public:
  virtual void addBeginEndObserver(BeginEndObserver * obs);
  virtual void removeBeginEndObserver(BeginEndObserver * obs);
  virtual void notifyBegin();
  virtual void notifyEnd();
};

} // namespace infovis

#endif // INFOVIS_DRAWING_NOTIFIERS_BEGINEND_HPP

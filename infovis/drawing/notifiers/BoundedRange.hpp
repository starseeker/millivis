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
#ifndef INFOVIS_DRAWING_NOTIFIERS_BOUNDEDRANGE_HPP
#define INFOVIS_DRAWING_NOTIFIERS_BOUNDEDRANGE_HPP

#include <infovis/alloc.hpp>
#include <vector>

namespace infovis {

/**
 * Bounded range model
 */
class BoundedRange
{
public:
  virtual ~BoundedRange();
  virtual float min() const = 0;
  virtual float max() const = 0;
  virtual float value() const = 0;
  virtual float range() const = 0;

  // invariant.  Note that we accept min() == max()
  bool valid() const {
    return
      min() <= value() &&
      range() >= 0 &&
      (value() + range()) <= max();
  }

  float check_drag_min(float v) const {
    if (v > value())
      return value();
    else
      return v;
  }
  float check_drag_value(float v) const {
    if (v < min())
      return min();
    else if ((v + range()) > max())
      return max() - range();
    else
      return v;
  }
  float check_drag_range(float v) const {
    if ((value() + v) > max())
      return max() - value();
    else
      return v;
  }
  float check_drag_max(float v) const {
    if (v < (value() + range()))
      return value() + range();
    else
      return v;
  }
};

/**
 * Default BoundedRange implementation
 */
class DefaultBoundedRange : public BoundedRange
{
protected:
  float min_, max_, value_, range_;
public:
  DefaultBoundedRange(float m, float M, float v, float r = 0);
  DefaultBoundedRange(const BoundedRange& other);

  const DefaultBoundedRange& operator = (const BoundedRange& other);
  virtual float min() const;
  virtual float max() const;
  virtual float value() const;
  virtual float range() const;
  
  virtual void set_min(float v);
  virtual void set_max(float v);
  virtual void set_value(float v);
  virtual void set_range(float v);
  virtual void set_value_range(float v, float r);
};

class BoundedRangeObservable;

/**
 * BoundedRange observer
 */
class BoundedRangeObserver
{
public:
  virtual ~BoundedRangeObserver();
  virtual void valueDragged(BoundedRangeObservable * obs);
  virtual void rangeDragged(BoundedRangeObservable * obs);
  virtual void updateBoundedRange(BoundedRangeObservable * obs);
};

/**
 * BoundedRange Observable
 */
class BoundedRangeObservable
{
public:
  virtual ~BoundedRangeObservable();

  virtual void addBoundedRangeObserver(BoundedRangeObserver * obs) = 0;
  virtual void removeBoundedRangeObserver(BoundedRangeObserver * obs) = 0;
  virtual const BoundedRange * getBoundedRange() const = 0;

  virtual void notifyValueDragged(float v) = 0;
  virtual void notifyRangeDragged(float r) = 0;
  virtual void notifyValueRangeDragged(float v, float r) = 0;
  virtual void notifyBoundedRange() = 0;
};

/**
 * Abstract BoundedRange Observable
 */
class AbstractBoundedRangeObservable
  : public BoundedRangeObservable,
    private std::vector<BoundedRangeObserver*, gc_alloc<BoundedRangeObserver*> >
{
public:
  virtual void addBoundedRangeObserver(BoundedRangeObserver * obs);
  virtual void removeBoundedRangeObserver(BoundedRangeObserver * obs);
  virtual const BoundedRange * getBoundedRange() const = 0;
  virtual void notifyValueDragged(float v);
  virtual void notifyRangeDragged(float r);
  virtual void notifyValueRangeDragged(float v, float r);
  virtual void notifyBoundedRange();
};

/**
 * Default BoundedRange Observable
 */
class DefaultBoundedRangeObservable
  : public AbstractBoundedRangeObservable, public DefaultBoundedRange
{
public:
  DefaultBoundedRangeObservable(float m, float M, float v, float r = 0)
    : DefaultBoundedRange(m, M, v, r) { }

  virtual const BoundedRange * getBoundedRange() const;  

  virtual void notifyValueDragged(float v);
  virtual void notifyRangeDragged(float r);
  virtual void notifyValueRangeDragged(float v, float r);
};

} // namespace infovis
#endif // INFOVIS_DRAWING_NOTIFIERS_BOUNDEDRANGE_HPP

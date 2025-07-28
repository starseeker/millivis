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
#ifndef TREEMAP2_RECORDER_HPP
#define TREEMAP2_RECORDER_HPP

#include <infovis/drawing/notifiers/BoundedRange.hpp>
#include <types.hpp>
#include <ColorRamp.hpp>
#include <LiteTreemap.hpp>

namespace infovis {

struct RecordItem
{
  string weight;
  string color;
  string name;
  Ramp ramp;
  bool color_smooth;
  float color_min, color_range;
  string order;
  LiteTreemap::Layout layout;
  string x_axis;
  string y_axis;

  bool operator == (const RecordItem& other) const {
    return
      weight == other.weight &&
      color == other.color &&
      name == other.name &&
      ramp == other.ramp &&
      color_smooth == other.color_smooth &&
      color_min == other.color_min &&
      color_range == other.color_range &&
      order == other.order &&
      layout == other.layout &&
      x_axis == other.x_axis &&
      y_axis == other.y_axis;
  }
};

class Recorder : public std::vector<RecordItem>,
		 public BoundedRange,
		 public AbstractBoundedRangeObservable
{
public:
  typedef std::vector<RecordItem> super_vector;
  Recorder();

  void set_current(unsigned c);
  unsigned get_current() const;
  void advance(int i);
  void add(const RecordItem& r);
  const RecordItem& current() const;

  void load(std::istream& in);
  void load(const string& fname);
  void save(std::ostream& out);
  void save(const string& fname);

  size_t size() const { return super_vector::size(); }
  // BoundedRange
  virtual float min() const;
  virtual float max() const;
  virtual float value() const;
  virtual float range() const;

  // AbstractBoundedRangeObservable
  virtual const BoundedRange * getBoundedRange() const;
  virtual void notifyValueDragged(float v);

protected:
  unsigned current_;
};

} // namespace infovis


#endif // TREEMAP2_RECORDER_HPP

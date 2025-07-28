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
#ifndef INFOVIS_TABLE_FILTER_HPP
#define INFOVIS_TABLE_FILTER_HPP

#include <infovis/alloc.hpp>
#include <infovis/table/column.hpp>

namespace infovis {

//typedef CharColumn FilterColumn;

/**
 * Filter class to filter out elements in some algorithms.
 */
struct filter_none {
  template <class N>
  bool operator() (N) const { return false; }
};

template <class T>
struct filter_column_of
{
  bool operator()(const T& val) const {
    return false;
  }
};

template <class T>
struct filter_range_column_of
{
  T min_;
  T max_;

  filter_range_column_of(const T& min, const T& max)
    : min_(min), max_(max)
  { }
  bool operator()(const T& val) const {
    return min_ <= val && val <= max_;
  }
};

template <class T, class Filter>
struct filter_update_min_max
{
  T min_;
  T max_;
  bool valid_;
  Filter filter_;

  filter_update_min_max(Filter filter)
    : valid_(false), filter_(filter)
  { }
  bool operator()(const T& val) {
    if (filter(val))
      return true;
    if (! valid_) {
      min_ = max_ = val;
      valid_ = true;
    }
    else if (val < min_)
      min_ = val;
    else if (val > max_)
      max_ = val;
    return false;
  }
};

#if 0
template <class T, class Filter>
inline void
filter_column(FilterColumn& filter_col,
	      const column_of<T>& col,
	      Filter filter)
{
  int end = col.size();
  for (int i = 0; i < end; i++) {
    filter_col &&= filter(col[i]);
  }
}
#endif

} // namespace infovis

#endif // INFOVIS_TABLE_FILTER_HPP


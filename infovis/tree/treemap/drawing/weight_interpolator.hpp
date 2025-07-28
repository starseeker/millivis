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
#ifndef INFOVIS_TREE_TREEMAP_DRAWING_WEIGHT_INTERPOLATOR_HPP
#define INFOVIS_TREE_TREEMAP_DRAWING_WEIGHT_INTERPOLATOR_HPP

#include <vector>
#ifndef NDEBUG
#include <cassert>
#include <cfloat>
#include <iostream>
#endif

namespace infovis {

/**
 * Interpolates between two columns.
 */
template <class DAContainer>
struct weight_interpolator
{
  using value_type = typename DAContainer::value_type;

  const DAContainer* from;
  const DAContainer* to;
  float param;
  float balance;

  weight_interpolator(const DAContainer& f, const DAContainer& t) noexcept
    : from(&f), to(&t), param(0), balance(1) {}

  weight_interpolator(const weight_interpolator& other) = default;

  weight_interpolator& operator=(const weight_interpolator& other) = default;
  
  void set_param(float p) noexcept { param = p; }
  float get_param() const noexcept { return param; }
  void set_balance(float b) noexcept { balance = b; }
  float get_balance() const noexcept { return balance; }
  void set_std_balance() { balance = (*from)[0] / (*to)[0]; }

  value_type operator[](int i) const {
    float f = (*from)[i];
    float t = (*to)[i];
    float ret = (1.0f - param) * f + param * t * balance;
#if 0
    if (std::isnan(ret)) {
      std::cerr << "nan found at index " << i << std::endl;
      return 0.0f;
    }
#endif
    return ret;
  }
};

template <class T>
inline typename weight_interpolator<T>::value_type
get(const weight_interpolator<T>& pa, int k) { return pa[k]; }

} // namespace infovis

#endif // INFOVIS_TREE_TREEMAP_DRAWING_WEIGHT_INTERPOLATOR_HPP

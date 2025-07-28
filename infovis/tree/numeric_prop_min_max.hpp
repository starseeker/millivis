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
#ifndef INFOVIS_TREE_NUMERIC_PROP_MIN_MAX_HPP
#define INFOVIS_TREE_NUMERIC_PROP_MIN_MAX_HPP

#include <infovis/tree/property_tree.hpp>
#include <limits>

namespace infovis {

inline void
numeric_prop_min_max(const property_tree::numeric_prop& prop,
		     float& min_value,
		     float& max_value)
{
  property_tree::numeric_prop::const_iterator i = prop.begin();
  if (i == prop.end()) {
    min_value = 0;
    max_value = 0;
    return;
  }
  min_value = max_value = *i++;

  while (i != prop.end()) {
    if (*i < min_value)
      min_value = *i;
    else if (*i > max_value)
      max_value = *i;
    i++;
  }
}

} // namespace infovis

#endif // INFOVIS_TREE_NUMERIC_PROP_MIN_MAX_HPP

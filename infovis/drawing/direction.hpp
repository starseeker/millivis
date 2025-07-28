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
#ifndef INFOVIS_DRAWING_DIRECTION_HPP
#define INFOVIS_DRAWING_DIRECTION_HPP

#include <infovis/alloc.hpp>

namespace infovis {

/**
 * direction data type holding four possible directions.
 */
enum direction {
  left_to_right,
  right_to_left,
  top_to_bottom,
  bottom_to_top
};

inline direction flip(direction d)
{
  switch(d) {
  case left_to_right:
    return top_to_bottom;
  case right_to_left:
    return bottom_to_top;
  case top_to_bottom:
    return left_to_right;
  case bottom_to_top:
    return right_to_left;
  }
  return left_to_right;
}
  
inline direction turn90(direction d)
{
  switch(d) {
  case left_to_right:
    return top_to_bottom;
  case right_to_left:
    return bottom_to_top;
  case top_to_bottom:
    return right_to_left;
  case bottom_to_top:
    return left_to_right;
  }
}
inline direction turn180(direction d)
{
  switch(d) {
  case left_to_right:
    return right_to_left;
  case right_to_left:
    return left_to_right;
  case top_to_bottom:
    return bottom_to_top;
  case bottom_to_top:
    return top_to_bottom;
  }
  return right_to_left;
}

};

#endif // INFOVIS_DRAWING_DIRECTION_HPP

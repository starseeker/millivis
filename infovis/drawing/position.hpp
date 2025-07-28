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
#ifndef INFOVIS_DRAWING_POSITION_HPP
#define INFOVIS_DRAWING_POSITION_HPP

#include <infovis/alloc.hpp>
#include <infovis/drawing/direction.hpp>

namespace infovis {

/**
 * Position data type
 */
enum position {
  pos_left,
  pos_bottom,
  pos_right,
  pos_top,
  pos_center
};

inline position default_position(direction d)
{
  switch(d) {
  case left_to_right:
    return pos_left;
  case right_to_left:
    return pos_right;
  case top_to_bottom:
    return pos_top;
  case bottom_to_top:
    return pos_bottom;
  }
  return pos_left;
}

inline position turn90(position d)
{
  switch(d) {
  case pos_left:
    return pos_bottom;
  case pos_bottom:
    return pos_right;
  case pos_right:
    return pos_top;
  case pos_top:
    return pos_left;
  case pos_center:
    return pos_center;
  }
}

inline position turn180(position d)
{
  switch(d) {
  case pos_left:
    return pos_right;
  case pos_bottom:
    return pos_top;
  case pos_right:
    return pos_left;
  case pos_top:
    return pos_bottom;
  case pos_center:
    return pos_center;
  }
}

};

#endif // INFOVIS_DRAWING_POSITION_HPP

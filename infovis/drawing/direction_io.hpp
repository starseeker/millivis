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
#ifndef INFOVIS_DRAWING_DIRECTION_IO_HPP
#define INFOVIS_DRAWING_DIRECTION_IO_HPP

#include <infovis/drawing/direction.hpp>
#include <iostream>

namespace infovis {

inline const char * to_string(direction d)
{
  switch(d) {
  case left_to_right:
    return "left_to_right";
    break;
  case right_to_left:
    return "right_to_left";
    break;
  case top_to_bottom:
    return "top_to_bottom";
    break;
  case bottom_to_top:
    return "bottom_to_top";
    break;
  }
  return "";
}


inline std::ostream& operator << (std::ostream& out, direction d)
{
  return out << to_string(d);
}

}

#endif // INFOVIS_DRAWING_DIRECTION_IO_HPP

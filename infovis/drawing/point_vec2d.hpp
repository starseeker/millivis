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
#ifndef INFOVIS_DRAWING_POINT_VEC2D_HPP
#define INFOVIS_DRAWING_POINT_VEC2D_HPP

#include <infovis/drawing/point_vec.hpp>

namespace infovis {

/**
 * Point in 2D template
 */
template <class Coord>
struct point2d : point_<2, Coord>
{
  typedef point_<2, Coord> super;

  point2d() { }

  point2d(const point2d& p) : point_<2, Coord>(p) { }

  template <class Coord2>
  point2d(const point_<2, Coord2>& p) : super(p) {}

#ifndef __GNUG__
  template <class Coord2>
  explicit point2d(const vector_<2,Coord2>& v) : super(v) {}
#endif

  point2d(const Coord& x, const Coord& y) {
    this->at(0) = x;
    this->at(1) = y;
  }

};

template <class Coord>
inline const Coord&
x(const point_<2, Coord>& p) { return p[0]; }

template <class Coord>
inline const Coord&
y(const point_<2, Coord>& p) { return p[1]; }

template <class Coord>
inline const Coord&
set_x(point_<2, Coord>& p, const Coord& v) {
  return p[0] = v;
}

template <class Coord>
inline const Coord&
set_y(point_<2, Coord>& p, const Coord& v) {
  return p[1] = v;
}

/**
 * Vector in 2D template
 */
template <class Coord>
struct vector2d : vector_<2, Coord>
{
  typedef vector_<2, Coord> super;

  vector2d () { }

  vector2d(const vector2d& v) : super(v) {}

  template <class Coord2>
  vector2d(const vector_<2, Coord2> & v) : super(v) {}

#ifndef __GNUG__
  template <class Coord2>
  explicit vector2d(const point_<2,Coord2>& p) : super(p) {}
#endif

  vector2d(const Coord& dx, const Coord& dy) {
    this->at(0) = dx;
    this->at(1) = dy;
  }

};

template <class Coord>
inline const Coord& dx(const vector_<2,Coord>& v) { return v[0]; }

template <class Coord>
inline const Coord& dy(const vector_<2,Coord>& v) { return v[1]; }

template <class Coord>
inline const Coord&
set_dx(vector_<2,Coord>& v, const Coord& val) {
    return v[0] = val;
}

template <class Coord>
inline const Coord&
set_dy(vector_<2,Coord>& v, const Coord& val) {
    return v[1] = val;
}

} // namespace infovis

#endif // INFOVIS_DRAWING_POINT_VEC2D_HPP

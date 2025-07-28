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
#ifndef INFOVIS_DRAWING_BOX_HPP
#define INFOVIS_DRAWING_BOX_HPP

#include <infovis/drawing/point_vec2d.hpp>
#include <algorithm>

namespace infovis {

/**
 * Traits for box types
 */
template <class Box>
struct box_traits
{
  typedef typename Box::coord_type coord_type;
  typedef typename Box::dist_type dist_type;
  typedef typename Box::point_type point_type;
  typedef typename Box::vector_type vector_type;
};

/**
 * Box type holding a min and max points
 */
template <class Coord, class UCoord = Coord>
struct box_min_max
{
  typedef Coord coord_type;
  typedef UCoord dist_type;
  typedef box_min_max<Coord,UCoord> self;
  typedef point2d<Coord> point_type;
  typedef vector2d<UCoord> vector_type;

  point_type min_, max_;

  box_min_max() : min_(0, 0), max_(0, 0) { }
  explicit box_min_max(const Coord& val)
    : min_(val, val), max_(val, val) { }

  template <class Box>
  explicit box_min_max(const Box& b)
  : min_(min(b)), max_(max(b)) { }

  box_min_max(const Coord& xm, const Coord& ym,
	      const Coord& xM, const Coord& yM) 
    : min_(xm, ym), max_(xM, yM) {}

  template <class Coord2>
  box_min_max(const point2d<Coord2>& m, const point2d<Coord2>& M)
    : min_(m), max_(M) { }

  template <class Coord2, class UCoord2>
  box_min_max(const point2d<Coord2>& m, const vector2d<UCoord2>& e)
    : min_(m), max_(m+e) { }

  template <class Box>
  bool operator == (const Box& b) const {
    return
      min_ == min(b) &&
      max_ == max(b);
  }
  template <class Box>
  bool operator != (const Box& b) const {
    return ! ((*this) == b);
  }
};

template <class Coord,class UCoord>
inline const Coord&
xmin(const box_min_max<Coord,UCoord>& b) { return x(b.min_); }

template <class Coord,class UCoord>
inline const Coord&
set_xmin(box_min_max<Coord,UCoord>& b, const Coord& v) { return set_x(b.min_, v); }

template <class Coord,class UCoord>
inline const Coord&
ymin(const box_min_max<Coord,UCoord>& b) { return y(b.min_); }

template <class Coord,class UCoord>
inline const Coord&
set_ymin(box_min_max<Coord,UCoord>& b, const Coord& v) { return set_y(b.min_, v); }

template <class Coord,class UCoord>
inline const Coord&
xmax(const box_min_max<Coord,UCoord>& b) { return x(b.max_); }

template <class Coord,class UCoord>
inline const Coord&
set_xmax(box_min_max<Coord,UCoord>& b, const Coord& v) { return set_x(b.max_, v); }

template <class Coord,class UCoord>
inline const Coord&
ymax(const box_min_max<Coord,UCoord>& b) { return y(b.max_); }

template <class Coord,class UCoord>
inline const Coord&
set_ymax(box_min_max<Coord,UCoord>& b, const Coord& v) { return set_y(b.max_, v); }

template <class Coord,class UCoord>
inline UCoord
width(const box_min_max<Coord,UCoord>& b) { return xmax(b) - xmin(b); }

template <class Coord,class UCoord>
inline UCoord
set_width(box_min_max<Coord,UCoord>& b, const UCoord& v) {
  set_xmax(b, xmin(b) + v);
  return v;
}

template <class Coord,class UCoord>
inline UCoord
height(const box_min_max<Coord,UCoord>& b) { return ymax(b) - ymin(b); }

template <class Coord,class UCoord>
inline const UCoord&
set_height(box_min_max<Coord,UCoord>& b, const UCoord& v) {
  set_ymax(b, ymin(b) + v);
  return v;
}

template <class Coord,class UCoord>
inline const point2d<Coord>&
min(const box_min_max<Coord,UCoord>& b) { return b.min_; }

template <class Coord,class UCoord>
inline const point2d<Coord>&
set_min(box_min_max<Coord,UCoord>& b, const point2d<Coord>& v) {
  return b.min_ = v;
}

template <class Coord,class UCoord>
inline const point2d<Coord>&
max(const box_min_max<Coord,UCoord>& b) { return b.max_; }

template <class Coord,class UCoord>
inline const point2d<Coord>&
set_max(box_min_max<Coord,UCoord>& b, const point2d<Coord>& v) {
  return b.max_ = v;
}

template <class Coord,class UCoord>
inline const vector2d<UCoord>
extent(const box_min_max<Coord,UCoord>& b) { return b.max_ - b.min_; }

template <class Coord,class UCoord>
inline const vector2d<UCoord>&
set_extent(box_min_max<Coord,UCoord>& b, const vector2d<UCoord>& v) {
  b.max_ = b.min_ + v;
  return v;
}

template <class Coord,class UCoord>
inline void
normalize(box_min_max<Coord,UCoord>& b)
{
  if (xmax(b) < xmin(b)) {
    Coord tmp = xmax(b);
    set_xmax(b, xmin(b));
    set_xmin(b, tmp);
  }
  if (ymax(b) < ymin(b)) {
    Coord tmp = ymax(b);
    set_ymax(b, ymin(b));
    set_ymin(b, tmp);
  }
}

template <class Coord, class UCoord>
inline void
translate(box_min_max<Coord,UCoord>& b,
	  const UCoord& dx, const UCoord& dy)
{
  const vector2d<UCoord> v(dx, dy);
  b.min_ += v;
  b.max_ += v;
}

template <class Coord, class UCoord>
inline void
translate(box_min_max<Coord,UCoord>& b,
	  const vector2d<UCoord>& v)
{
  b.min_ += v;
  b.max_ += v;
}

template <class Box>
inline bool
add_point(Box& b,
	  const typename box_traits<Box>::coord_type& x,
	  const typename box_traits<Box>::coord_type& y)
{
  bool changed = false;
  if (x < xmin(b)) {
    changed = true;
    set_xmin(b, x);
  }
  else if (xmax(b) < x) {
    changed = true;
    set_xmax(b, x);
  }
  if (y < ymin(b)) {
    changed = true;
    set_ymin(b, y);
  }
  else if (ymax(b) < y) {
    changed = true;
    set_ymax(b, y);
  }
  return changed;
}

template <class Box>
inline Box
grow_box(const Box& b,
	 const typename box_traits<Box>::coord_type& dx,
	 const typename box_traits<Box>::coord_type& dy)
{
  return Box(xmin(b)-dx, ymin(b)-dy,
	     xmax(b)+dx, ymax(b)+dy);
}

template <class Box>
inline bool
inside(const Box& b,
       const typename box_traits<Box>::coord_type& x,
       const typename box_traits<Box>::coord_type& y)
{
  return ! (x < xmin(b) || xmax(b) < x ||
	    y < ymin(b) || ymax(b) < y);
}

template <class Box>
inline bool
intersects(const Box& b1, const Box& b2)
{
  return !
    (xmax(b2) <= xmin(b1)  || xmax(b1) <= xmin(b2) ||
     ymax(b2) <= ymin(b1)  || ymax(b1) <= ymin(b2));
}

template <class Box>
inline bool
contains(const Box& b1, const Box& b2)
{
  return (xmin(b1) <= xmin(b2) && xmax(b1) >= xmax(b2) &&
	  ymin(b1) <= ymin(b2) || ymax(b1) >= ymax(b2));
}

template <class Box>
inline bool
empty(const Box& b)
{
  return xmin(b) == xmax(b) || ymin(b) == ymax(b);
}

template <class Box>
inline typename box_traits<Box>::point_type
center(const Box& b)
{
  return min(b) + extent(b)/2;
}

template <class Box>
inline Box
box_union(const Box& b1, const Box& b2)
{
  return Box(std::min(xmin(b1), xmin(b2)),
	     std::min(ymin(b1), ymin(b2)),
	     std::max(xmax(b1), xmax(b2)),
	     std::max(ymax(b1), ymax(b2)));
}

template <class Box>
inline Box
box_intersection(const Box& b1, const Box& b2)
{
  if (xmax(b1) < xmin(b2) ||
      xmax(b2) < xmin(b1) ||
      ymax(b1) < ymin(b2) ||
      ymax(b2) < ymin(b1))
    return Box();
  return Box(std::max(xmin(b1), xmin(b2)),
	     std::max(ymin(b1), ymin(b2)),
	     std::min(xmax(b1), xmax(b2)),
	     std::min(ymax(b1), ymax(b2)));
}

/**
 * Box type holding a min point and an extent vector
 */
template <class Coord, class UCoord = Coord>
struct box_min_extent
{
  typedef Coord coord_type;
  typedef UCoord dist_type;
  typedef box_min_extent<Coord,UCoord> self;
  typedef point2d<Coord> point_type;
  typedef vector2d<UCoord> vector_type;

  point_type min_;
  vector_type extent_;

  box_min_extent() : min_(0, 0), extent_(0, 0) {}
  box_min_extent(const self& b)
    : min_(min(b)), extent_(extent(b)) { }
  template <class Box>
  explicit box_min_extent(const Box& b)
    : min_(min(b)), extent_(extent(b)) {}
  box_min_extent(const Coord& xm, const Coord& ym,
		 const UCoord& w, const UCoord& h) 
    : min_(xm, ym), extent_(w, h) {}

  template <class Coord2>
  box_min_extent(const point2d<Coord2>& m, const point2d<Coord2>& M)
    : min_(m), extent_(M-m) { }

  template <class Coord2, class UCoord2>
  box_min_extent(const point2d<Coord2>& m, const vector2d<UCoord2>& e)
    : min_(m), extent_(e) { }
    
  template <class Box>
  bool operator == (const Box& b) const {
    return
      min_ == min(b) &&
      extent_ == extent(b);
  }
  template <class Box>
  bool operator != (const Box& b) const {
    return ! ((*this) == b);
  }
};
	
template <class Coord,class UCoord>
inline const Coord&
xmin(const box_min_extent<Coord,UCoord>& b) { return x(b.min_); }

template <class Coord,class UCoord>
inline const Coord&
set_xmin(box_min_extent<Coord,UCoord>& b, const Coord& v) { return set_x(b.min_, v); }

template <class Coord,class UCoord>
inline const Coord&
ymin(const box_min_extent<Coord,UCoord>& b) { return y(b.min_); }

template <class Coord,class UCoord>
inline const Coord&
set_ymin(box_min_extent<Coord,UCoord>& b, const Coord& v) { return set_y(b.min_, v); }

template <class Coord,class UCoord>
inline Coord
xmax(const box_min_extent<Coord,UCoord>& b) { return x(b.min_) + dx(b.extent_); }

template <class Coord,class UCoord>
inline const Coord&
set_xmax(box_min_extent<Coord,UCoord>& b, const Coord& v) {
  set_dx(b.extent_, UCoord(v - x(b.min_)));
  return v;
}

template <class Coord,class UCoord>
inline Coord
ymax(const box_min_extent<Coord,UCoord>& b) { return y(b.min_) + dy(b.extent_); }

template <class Coord,class UCoord>
inline const Coord&
set_ymax(box_min_extent<Coord,UCoord>& b, const Coord& v) {
  set_dy(b.extent_, UCoord(v - y(b.min_)));
  return v;
}

template <class Coord,class UCoord>
inline const UCoord&
width(const box_min_extent<Coord,UCoord>& b) { return dx(b.extent_); }

template <class Coord,class UCoord>
inline const UCoord&
set_width(box_min_extent<Coord,UCoord>& b, const UCoord& v) {
  return set_dx(b.extent_, v);
}

template <class Coord,class UCoord>
inline const UCoord&
height(const box_min_extent<Coord,UCoord>& b) { return dy(b.extent_); }

template <class Coord,class UCoord>
inline const UCoord&
set_height(box_min_extent<Coord,UCoord>& b, const UCoord& v) {
  return set_dy(b.extent_, v); }

template <class Coord,class UCoord>
inline const point2d<Coord>&
min(const box_min_extent<Coord,UCoord>& b) { return b.min_; }

template <class Coord,class UCoord>
inline const point2d<Coord>&
set_min(box_min_extent<Coord,UCoord>& b, const point2d<Coord>& v) {
  return b.min_ = v;
}

template <class Coord,class UCoord>
inline const point2d<Coord>
max(const box_min_extent<Coord,UCoord>& b) {
  return b.min_ + vector2d<Coord>(b.extent_); }

template <class Coord,class UCoord>
inline point2d<Coord>
set_max(box_min_extent<Coord,UCoord>& b, const point_<2, Coord>& p) {
  const vector_<2,Coord> e(b.extent_);
  b.min_ = p - e;
  return p;
}

template <class Coord,class UCoord>
inline const vector2d<UCoord>&
extent(const box_min_extent<Coord,UCoord>& b) { return b.extent_; }

template <class Coord,class UCoord>
inline const vector2d<UCoord>&
set_extent(box_min_extent<Coord,UCoord>& b, const vector2d<UCoord>& v) {
  return b.extent_ = v;
}

template <class Coord, class UCoord>
inline void
translate(box_min_extent<Coord,UCoord>& b,
	  const UCoord& dx, const UCoord& dy)
{
  const vector2d<Coord> v(dx, dy);
  b.min_ += v;
}

template <class Coord, class UCoord>
inline void
translate(box_min_extent<Coord,UCoord>& b,
	  const vector2d<UCoord>& v)
{
  b.min_ += v;
}
  

#if 0 
template <class Coord, class UCoord>
template <class Box>
box_min_max<Coord,UCoord>::box_min_max<Coord,UCoord>(const Box& b)
  : min_(min(b)), max_(max(b)) { }
#endif


} // namespace infovis

#endif // INFOVIS_TREE_DRAW_BOX_HPP

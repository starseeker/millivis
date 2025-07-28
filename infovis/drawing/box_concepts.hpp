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
#ifndef INFOVIS_DRAWING_BOX_CONCEPTS_HPP
#define INFOVIS_DRAWING_BOX_CONCEPTS_HPP

#include <boost/concept_check.hpp>
#include <infovis/drawing/box.hpp>

namespace infovis {

/**
 * Box Concept (Abstract requirements for a box).
 */
template <class Box>
struct BoxConcept
{
  typedef typename box_traits<Box>::coord_type coord_type;
  typedef typename box_traits<Box>::dist_type dist_type;
  typedef typename box_traits<Box>::point_type point_type;
  typedef typename box_traits<Box>::vector_type vector_type;

  void constraints() {
    boost::function_requires< boost::EqualityComparableConcept<coord_type> >();
    boost::function_requires< boost::ComparableConcept<coord_type> >();
    boost::function_requires< boost::AssignableConcept<coord_type> >();

    x = xmin(b);
    y = ymin(b);
    x = xmax(b);
    y = ymax(b);
    u = width(b);
    v = height(b);
    pt = min(b);
    pt = max(b);
    vec = extent(b);
    x = set_xmin(b, x);
    y = set_ymin(b, y);
    x = set_xmax(b, x);
    y = set_ymax(b, y);
    u = set_width(b, u);
    v = set_height(b, v);
    pt = set_min(b, pt);
    pt = set_max(b, pt);
    vec = set_extent(b, vec);
    translate(b, u, v);
    translate(b, vec);
    add_point(b, x, y);
    const_constraints(b);
  }
  void const_constraints(const Box&  b) {
    coord_type x, y;
    dist_type u, v;
    point_type pt;
    vector_type vec;

    x = xmin(b);
    y = ymin(b);
    x = xmax(b);
    y = ymax(b);
    u = width(b);
    v = height(b);
    pt = min(b);
    pt = max(b);
    vec = extent(b);
    bool bv = inside(b, x, y);
    bv = intersects(b, b);
    bv = contains(b, b);
    bv = empty(b);
  }

  coord_type x, y;
  dist_type u, v;
  point_type pt;
  vector_type vec;
  Box b;
};

} // namespace infovis

#endif

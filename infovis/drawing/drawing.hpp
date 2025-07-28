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
#ifndef INFOVIS_DRAWING_BASE_HPP
#define INFOVIS_DRAWING_BASE_HPP

#include <infovis/alloc.hpp>
#include <infovis/drawing/point_vec2d.hpp>
#include <infovis/drawing/box.hpp>
#include <infovis/drawing/color.hpp>
#include <string>
using std::string;

namespace infovis {

typedef float Coord;
typedef point2d<Coord> Point;
typedef vector2d<Coord> Vector;
typedef box_min_max<Coord> Box;
typedef color_rgba<unsigned char> Color;
extern const Color color_none;
extern const Color color_white;
extern const Color color_black;
extern const Color color_red;
extern const Color color_green;
extern const Color color_blue;
extern const Color color_cyan;
extern const Color color_magenta;
extern const Color color_yellow;
extern const Color color_half_grey;

} // namespace infovis

#endif // INFOVIS_DRAWING_BASE_HPP

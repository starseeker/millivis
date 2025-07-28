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
#ifndef INFOVIS_DRAWING_POINT_VEC_IO_HPP
#define INFOVIS_DRAWING_POINT_VEC_IO_HPP

#include <iostream>
#include <typeinfo>
#include <cstring>
#include <infovis/drawing/point_vec.hpp>

namespace infovis {

#define CFOREACH for (int i = 0; i < Dim; i++)

template <int Dim, class Coord>
inline std::ostream&
operator << (std::ostream& out, const point_<Dim, Coord>& p)
{
  out << typeid(p).name() <<"(";
  //out << "point<" << Dim << ">::(" << p[0];
  CFOREACH { out << ((i == 0) ? "" : ",") << p[i]; }
  return out << ')';
}

template <int Dim, class Coord>
inline std::istream&
operator >> (std::istream& in, point_<Dim, Coord>& p)
{
  in.ignore(strlen(typeid(p).name())+1);
  CFOREACH { in >> p[i]; in.ignore(1); }
  return in;
}

template <int Dim, class Coord>
inline std::ostream&
operator << (std::ostream& out, const vector_<Dim, Coord>& v)
{
  out << typeid(v).name() <<"(";
  CFOREACH { out << (i==0? "" : ",") << v[i]; }
  return out << ')';
}

template <int Dim, class Coord>
inline std::istream&
operator >> (std::istream& in, vector_<Dim, Coord>& v)
{
  in.ignore(strlen(typeid(p).name()+1));
  CFOREACH { in >> v[i]; in.ignore(1); }
  return in;
}
#undef CFOREACH
} // namespace

#endif // INFOVIS_DRAWING_POINT_VEC_IO_HPP

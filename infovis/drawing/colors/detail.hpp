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
#ifndef INFOVIS_DRAWING_COLORS_DETAILS_HPP
#define INFOVIS_DRAWING_COLORS_DETAILS_HPP

#include <limits>

namespace infovis {

namespace detail {
template <class T>
inline T one(const T&, int&)
{
  return std::numeric_limits<T>::max();
}

template <class T>
inline T one(const T&, float&)
{
  return T(1);
}

template <class T>
inline T one(const T& t)
{
  typename boost::detail::if_true<(std::numeric_limits<T>::is_integer)>
    ::template then<int,float>::type int_or_float;
  return one(t, int_or_float);
}


template <class S, class T>
inline void convert(S& s, int, const T& t, int)
{
  s = t * one(s) / one(t);
}

template <class S, class T>
inline void convert(S& s, float, const T& t, int)
{
  s = S(t / S(one(t)));
}

template <class S, class T>
inline void convert(S& s, int, const T& t, float)
{
  s = S(t * one(s));
}

template <class S, class T>
inline void convert(S& s, float&, const T& t, float)
{
  s = t;
}

template <class S, class T>
inline void convert(S& s, const T& t)
{
  typename boost::detail::if_true<(std::numeric_limits<S>::is_integer)>
    ::template then<int,float>::type int_or_float1(0);
  typename boost::detail::if_true<(std::numeric_limits<T>::is_integer)>
    ::template then<int,float>::type int_or_float2(0);
  convert(s, int_or_float1, t, int_or_float2);
}

inline void round(double& v, double d) { v = d; }
inline void round(float& v, double d) { v = float(d); }
inline void round(unsigned long& v, double d) { v = (unsigned long)(d + 0.5); }
inline void round(long& v, double d) { v = long(d + 0.5); }
inline void round(unsigned int& v, double d) { v = (unsigned int)(d + 0.5); }
inline void round(int& v, double d) { v = int(d + 0.5); }
inline void round(unsigned short& v, double d) { v = (unsigned short)(d + 0.5); }
inline void round(short& v, double d) { v = short(d + 0.5); }
inline void round(unsigned char& v, double d) { v = (unsigned char)(d + 0.5); }
inline void round(char& v, double d) { v = char(d + 0.5); }

} // namespace detail

} // namespace infovis

#endif

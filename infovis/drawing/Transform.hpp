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
#ifndef INFOVIS_DRAWING_TRANSFORM_HPP
#define INFOVIS_DRAWING_TRANSFORM_HPP

#include <infovis/drawing/drawing.hpp>
#include <vector>
#include <cstring> // TODO: Added for memset/memcpy - C++17 modernization

namespace infovis {

/**
 * Affine transforms
 */
class Transform
{
protected:
  typedef std::vector<float, gc_alloc<float,true> > ParamList;
public:
  /**
   * Internal state of an Affine Transform
   */
  typedef struct Matrix
  {
    float m[6];
    Matrix() { memset(m, 0, sizeof(m)); }
    explicit Matrix(float f) {
      m[0] = f;
      m[1] = 0;
      m[2] = f;
      m[3] = 0;
      m[4] = 0;
      m[5] = 0;
    }
    //Matrix(const float M[6]) { memcpy(m, M, sizeof(m)); }
    Matrix(const Matrix& M) { memcpy(m, &M, sizeof(m)); }
    Matrix(ParamList::const_iterator i) {
      m[0] = *i++;
      m[1] = *i++;
      m[2] = *i++;
      m[3] = *i++;
      m[4] = *i++;
      m[5] = *i++;
    }
    float& operator[] (int i) { return m[i]; }
    const float operator[] (int i) const { return m[i]; }
  };
  void translate(const Vector& v);
  void scale(const Vector& v);
  void rotate(float angle);
  void matrix(const Matrix& );
  void matrix(float sxx, float sxy, float syx, float syy,
	      float tx, float ty);
  void apply() const;
  void apply(Matrix&) const;

protected:
  enum op {
    op_trans,
    op_scale,
    op_rotate,
    op_matrix
  };
  typedef std::vector<op, gc_alloc<op,true> > OpList;
  OpList oplist;
  ParamList paramlist;

};
    
  inline Transform::Matrix
  operator * (const Transform::Matrix& m1, const Transform::Matrix& m2)
  {
    Transform::Matrix ret;
    ret[0] = m1[0] * m2[0] + m1[2] * m2[1];
    ret[1] = m1[1] * m2[0] + m1[3] * m2[1];
    ret[2] = m1[0] * m2[1] + m1[2] * m2[3];
    ret[3] = m1[1] * m2[1] + m1[3] * m2[3];
    ret[4] = m1[0] * m2[4] + m1[2] * m2[5] + m1[4];
    ret[5] = m1[1] * m2[4] + m1[3] * m2[5] + m1[5];
    return ret;
  }

  inline Point
  operator * (const Transform::Matrix& m, const Point& pt)
  {
    return Point(m[0] * x(pt) + m[2] * y(pt) + m[4],
		 m[1] * x(pt) + m[3] * y(pt) + m[5]);
  }

  inline Vector
  operator * (const Transform::Matrix& m, const Vector& pt)
  {
    return Vector(m[0] * dx(pt) + m[2] * dy(pt),
		  m[1] * dx(pt) + m[3] * dy(pt));
  }
} // namespace infovis 


#endif

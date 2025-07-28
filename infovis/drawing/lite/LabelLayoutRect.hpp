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
#ifndef INFOVIS_DRAWING_LITE_LABELLAYOUTRECT_HPP
#define INFOVIS_DRAWING_LITE_LABELLAYOUTRECT_HPP

#include <infovis/drawing/lite/LabelLayout.hpp>

namespace infovis {

/**
 * Dynamic labeling inside a rectangle
 */
class LabelLayoutRect : public LabelLayout
{
public:
  LabelLayoutRect(const Box& box, bool x_stable = false);

  virtual LayoutResult doLayout();

  
  virtual bool isXStable() const;
  virtual void setXStable(bool v);
  
protected:
  typedef std::vector<int> Stack;
  typedef std::vector<float> Pos;
  void record_offsets();
  Point nearest_point(const Point& from) const;
  void move_labels_initially();
  LayoutResult compute_left_right();
  void align_left(float max_width) const;
  void align_right(float max_width) const;
  float compute_max_width(const Stack& s);
  void optimize_origin(const Stack&, float height, Pos& y_pos) const;
  float compute_offset(float dy0, float dy1) const;

  bool x_stable_;
  std::vector<float> x_offsets_;
  Stack left_;
  Stack right_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_LABELLAYOUTRECT_HPP

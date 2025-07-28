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
#include <BarGraph.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <infovis/tree/numeric_prop_min_max.hpp>
#include <algorithm>

namespace infovis {


BarGraph::BarGraph(unsigned bars)
  : bars_(bars)
{
  reset();
}

void
BarGraph::reset(float val)
{
  min_ = 0;
  max_ = 0;
  std::fill(bars_.begin(), bars_.end(), val);
}

void
BarGraph::setValue(unsigned index, float value)
{
  bars_[index] = value;
  if (value < min_)
    min_ = value;
  else if (value > max_)
    max_ = value;
}

float
BarGraph::getValue(unsigned index) const
{
  return bars_[index];
}

void
BarGraph::computeDistribution(const FloatColumn& values)
{
#ifdef VECTOR_AS_TREE
  float min_val, max_val;
  numeric_prop_min_max(values, min_val, max_val);
  computeDistribution(values, min_val, max_val);
#else
  computeDistribution(values, values.min(), values.max());
#endif
}

void
BarGraph::computeDistribution(const FloatColumn& values,
			      float min_val, float max_val)
{
  float step = (max_val - min_val + 1) / bars_.size();
  float gap = bars_.size() / (max_val - min_val + 1);

  reset(0);

  for (const auto& value : values) {
    if (value < min_val || value > max_val)
      continue;
    int index = int((value - min_val) / step);
    for (int j = index; j <= int(index + gap); j++)
      bars_[j]++;
  }
  max_ = min_ = bars_[0];
  for (auto it = bars_.begin() + 1; it != bars_.end(); ++it) {
    const float v = *it;
    if (v < min_)
      min_ = v;
    else if (v > max_)
      max_ = v;
  }
}


void
BarGraph::updateMinMax()
{
  if (min_ < max_ || bars_.empty())
    return;

  max_ = min_ = bars_[0];
  for (auto it = bars_.begin() + 1; it != bars_.end(); ++it) {
    const float v = *it;
    if (v < min_)
      min_ = v;
    else if (v > max_)
      max_ = v;
  }
}

void
BarGraph::render(const Box& box, direction dir) const
{
  glPushMatrix();
  switch(dir) {
  case left_to_right:
    glTranslatef(xmin(box), ymin(box), 0);
    glScalef(width(box), height(box), 1);
    break;
  case right_to_left:
    glScalef(-width(box), height(box), 1);
    glTranslatef(xmax(box), ymin(box), 0);
    break;
  case top_to_bottom:
  case bottom_to_top:
    glRotatef(90, 0, 0, 1);
    glScalef(width(box), height(box), 1);
    glTranslatef(xmin(box), ymax(box), 0);
    break;
  }
  glScalef(1.0f/(bars_.size()+1), 1.0f/(max_ - min_ + 1), 1);
  glBegin(GL_QUAD_STRIP);
  glVertex2f(0, 0);
  glVertex2f(1, bars_[1]-min_);
  for (size_t i = 0; i < bars_.size(); i++) {
    glVertex2f(static_cast<float>(i), 0);
    glVertex2f(static_cast<float>(i), bars_[i]-min_);
  }
  glEnd();
  glPopMatrix();
}

} // namespace infovis

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
#include <infovis/drawing/lite/LabelLayout.hpp>

namespace infovis {

LabelLayout::LabelLayout(const Box& box)
  : bounds_(box),
    list_(),
    center_(0, 0),
    radius_(0)
{ }

LabelLayout::~LabelLayout()
{ }

LabelLayout::LayoutResult
LabelLayout::layout(const List& list, const Point& center, float radius)
{
  list_ = list;
  center_ = center;
  radius_ = radius;
  return doLayout();
}

Box
LabelLayout::getBounds() const
{
  return bounds_;
}

void
LabelLayout::setBounds(const Box& box)
{
  bounds_ = box;
}

const LabelLayout::List&
LabelLayout::getList() const { return list_; }

void
LabelLayout::setList(const LabelLayout::List& v) {list_ = v; }

const Point&
LabelLayout::getCenter() const { return center_; }

void
LabelLayout::setCenter(const Point& v) {center_ = v; }

float
LabelLayout::getRadius() const { return radius_; }

void
LabelLayout::setRadius(float v) {radius_ = v; }

LiteDisplacedLabel *
LabelLayout::getLabel(int i) const
{
  if (i < 0 || i > list_.size())
    return 0;
  return list_[i];
}

int
LabelLayout::getLabelCount() const
{
  return list_.size();
}

} // namespace infovis


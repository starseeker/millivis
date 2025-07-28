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
#include <infovis/drawing/lite/LabelLayoutRect.hpp>
#include <list>
#include <algorithm>

namespace infovis {

LabelLayoutRect::LabelLayoutRect(const Box& box, bool x_stable)
  : LabelLayout(box),
    x_stable_(x_stable),
    left_(),
    right_()
{ }

LabelLayout::LayoutResult
LabelLayoutRect::doLayout()
{
  if (list_.size() == 0)
    return layout_dont_overlap;
  if (isXStable())
    record_offsets();
  move_labels_initially();
  return compute_left_right();
}

bool
LabelLayoutRect::isXStable() const { return x_stable_; }

void
LabelLayoutRect::setXStable(bool v) {x_stable_ = v; }

void
LabelLayoutRect::record_offsets()
{
  x_offsets_.clear();
  for (List::const_iterator i = list_.begin(); i != list_.end(); i++) {
    Point p = (*i)->getPosition();
    x_offsets_.push_back(x(p) - x(center_));
  }
}

struct y_order {
  typedef LabelLayoutRect::List List;
  const List& list_;

  y_order(const List& l) : list_(l) { }

  bool operator() (int i1, int i2) const {
    const LiteDisplacedLabel * l1 = list_[i1];
    const LiteDisplacedLabel * l2 = list_[i2];
    return y(l1->getOrig()) < y(l2->getOrig());
  }
};

void
LabelLayoutRect::move_labels_initially()
{
  // Move labels to their initial position
  left_.clear();
  right_.clear();
  float left = x(center_) - radius_;
  float right = x(center_) + radius_;
  Point p;
  for (int i = 0; i < list_.size(); i++) {
    LiteDisplacedLabel * ld = list_[i];
    Point from = ld->getPosition();
    float dx = x(from) - x(center_);
    if (dx < 0) {
      p = Point(left, y(from));
      ld->setPosition(p);
      left_.push_back(i);
    }
    else {
      p = Point(right, y(from));
      ld->setPosition(p);
      right_.push_back(i);
    }
  }
  std::sort(left_.begin(), left_.end(), y_order(list_));
  std::sort(right_.begin(), right_.end(), y_order(list_));
}

Point
LabelLayoutRect::nearest_point(const Point& from) const
{
  float dx = x(from) - x(center_);
  float left = x(center_) - radius_;
  float right = x(center_) + radius_;
  if (dx < 0)
    return Point(left, y(from));
  else
    return Point(right, y(from));
}

static inline float
abs(float f)
{
  return f < 0 ? -f : f;
}

float
LabelLayoutRect::compute_max_width(const Stack& s)
{
  float max_width = 0;
  for (Stack::const_iterator i = s.begin(); i != s.end(); i++) {
    Box b(list_[(*i)]->getBounds());
    if (x_stable_) {
      max_width = std::max(max_width, width(b) + abs(x_offsets_[*i]));
    }
    else {
      max_width = std::max(max_width, width(b));
    }
  }
  return max_width;
}

LabelLayout::LayoutResult
LabelLayoutRect::compute_left_right()
{
  float max_left_width = compute_max_width(left_);
  float max_right_width = compute_max_width(right_);
  if ((x(center_) - radius_ - max_left_width) < xmin(bounds_) &&
      (x(center_) + radius_ + max_right_width) <= width(bounds_)) {
    right_.insert(right_.begin(), left_.begin(), left_.end());
    left_.clear();
    std::sort(right_.begin(), right_.end(), y_order(list_));
    align_right(max_right_width);
  }
  else if ((x(center_) + radius_ + max_right_width) > width(bounds_) &&
	   (x(center_) - radius_ - max_left_width) >= xmin(bounds_)) {
    left_.insert(left_.begin(), right_.begin(), right_.end());
    right_.clear();
    std::sort(left_.begin(), left_.end(), y_order(list_));
    align_left(std::max(max_left_width, max_right_width));
  }
  else {
    align_left(max_left_width);
    align_right(max_right_width);
  }

  return layout_dont_overlap;
}

void
LabelLayoutRect::align_left(float max_width) const
{
  if (left_.empty())
    return;
  float left = x(center_) - radius_ - max_width;
  if (left < xmin(bounds_))
    left = xmin(bounds_);
  float h = height(list_[0]->getBounds());

  Pos y_pos;
  optimize_origin(left_, h, y_pos);
#if 0
  float offset = compute_offset(y_pos.front() + y(center_),
				y_pos.back() + y(center_));
#endif
  for (int i = 0; i < left_.size(); i++) {
    LiteDisplacedLabel * ld = list_[left_[i]];
    Point p = ld->getPosition();
    if (x_stable_)
      set_x(p, left + x_offsets_[left_[i]]);
    else
      set_x(p, left);
#if 0
    set_y(p, y_pos[i] + y(center_) + offset);
#else
    set_y(p, y_pos[i]);
#endif
    ld->setJustification(LiteLabel::just_left);
    ld->setPosition(p);
    set_x(p, left + max_width);
    ld->addPath(p);
    set_x(p, left + width(ld->getBounds()));
    ld->addPath(p);
  }
}

void
LabelLayoutRect::align_right(float max_width) const
{
  if (right_.empty())
    return;
  float left = x(center_) + radius_;
  if ((left + max_width) > xmax(bounds_))
    left = xmax(bounds_) - max_width;
  float h = height(list_[0]->getBounds());

  Pos y_pos;
  optimize_origin(right_, h, y_pos);
#if 0
  float offset = compute_offset(y_pos.front() + y(center_),
				y_pos.back() + y(center_));
#endif
  for (int i = 0; i < right_.size(); i++) {
    LiteDisplacedLabel * ld = list_[right_[i]];
    Point p = ld->getPosition();
    if (x_stable_)
      set_x(p, left + x_offsets_[right_[i]]);
    else
      set_x(p, left);
#if 0
    set_y(p, y_pos[i] + y(center_) + offset);
#else
    set_y(p, y_pos[i]);
#endif
    ld->setJustification(LiteLabel::just_left);
    ld->setPosition(p);
    ld->addPath(p);
  }
}

void
LabelLayoutRect::optimize_origin(const Stack& stack, float height, Pos& y_pos) const
{
#if 1
  bool collide = true;
  typedef std::pair<int,int> Segment;
  typedef std::list<Segment, gc_alloc<Segment, true> > Segments;

  Segments segs;
  y_pos.clear();
  Stack y_orig(stack.size());

  for (int i = 0; i < stack.size(); i++) {
    segs.push_back(Segment(i, 1));
    y_pos.push_back(y(list_[stack[i]]->getOrig()));
    y_orig[i] = y_pos.back();
  }

  while (collide) {
    collide = false;
    float y0 = ymin(bounds_);
    for (Segments::iterator i = segs.begin(); i != segs.end(); i++) {
      float yp = y_pos[i->first];
      if (yp < y0) {
	collide = true;
	if (i == segs.begin()) {
	  y_pos[i->first] = y0;
	  yp = y0;
	}
	else {
	  Segments::iterator j = i;
	  --j;
	  j->second += i->second;
	  // optimize the vertical position such as the centroids are aligned
	  float cy = 0;		// centroid
	  for (int k = j->first; k < (j->first + j->second); k++) {
	    cy += y_orig[k];
	  }
	  cy /= j->second;	// mean of y original positions
	  yp = cy - (j->second*height) / 2;
	  if (yp < ymin(bounds_))
	    yp = ymin(bounds_);
	  y_pos[j->first] = yp;
	  segs.erase(i);
	  i = j;
	}
      }
      y0 = yp + height * i->second;
    }
  }
  for (Segments::iterator j = segs.begin(); j != segs.end(); j++) {
    for (int k = j->first+1; k < (j->first + j->second); k++) {
      y_pos[k] = y_pos[k-1] + height;
    }
  }
  
#else
  float origin = (stack.size() * height) / 2;

  for (int i = 0; i < stack.size(); i++) {
    y_pos.push_back(i * height + origin);
  }
#endif
}

float
LabelLayoutRect::compute_offset(float dy0, float dy1) const
{
  if (dy0 > dy1) {
    std::swap(dy0, dy1);
  }
  if (dy0 < 0) {
    return -dy0;
  }
  if (dy1 > height(bounds_)) {
    return height(bounds_) - dy1;
  }
  return 0;
}

} // namespace infovis

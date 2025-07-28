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
#include <infovis/drawing/lite/Border.hpp>
#include <infovis/drawing/gl_support.hpp>

namespace infovis {

Border::Border(float w)
  : width_(w),
    inside_(color_white),
    outside_(1.0f, 1.0f, 1.0f, 0.0f)
{ }

Border::Border(const Color& inside, float w)
  : width_(w),
    inside_(inside),
    outside_(inside)
{
  outside_[Color::alpha] = 0;
}

Border::Border(const Color& inside, const Color& outside, float w)
  : width_(w),
    inside_(inside),
    outside_(outside)
{ }

Border::~Border()
{ }

float
Border::getWidth() const { return width_; }

void
Border::setWidth(float v) { width_ = v; }

Border::Style
Border::getStyle() const { return style_; }

void
Border::setStyle(Style v) {style_ = v; }

const Color&
Border::getInside() const {return inside_; }

void
Border::setInside(const Color& v) {inside_ = v; }

const Color&
Border::getOutside() const { return outside_; }

void
Border::setOutside(const Color& v) {outside_ = v; }

Box
Border::growBox(const Box& bounds) const
{
  return grow_box(bounds, width_, width_);
}

Box
Border::shrinkBox(const Box& bounds) const
{
  return grow_box(bounds, -width_, -width_);
}

void
Border::render(const Lite::RenderContext& c,
	       const Box& inBounds)
{
  if (c.is_picking) {
      draw_box(growBox(inBounds));
      return;
  }
    
  glPushAttrib(GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  {
    gl::begin_quad_strip q;
    q.color(outside_[0], outside_[1], outside_[2], outside_[3]);
    q.vertex(xmin(inBounds)-width_, ymax(inBounds)+width_);
    q.color(inside_[0], inside_[1], inside_[2], inside_[3]);
    q.vertex(xmin(inBounds), ymax(inBounds));

    q.color(outside_[0], outside_[1], outside_[2], outside_[3]);
    q.vertex(xmax(inBounds)+width_, ymax(inBounds)+width_);
    q.color(inside_[0], inside_[1], inside_[2], inside_[3]);
    q.vertex(xmax(inBounds), ymax(inBounds));

    q.color(outside_[0], outside_[1], outside_[2], outside_[3]);
    q.vertex(xmax(inBounds)+width_, ymin(inBounds)-width_);
    q.color(inside_[0], inside_[1], inside_[2], inside_[3]);
    q.vertex(xmax(inBounds), ymin(inBounds));

    q.color(outside_[0], outside_[1], outside_[2], outside_[3]);
    q.vertex(xmin(inBounds)-width_, ymin(inBounds)-width_);
    q.color(inside_[0], inside_[1], inside_[2], inside_[3]);
    q.vertex(xmin(inBounds), ymin(inBounds));
  
    q.color(outside_[0], outside_[1], outside_[2], outside_[3]);
    q.vertex(xmin(inBounds)-width_, ymax(inBounds)+width_);
    q.color(inside_[0], inside_[1], inside_[2], inside_[3]);
    q.vertex(xmin(inBounds), ymax(inBounds));
  }
  glPopAttrib();
}

} // namespace infovis

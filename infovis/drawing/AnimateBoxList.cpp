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
#include <infovis/drawing/AnimateBoxList.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <algorithm>

namespace infovis {

AnimateBoxList::AnimateBoxList(int size)
  : start_(size), end_(size), tex_coords_(size), texture_(0)
{ }


void
AnimateBoxList::setStartList(const BoxList& bl)
{
  start_ = bl;
}

void
AnimateBoxList::setEndList(const BoxList& el)
{
  end_ = el;
}

void
AnimateBoxList::setTexCoords(const BoxList& tc)
{
  tex_coords_ = tc;
}

void
AnimateBoxList::setTexture(unsigned int tex)
{
  texture_ = tex;
}

static float
interp(float from, float to, float param)
{
  return (1-param) * from + param * to;
}

static Box
interp_box(const Box& from, const Box& to, float param)
{
  return Box(interp(xmin(from), xmin(to), param),
	     interp(ymin(from), ymin(to), param),
	     interp(xmax(from), xmax(to), param),
	     interp(ymax(from), ymax(to), param));
}

static bool
is_null_box(const Box& b)
{
  return
    xmin(b) == 0 &&
    xmax(b) == 0 &&
    ymin(b) == 0 &&
    ymax(b) == 0;
}

void
AnimateBoxList::render(float param)
{
  // TODO: Fixed type compatibility for C++17 - use size_type consistently 
  std::size_t last = std::min(start_.size(), end_.size());
  last = std::min(last, tex_coords_.size());
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
  std::vector<Point> boxes(last*4);
  std::vector<Point> tex_coords(last*4);
  //glBegin(GL_QUADS);
  int j = 0;
  for (std::size_t i = 0; i < last; i++) {
    const Box& from = start_[i];
    const Box& to = end_[i];
    const Box& tex = tex_coords_[i];
    if (is_null_box(tex))
      continue;

    Box interp = interp_box(from, to, param);

    tex_coords[j] = Point(xmin(tex), ymin(tex));
    boxes[j] = Point(xmin(interp), ymin(interp));
    j++;
    tex_coords[j] = Point(xmax(tex), ymin(tex));
    boxes[j] = Point(xmax(interp), ymin(interp));
    j++;
    tex_coords[j] = Point(xmax(tex), ymax(tex));
    boxes[j] = Point(xmax(interp), ymax(interp));
    j++;
    tex_coords[j] = Point(xmin(tex), ymax(tex));
    boxes[j] = Point(xmin(interp), ymax(interp));
    j++;
  }
  //glEnd();
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, &(*boxes.begin()));
  glTexCoordPointer(2, GL_FLOAT, 0, &(*tex_coords.begin()));
  glDrawArrays(GL_QUADS, 0, j);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glPopAttrib();
  glBindTexture(GL_TEXTURE_2D, 0);
}

void
AnimateBoxList::computeTexCoords(const Box& tex_bounds)
{
  tex_coords_.resize(start_.size());
  for (int i = 0; i < start_.size(); i++) {
    Box& from = start_[i];
    Box& to = end_[i];
    Box& b = tex_coords_[i];
    if (is_null_box(from)) {
      if (is_null_box(to))
	continue;
      Point p = center(to);
      from = Box(p,p);
    }
    else if (is_null_box(to)) {
      Point p = center(from);
      to = Box(p,p);
    }

    set_xmin(b, (xmin(from)-xmin(tex_bounds))/width(tex_bounds));
    set_ymin(b, (ymin(from)-ymin(tex_bounds))/height(tex_bounds));
    set_xmax(b, (xmax(from)-xmin(tex_bounds))/width(tex_bounds));
    set_ymax(b, (ymax(from)-ymin(tex_bounds))/height(tex_bounds));
  }
}

void
AnimateBoxList::swap()
{
  start_.swap(end_);
}

} /// namespace infovis

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
#include <AnimateTree.hpp>
#include <FastDrawer.hpp>

namespace infovis {

AnimateTree::AnimateTree(int size, Tree& tree, FastDrawer& drawer)
  : AnimateBoxList(size),
    tree_(tree),
    drawer_(drawer)
{ }

static inline float
interp(float from, float to, float param)
{
  return (1-param) * from + param * to;
}

static inline Box
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
AnimateTree::render(float param)
{
  if (texture_ != 0) {
    AnimateBoxList::render(param);
    return;
  }
  unsigned last = std::min(start_.size(), end_.size());
  //last = std::min(last, tex_coords_.size());
  glPushAttrib(GL_COLOR_BUFFER_BIT
	       | GL_FOG_BIT 
	       | GL_STENCIL_BUFFER_BIT
	       );

  //glShadeModel(GL_SMOOTH);
  glPushMatrix();
  glScalef(1, 1, -1);
  drawer_.start();
  //glEnable(GL_STENCIL_TEST);
  //glStencilOp(GL_KEEP, GL_INCR, GL_INCR);

  glFogi(GL_FOG_START, 0);
  glFogi(GL_FOG_END, 3);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

  for (int i = 0; i < last; i++) {
    const Box& from = start_[i];
    const Box& to = end_[i];
    Box interp;
    if (is_null_box(from)) {
      if (is_null_box(to))
	continue;
      Point p(center(to));
      interp = interp_box(Box(p, p), to, param);
    }
    else if (is_null_box(to)) {
      Point p(center(from));
      interp = interp_box(from, Box(p, p), param);
    }
    else {
      interp = interp_box(from, to, param);
    }
    drawer_.draw_box(interp, i, 1);
  }
  drawer_.finish();
  glPopMatrix();
  glPopAttrib();
}

void
AnimateTree::clear()
{
  std::fill(start_.begin(), start_.end(), Box());
  std::fill(end_.begin(), end_.end(), Box());
  std::fill(tex_coords_.begin(), tex_coords_.end(), Box());
  texture_ = 0;
}


} /// namespace infovis

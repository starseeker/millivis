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
#include <infovis/drawing/lite/LiteSaveUnder.hpp>
#include <infovis/drawing/gl.hpp>
#include <GL/glut.h>

namespace infovis {

LiteSaveUnder::~LiteSaveUnder()
{
  delete save_under;
  save_under = 0;
}

void
LiteSaveUnder::doRender(const RenderContext& c)
{
  box_min_extent<int> ibounds(getBounds()); // warns but OK
  save(ibounds);
  LiteProxy::render(c);
}

void
LiteSaveUnder::repaint()
{
  if (save_under == 0)
    return;
  box_min_extent<int> ibounds(getBounds()); // warns but OK
  restore();
  if (saved != ibounds) {
    save(ibounds);
  }
  LiteProxy::render(RenderContext());
}

void
LiteSaveUnder::save(const IntBox& b)
{
  saved = b;
  int size = width(saved) * height(saved) * 4;
  if (size > save_under_size) {
    save_under_size = size;
    delete save_under;
    save_under = new unsigned char [save_under_size];
  }
  glReadPixels(xmin(saved), ymin(saved),
	       width(saved), height(saved),
	       GL_RGB, GL_UNSIGNED_BYTE, save_under);
}

void
LiteSaveUnder::restore()
{
  glRasterPos2d(xmin(saved), ymin(saved));
  glDrawPixels(width(saved), height(saved),
	       GL_RGB, GL_UNSIGNED_BYTE, save_under);
}

void
LiteSaveUnder::refresh()
{
  GLint drawBuffer;
  glGetIntegerv(GL_DRAW_BUFFER, &drawBuffer);
  if (drawBuffer == GL_BACK ||
      drawBuffer == GL_BACK_LEFT ||
      drawBuffer == GL_BACK_RIGHT) {
    glDrawBuffer(GL_FRONT);
    glutSetCursor(GLUT_CURSOR_NONE);
    glRasterPos2d(xmin(saved), ymin(saved));
    glCopyPixels(xmin(saved), ymin(saved),
		 width(saved), height(saved),
		 GL_COLOR);
    glDrawBuffer(GL_BACK);
    glutSetCursor(GLUT_CURSOR_INHERIT);
  }
  glFlush();
}
} // namespace infovis

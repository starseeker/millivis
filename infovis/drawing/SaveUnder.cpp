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
#include <infovis/drawing/SaveUnder.hpp>
#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <iostream>

#undef DBG
#ifndef NDEBUG
#define DBG {						\
  int err = glGetError();				\
  if (err)						\
    std::cerr << __FILE__ << "(" << __LINE__ << ") : "	\
	      << "gl error: " << gl::glErrorString(err)	\
	      << std::endl;				\
}
#else
#define DBG
#endif

namespace infovis {

SaveUnder::SaveUnder(bool t)
  : use_texture(t),
    data(0),
    data_size(0),
    max_width(0),
    max_height(0),
    x(0), y(0), width(0), height(0)
{
}

SaveUnder::SaveUnder(unsigned int mw, unsigned int mh, bool t)
  : use_texture(t),
    data(0),
    data_size(0),
    max_width(mw),
    max_height(mh),
    x(0), y(0), width(0), height(0)
{
}

SaveUnder::~SaveUnder()
{
  free_ressources();
}

unsigned int
SaveUnder::next_power_of_2(unsigned int n)
{
  unsigned int p;
  for (p = 0; (1 << p) < n && p < 32; p++)
    ;
  return 1 << p;
}

#define select_texture(tex)  { glBindTexture(GL_TEXTURE_2D, tex); DBG; }

static void
create_texture(unsigned int bind_to, unsigned int width, unsigned int height)
{
  select_texture(bind_to);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
	       GL_RGBA, GL_UNSIGNED_BYTE, 0);
  DBG;
}

void
SaveUnder::allocate_ressources()
{
  if (data_size != 0) return;
  if (max_width == 0 || max_height == 0) {
    max_width = width;
    max_height = height;
  }
  if (max_width == 0 || max_height == 0) {
    return;
  }
  if (use_texture) {
    // We may be smarter by trying to tile anyway, i.e. if width is 700,
    // use 3 tiles of 256 instead of 1 tile of 1024.  We won't care for now.

    max_width = next_power_of_2(max_width);
    max_height = next_power_of_2(max_height);
    // First, try to allocate the whole texture 
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA8,
		 max_width, max_height, 0,
		 GL_RGBA, GL_UNSIGNED_BYTE, 0);

    GLint allocated_width;
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0,
			     GL_TEXTURE_WIDTH, &allocated_width);
    
    if (allocated_width == max_width) {
      tile_width = max_width;
      tile_height = max_height;
      data_size = 1;
      data = new unsigned int[1];
      glGenTextures(1, data);
      create_texture(data[0], max_width, max_height);
      return;
    }
    // cannot allocate in one chunk
    int max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    // ht is the number of horizontal tiles we need to allocate
    // vt is the number of vertical tiles we need to allocate
    int ht = max_width / max_tex_size;
    if (ht == 0) ht = 1;
    int vt = max_height / max_tex_size;
    if (vt == 0) vt = max_height;
    data_size = ht * vt;
    data = new unsigned int[data_size];
    glGenTextures(data_size, data);
    tile_width = tile_height = max_tex_size;
    for (int y = 0; y < max_height; y += tile_height) {
      for (int x = 0; x < max_width; x += tile_width) {
	create_texture(data[ht * (y/tile_height) + x/tile_width],
		       std::min(max_width, tile_width),
		       std::min(max_height, tile_height));
      }
    }
  }
  else {
    data_size = max_width * max_height;
#ifdef USE_GC
    //GC_expand_hp(2*data_size);	// try
    data =
      static_cast<unsigned int*>(GC_malloc_atomic_ignore_off_page(data_size *
								  sizeof(unsigned int)));
#else
    data = new unsigned int[data_size];
#endif
  }
}

void
SaveUnder::free_ressources()
{
  if (use_texture) {
    if (data_size != 0) {
      glDeleteTextures(data_size, data);
      DBG;
    }
  }
  else {
#ifdef USE_GC
    GC_free(data);
#else
    delete[] data;
#endif
  }
  data = 0;
  data_size = 0;
}

void
SaveUnder::save(int X, int Y, unsigned int w, unsigned int h)
{
  allocate_ressources();
  DBG;
  glReadBuffer(GL_BACK);
  DBG;
#if 0
  std::cerr << "save("
	    << X << ", "
	    << Y << ", "
	    << w << ", "
	    << h << ")\n";
#endif
  x = X;
  y = Y;
  width = std::min(w, max_width);
  height = std::min(h, max_height);
  if (use_texture) {

    int ht = max_width / tile_width;
    if (ht == 0) ht = 1;
    for (int y = 0; y < height; y += tile_height) {
      int maxy = std::min(y + tile_height, height);
      for (int x = 0; x < width; x += tile_width) {
	int maxx = std::min(x + tile_width, width);
	select_texture(data[ht * (y/tile_height) + x/tile_width]);
	{
	  int err = glGetError();
	  if (err)						
	    std::cerr << __FILE__ << "(" << __LINE__ << ") : "	
		      << "gl error: " << gl::glErrorString(err)
		      << " for texture "
		      << "data[" << ht * (y/tile_height) + x/tile_width << "]"
		      << "="
		      << data[ht * (y/tile_height) + x/tile_width]
		      << " ht=" << ht
		      << " y=" << y
		      << " x=" << x
		      << " tile_width=" << tile_width
		      << std::endl;
	}
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0,
			    0, 0,
			    x+X, y+Y,
			    maxx - x, maxy - y);
	DBG;
      }
    }
    select_texture(0);
  }
  else {
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
}

void
SaveUnder::restore(int X, int Y)
{
  if (data_size == 0) return;
  if (use_texture) {
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    DBG;

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    int ht = max_width / tile_width;
    if (ht == 0) ht = 1;
    for (int y = 0; y < height; y += tile_height) {
      int maxy = std::min(y + tile_height, height);
      for (int x = 0; x < width; x += tile_width) {
	int maxx = std::min(x + tile_width, width);
	select_texture(data[ht * (y/tile_height) + x/tile_width]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	gl::vertex(X+x, Y+y);
	glTexCoord2f(0.0f, float(maxy - y)/tile_height);
	gl::vertex(X+x, Y+maxy);
	glTexCoord2d(float(maxx - x)/tile_width, float(maxy - y)/tile_height);
	gl::vertex(X+maxx, Y+maxy);
	glTexCoord2d(float(maxx - x)/tile_width, 0.0f);
	gl::vertex(X+maxx, Y+y);
	glEnd();
      }
    }
    select_texture(0);
    DBG;
    glPopAttrib();
  }
  else {
    glRasterPos2i(X, Y);
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    DBG;
  }
}

void
SaveUnder::restore()
{
  restore(x, y);
}

void
SaveUnder::resize(unsigned int w, unsigned int h)
{
}

} // namespace infovis

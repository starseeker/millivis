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
#include <infovis/drawing/SaveUnder.hpp> // for next_power_of_2
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <FastDrawer.hpp>
#include <ColorRamp.hpp>
#include <iostream>
#if 0
#define GLH_EXT_SINGLE_FILE
#include <glh_nveb.h>
#include <glh_extensions.h>
#include <glh_obs.h>
#include <glh_glut.h>
#endif

#include <gl/glext.h>

namespace infovis {

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

static int wait_count;
static int flush_count;
static unsigned long vertex_count;
static int start_time;

FastDrawer::FastDrawer(const Tree& t,
		       const FloatColumn * color_prop,
		       unsigned size)
  : BorderDrawer(t),
    size_(size),
    color_prop_(color_prop),
    data_(0),

    current_(0),
    mode_(gl::bm_quads),
    max_depth_(unsigned(-1)),
    filter_(FilterColumn::cast(t.find_column("$filter"))),
    color_texture_(0),
    color_smooth_(false),
    color_ramp_(),
    color_range_(0),
    color_min_(0),
    color_scale_(0),
    color_delta_(0),
    dryrun_(false),
    total_size_(0)
{
#if 0
  if (false &&
      glh_init_extensions("GL_ARB_multitexture "
			  "GL_NV_fence "
			  "GL_NV_vertex_array_range "
			  )) {
    if (glh_init_extension("GL_ARB_multitexture") &&
	glh_init_extension("GL_NV_vertex_array_range") &&
	glh_init_extension("GL_NV_fence")) {
      float * memory = 0;	// float * for easier calculation
      int cur_size = size_;
      while (cur_size != 0) {
	int byte_size = cur_size * VERTEX_INFO * sizeof(float);
	memory = static_cast<float*>(wglAllocateMemoryNV(byte_size, 0, 0, 0.5f));
	if (memory == 0) {
	  cur_size /= 2;
	  if (cur_size < 10000)
	    cur_size = 0;	// stop if we cannot get a reasonable size
	}
	else {
#ifdef USE_GC
	  GC_exclude_static_roots(memory, memory + VERTEX_INFO*cur_size);
#endif
	  total_size_ = cur_size;
	  size_ = cur_size / 4;
	  glVertexArrayRangeNV(byte_size, memory);
	  DBG;
	  buffer[0].pointer = memory;
	  buffer[1].pointer = memory + size_*VERTEX_INFO;
	  buffer[2].pointer = memory + 2*size_*VERTEX_INFO;
	  buffer[3].pointer = memory + 3*size_*VERTEX_INFO;
	  glGenFencesNV(1, & buffer[0].fence);
	  glGenFencesNV(1, & buffer[1].fence);
	  glGenFencesNV(1, & buffer[2].fence);
	  glGenFencesNV(1, & buffer[3].fence);
	  current_buffer_ = -1;
	  return;
	}
      }
      // cur_size == 0 so we could't allocate,
    }
  }
#endif
  // continue with std allocation
  data_ = new float[size_ * VERTEX_INFO];
}

FastDrawer::~FastDrawer()
{
#if 0
  if (total_size_ != 0) {
    wglFreeMemoryNV(buffer[0].pointer);
    glDeleteFencesNV(1, &buffer[0].fence);
    glDeleteFencesNV(1, &buffer[1].fence);
    glDeleteFencesNV(1, &buffer[2].fence);
    glDeleteFencesNV(1, &buffer[3].fence);
  }
  else
#endif
    delete []data_;
#ifndef NO_TEXTURE
  glDeleteTextures(1, &color_texture_);

  DBG;
  color_texture_ = 0;
#endif
}

void
FastDrawer::flush()
{
  //glEnableClientState(GL_COLOR_ARRAY);
  flush_count++;
  if (current_ != 0) {
    vertex_count += current_ / VERTEX_INFO;
    DBG;
    if (! dryrun_) {
      glDrawArrays(mode_, 0, current_/VERTEX_INFO);
#if 0
      if (total_size_ != 0)
	glSetFenceNV(buffer[current_buffer_].fence, GL_ALL_COMPLETED_NV);
#endif
    }
    DBG;
    current_ = 0;
  }
  next_buffer();
}

void
FastDrawer::set_mode(gl::begin_mode mode)
{
  mode_ = mode;
}

void
FastDrawer::next_buffer()
{
  if (total_size_ == 0)
    return;			// NVidia extension not active
  bool test_fence = true;
  if (current_buffer_ == -1)
    test_fence = false;
  current_buffer_++;
  if (current_buffer_ > 3)
    current_buffer_ = 0;
  data_ = buffer[current_buffer_].pointer;
#if 0
  if (test_fence) {
    if (! dryrun_) {
      if (!glTestFenceNV(buffer[current_buffer_].fence)) {
	wait_count++;
	glFinishFenceNV(buffer[current_buffer_].fence);
      }
    }
    DBG;
  }
#endif
  glVertexPointer(3, GL_FLOAT, VERTEX_INFO * sizeof(float),
		  buffer[current_buffer_].pointer);
#ifndef NO_TEXTURE
  glTexCoordPointer(1, GL_FLOAT, VERTEX_INFO * sizeof(float),
		    buffer[current_buffer_].pointer + 3);
#else
  glColorPointer(4, GL_UNSIGNED_BYTE, VERTEX_INFO * sizeof(float),
		 buffer[current_buffer_].pointer + 3);
#endif
  DBG;
}

void
FastDrawer::wait_fence()
{
  if (total_size_ == 0)
    return;			// NVidia extension not active
  current_buffer_--;
  if (current_buffer_ < 0)
    current_buffer_ = 3;
#if 0
  // wait for the processing of the last buffer sent
    if (!glTestFenceNV(buffer[current_buffer_].fence)) {
      wait_count++;
      glFinishFenceNV(buffer[current_buffer_].fence);
    }
#endif
}

void
FastDrawer::allocate_ressources()
{
  if (color_texture_ == 0) {
#ifndef NO_TEXTURE
    glGenTextures(1, &color_texture_);
#endif
    if (color_ramp_.empty()) {
      // beware of the order, set_color_ramp calls allocate_ressources
      set_color_ramp(getRamp(ramp_categorical1));
      set_color_smooth(false);
    }
  }
#ifndef NO_TEXTURE
#if VERTEX_INFO==5
  glBindTexture(GL_TEXTURE_2D, color_texture_);
#else
  glBindTexture(GL_TEXTURE_1D, color_texture_);
#endif
  DBG;
#endif
}

void
FastDrawer::set_color_ramp(const std::vector<Color>& colors)
{
  color_ramp_ = colors;

#ifndef NO_TEXTURE
  int i;
  unsigned s = SaveUnder::next_power_of_2(colors.size());
  string version = LiteWindow::getVersion();

  //std::cerr << "OpenGL Version is " << version << std::endl;
  if (version[0] > '1' ||
      (version[0] == '1' && version[1] == '.' && version[2] >= '2')) {
    Color * ramp = new Color[s];
    for (i = 0; i < colors.size(); i++) {
      ramp[i] = colors[i];
    }
    while (i < (s)) {
      ramp[i++] = colors[colors.size()-1]; // clamp
    }
    allocate_ressources();
    set_color_smooth(color_smooth_);

#if VERTEX_INFO==5
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		 s, 1, 0,
		 GL_RGBA, type_of(color_ramp_[0][Color::red]),
		 ramp);
#else
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA,
		 s, 0,
		 GL_RGBA, type_of(color_ramp_[0][Color::red]),
		 ramp);
#endif
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    DBG;
    delete []ramp;
  }
  else {
#if VERTEX_INFO==5
    Color * ramp = new Color[3*(s+2)];
#else
    Color * ramp = new Color[(s+2)];
#endif
    ramp[0] = colors[0];  
    for (i = 1; i <= colors.size(); i++) {
      ramp[i] = colors[i-1];
    }
    while (i < (s+2)) {
      ramp[i++] = colors[colors.size()-1]; // clamp
    }

#if VERTEX_INFO==5
    memcpy(ramp+s+2, ramp, s+2);
    memcpy(ramp+2*(s+2), ramp, s+2);
#endif
    allocate_ressources();
    set_color_smooth(color_smooth_);
#if VERTEX_INFO==5
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		 s+2, 1, 1,
		 GL_RGBA, type_of(color_ramp_[0][Color::red]),
		 ramp);
#else
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA,
		 s+2, 1,
		 GL_RGBA, type_of(color_ramp_[0][Color::red]),
		 ramp);
#endif
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    DBG;
    delete []ramp;
  }
#endif // NO_TEXTURE
  set_color_range(color_min_, color_range_);
}

const std::vector<Color>&
FastDrawer::get_color_ramp() const
{
  return color_ramp_;
}

void
FastDrawer::set_color_prop(const FloatColumn * prop)
{
  color_prop_ = prop;
}

void
FastDrawer::set_color_smooth(bool smooth)
{
  color_smooth_ = smooth;
  allocate_ressources();
  //std::cerr << "Color smooth: " << smooth << std::endl;
#ifndef NO_TEXTURE
#if VERTEX_INFO==5
  if (color_smooth_) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }  
#else
  // do it always linear
  //if (color_smooth_) {
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   }
//   else {
//     glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   }  
#endif
#endif // NO_TEXTURE
}

bool
FastDrawer::get_color_smooth() const
{
  return color_smooth_;
}

void
FastDrawer::set_color_range(float min_value, float range)
{
  color_min_ = min_value;
  color_range_ = range;

#ifdef NO_TEXTURE
  if (color_range_ == 0) {
    color_range_ = color_ramp_.size()-1;
    color_scale_ = 1.0f;
  }
  else
    color_scale_ = (color_ramp_.size()-1) / color_range_;
#else
  unsigned s = SaveUnder::next_power_of_2(color_ramp_.size());

  if (color_range_ == 0) {
    color_range_ = color_ramp_.size()-1;
  }
  color_scale_ = (color_ramp_.size()-1) / ((s-1) * color_range_) ;
#endif
  //color_delta_ = color_scale_ * 0.2f;
  color_delta_ = 0;

#if 0
  std::cerr << "Color range: " << range 
	    << " scale: " << color_scale_
	    << " min_value: " << color_min_
	    << std::endl;
#endif
  set_color_smooth(color_scale_ != 1.0f);
}

void
FastDrawer::get_color_range(float& min_value, float& range)
{
  min_value = color_min_;
  range = color_range_;
}

void
FastDrawer::start(gl::begin_mode mode)
{
  mode_ = mode;
#ifdef NO_TEXTURE
  glEnableClientState(GL_COLOR_ARRAY);
#else
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
  glEnableClientState(GL_VERTEX_ARRAY);
  if (total_size_ != 0)
    glEnableClientState(GL_VERTEX_ARRAY_RANGE_NV);
  DBG;
  current_ = 0;
  current_buffer_ = -1;
#ifdef NO_TEXTURE
  glPushAttrib(GL_COLOR_BUFFER_BIT);
#else
  glPushAttrib(GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
#if VERTEX_INFO==5
  glEnable(GL_TEXTURE_2D);
#else
  glEnable(GL_TEXTURE_1D);
#endif
#endif
  glDisable(GL_BLEND);
  DBG;
  allocate_ressources();
  if (total_size_ != 0)
    next_buffer();
  else {
    glVertexPointer(3, type_of(data_[0]), VERTEX_INFO * sizeof(float),
		    data_);
    DBG;
#ifndef NO_TEXTURE
    glTexCoordPointer(1, type_of(data_[0]), VERTEX_INFO * sizeof(float),
		      data_+3);
#else
    glColorPointer(4, GL_UNSIGNED_BYTE, VERTEX_INFO * sizeof(float),
		   data_ + 3);
#endif
    DBG;
  }
#ifndef NO_TEXTURE_TRANSFORM
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glLoadIdentity();
  glScalef(color_scale_, 1, 1);
  glTranslatef(-color_min_, 0, 0);
  glMatrixMode(GL_MODELVIEW);
#endif
  DBG;
  start_time = LiteWindow::time();
}

void
FastDrawer::finish()
{
  flush();
  wait_fence();
#ifndef NO_TEXTURE_TRANSFORM
  glMatrixMode(GL_TEXTURE);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
#endif
  glPopAttrib();
#ifdef NO_TEXTURE
  glDisableClientState(GL_COLOR_ARRAY);
#else
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
  glDisableClientState(GL_VERTEX_ARRAY);
  if (total_size_ != 0)
    glDisableClientState(GL_VERTEX_ARRAY_RANGE_NV);
  DBG;
  if (wait_count != 0) {
    std::cout << "Waited for fence " << wait_count
	      << " times over " << flush_count << std::endl;
    wait_count = 0;
  }
  flush_count = 0;
  int time = LiteWindow::time() - start_time;
#ifdef PRINT
  if (time > 500) {
    std::cout << "Raw speed: "
	      << vertex_count << " vertices in "
	      << time / 1000.0f << "second, "
	      << vertex_count * 1000.0f / time << " vertex per second\n";
  }
#endif
  vertex_count = 0;
}



} // namespace infovis

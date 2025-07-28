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
#ifndef __glcc__
#define __glcc__
// Copyright (c) 2001 by Stephane Conversy, Ecole des Mines de Nantes.
// Modified by Jean-Daniel Fekete (fekete@emn.fr)

#if defined(_WIN32)
#  include <windows.h>
#undef min
#undef max
#endif
#include <GL/gl.h>
#include <stdexcept>

/**
 * C++ binding for type safer OpenGL
 */
namespace gl {
typedef GLbyte byte_t;
typedef GLubyte ubyte_t;
typedef GLshort short_t;
typedef GLushort ushort_t;
typedef GLdouble double_t;
typedef GLfloat float_t;
typedef GLint int_t;
typedef GLuint uint_t;
typedef GLsizei sizei_t;

enum PixelType {
  pt_bitmap                         = GL_BITMAP,
  pt_byte                           = GL_BYTE,
  pt_unsigned_byte                  = GL_UNSIGNED_BYTE,
  pt_short                          = GL_SHORT,
  pt_unsigned_short                 = GL_UNSIGNED_SHORT,
  pt_int                            = GL_INT,
  pt_unsigned_int                   = GL_UNSIGNED_INT,
  pt_float                          = GL_FLOAT,
  pt_2_bytes                        = GL_2_BYTES,
  pt_3_bytes                        = GL_3_BYTES,
  pt_4_bytes                        = GL_4_BYTES,
  pt_double                         = GL_DOUBLE
};

enum PixelCopyType {
  pct_color                         = GL_COLOR,
  pct_depth                         = GL_DEPTH,
  ptc_stencil                       = GL_STENCIL
};

enum PixelFormat {
  pf_color_index                    = GL_COLOR_INDEX,
  pf_stencil_index                  = GL_STENCIL_INDEX,
  pf_depth_component                = GL_DEPTH_COMPONENT,
  pf_red                            = GL_RED,
  pf_green                          = GL_GREEN,
  pf_blue                           = GL_BLUE,
  pf_alpha                          = GL_ALPHA,
  pf_rgb                            = GL_RGB,
  pf_rgba                           = GL_RGBA,
  pf_luminance                      = GL_LUMINANCE,
  pf_luminance_alpha                = GL_LUMINANCE_ALPHA
};

static inline const char * glErrorString(int code)
{
  switch(code) {
  case GL_NO_ERROR: return "GL_NO_ERROR";
  case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
  case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
  case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
  case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
  }
  return "GL_UNDOCUMENTED_ERROR";
}
      

/**
 * Exception class for throwing eOpenGL errors as excepctions
 */
struct gl_error : public std::runtime_error {
  gl_error(int err, const char * file, int line=-1)
    : runtime_error(glErrorString(err)), file(file), line(line) {}
  gl_error(const gl_error& other)
    : runtime_error(other.what()), file(other.file), line(other.line) {}
  const char* file;
  int line;
};
	
	
inline void check_error(const char* file=0, int line=-1) {
  int err=glGetError();
  if(err)
    throw gl_error(err, file, line);
}
	
#ifndef NDEBUG
#define DBG check_error(__FILE__, __LINE__);
#else
#define DBG
#endif
	
enum begin_mode {
  bm_points = GL_POINTS,
  bm_lines = GL_LINES,
  bm_line_strip = GL_LINE_STRIP,
  bm_line_loop = GL_LINE_LOOP,
  bm_triangles = GL_TRIANGLES,
  bm_triangle_strip = GL_TRIANGLE_STRIP,
  bm_triangle_fan = GL_TRIANGLE_FAN,
  bm_quads = GL_QUADS,
  bm_quad_strip = GL_QUAD_STRIP,
  bm_polygon = GL_POLYGON  // shared value
};

inline void begin(begin_mode m) { glBegin(m); /*DBG*/ } // glGetError is forbidden inside a begin, but it works as long as there wasn't any glVertex call !!!
inline void end() { glEnd(); DBG}

inline void vertex  (double_t x, double_t y)		    { glVertex2d (x, y); }
inline void vertex2 (double_t x, double_t y)		    { glVertex2d (x, y); }
inline void vertex2 (const double_t *v)		    { glVertex2dv (v); }
inline void vertex  (float_t x, float_t y)		    { glVertex2f (x, y); }
inline void vertex2 (float_t x, float_t y)		    { glVertex2f (x, y); }
inline void vertex2 (const float_t *v)		    { glVertex2fv (v); }
inline void vertex  (int_t x, int_t y)		    { glVertex2i (x, y); }
inline void vertex2 (int_t x, int_t y)		    { glVertex2i (x, y); }
inline void vertex2 (const int_t *v)			    { glVertex2iv (v); }
inline void vertex  (short_t x, short_t y)		    { glVertex2s (x, y); }
inline void vertex2 (short_t x, short_t y)		    { glVertex2s (x, y); }
inline void vertex2 (const short_t *v)		    { glVertex2sv (v); }
inline void vertex  (double_t x, double_t y, double_t z)  { glVertex3d (x, y, z); }
inline void vertex3 (double_t x, double_t y, double_t z)  { glVertex3d (x, y, z); }
inline void vertex3 (const double_t *v)		    { glVertex3dv (v); }
inline void vertex  (float_t x, float_t y, float_t z)	    { glVertex3f (x, y, z); }
inline void vertex3 (float_t x, float_t y, float_t z)	    { glVertex3f (x, y, z); }
inline void vertex3 (const float_t *v)		    { glVertex3fv (v); }
inline void vertex  (int_t x, int_t y, int_t z)	    { glVertex3i (x, y, z); }
inline void vertex3 (int_t x, int_t y, int_t z)	    { glVertex3i (x, y, z); }
inline void vertex3 (const int_t *v)			    { glVertex3iv (v); }
inline void vertex  (short_t x, short_t y, short_t z)	    { glVertex3s (x, y, z); }
inline void vertex3 (short_t x, short_t y, short_t z)	    { glVertex3s (x, y, z); }
inline void vertex3 (const short_t *v)		    { glVertex3sv (v); }
inline void vertex  (double_t x, double_t y, double_t z, double_t w) { glVertex4d (x, y, z, w); }
inline void vertex4 (double_t x, double_t y, double_t z, double_t w) { glVertex4d (x, y, z, w); }
inline void vertex4 (const double_t *v)		    { glVertex4dv (v); }
inline void vertex  (float_t x, float_t y, float_t z, float_t w) { glVertex4f (x, y, z, w); }
inline void vertex4 (float_t x, float_t y, float_t z, float_t w) { glVertex4f (x, y, z, w); }
inline void vertex4 (const float_t *v)		    { glVertex4fv (v); }
inline void vertex  (int_t x, int_t y, int_t z, int_t w)  { glVertex4i (x, y, z, w); }
inline void vertex4 (int_t x, int_t y, int_t z, int_t w)  { glVertex4i (x, y, z, w); }
inline void vertex4 (const int_t *v)			    { glVertex4iv (v); }
inline void vertex  (short_t x, short_t y, short_t z, short_t w) { glVertex4s (x, y, z, w); }
inline void vertex4 (short_t x, short_t y, short_t z, short_t w) { glVertex4s (x, y, z, w); }
inline void vertex4 (const short_t *v)		    { glVertex4sv (v); }

inline void color  (byte_t red, byte_t green, byte_t blue) { glColor3b (red, green, blue); }
inline void color3 (byte_t red, byte_t green, byte_t blue) { glColor3b (red, green, blue); }
inline void color3 (const byte_t *v)			    { glColor3bv (v); }
inline void color  (double_t red, double_t green, double_t blue) { glColor3d (red, green, blue); }
inline void color3 (double_t red, double_t green, double_t blue) { glColor3d (red, green, blue); }
inline void color3 (const double_t *v)		    { glColor3dv (v); }
inline void color  (float_t red, float_t green, float_t blue) { glColor3f (red, green, blue); }
inline void color3 (float_t red, float_t green, float_t blue) { glColor3f (red, green, blue); }
inline void color3 (const float_t *v)			    { glColor3fv (v); }
inline void color  (int_t red, int_t green, int_t blue)   { glColor3i (red, green, blue); }
inline void color3 (int_t red, int_t green, int_t blue)   { glColor3i (red, green, blue); }
inline void color3 (const int_t *v)			    { glColor3iv (v); }
inline void color  (short_t red, short_t green, short_t blue) { glColor3s (red, green, blue); }
inline void color3 (short_t red, short_t green, short_t blue) { glColor3s (red, green, blue); }
inline void color3 (const short_t *v)			    { glColor3sv (v); }
inline void color  (ubyte_t red, ubyte_t green, ubyte_t blue) { glColor3ub (red, green, blue); }
inline void color3 (ubyte_t red, ubyte_t green, ubyte_t blue) { glColor3ub (red, green, blue); }
inline void color3 (const ubyte_t *v)			    { glColor3ubv (v); }
inline void color  (uint_t red, uint_t green, uint_t blue) { glColor3ui (red, green, blue); }
inline void color3 (uint_t red, uint_t green, uint_t blue) { glColor3ui (red, green, blue); }
inline void color3 (const uint_t *v)			    { glColor3uiv (v); }
inline void color  (ushort_t red, ushort_t green, ushort_t blue) { glColor3us (red, green, blue); }
inline void color3 (ushort_t red, ushort_t green, ushort_t blue) { glColor3us (red, green, blue); }
inline void color3 (const ushort_t *v)		    { glColor3usv (v); }
inline void color  (byte_t red, byte_t green, byte_t blue, byte_t alpha) { glColor4b (red, green, blue, alpha); }
inline void color4 (byte_t red, byte_t green, byte_t blue, byte_t alpha) { glColor4b (red, green, blue, alpha); }
inline void color4 (const byte_t *v)			    { glColor4bv (v); }
inline void color  (double_t red, double_t green, double_t blue, double_t alpha) { glColor4d (red, green, blue, alpha); }
inline void color4 (double_t red, double_t green, double_t blue, double_t alpha) { glColor4d (red, green, blue, alpha); }
inline void color4 (const double_t *v)		    { glColor4dv (v); }
inline void color  (float_t red, float_t green, float_t blue, float_t alpha) { glColor4f (red, green, blue, alpha); }
inline void color4 (float_t red, float_t green, float_t blue, float_t alpha) { glColor4f (red, green, blue, alpha); }
inline void color4 (const float_t *v)			    { glColor4fv (v); }
inline void color  (int_t red, int_t green, int_t blue, int_t alpha) { glColor4i (red, green, blue, alpha); }
inline void color4 (int_t red, int_t green, int_t blue, int_t alpha) { glColor4i (red, green, blue, alpha); }
inline void color4 (const int_t *v)			    { glColor4iv (v); }
inline void color  (short_t red, short_t green, short_t blue, short_t alpha) { glColor4s (red, green, blue, alpha); }
inline void color4 (short_t red, short_t green, short_t blue, short_t alpha) { glColor4s (red, green, blue, alpha); }
inline void color4 (const short_t *v)			    { glColor4sv (v); }
inline void color  (ubyte_t red, ubyte_t green, ubyte_t blue, ubyte_t alpha) { glColor4ub (red, green, blue, alpha); }
inline void color4 (ubyte_t red, ubyte_t green, ubyte_t blue, ubyte_t alpha) { glColor4ub (red, green, blue, alpha); }
inline void color4 (const ubyte_t *v)			    { glColor4ubv (v); }
inline void color  (uint_t red, uint_t green, uint_t blue, uint_t alpha) { glColor4ui (red, green, blue, alpha); }
inline void color4 (uint_t red, uint_t green, uint_t blue, uint_t alpha) { glColor4ui (red, green, blue, alpha); }
inline void color4 (const uint_t *v)			    { glColor4uiv (v); }
inline void color  (ushort_t red, ushort_t green, ushort_t blue, ushort_t alpha) { glColor4us (red, green, blue, alpha); }
inline void color4 (ushort_t red, ushort_t green, ushort_t blue, ushort_t alpha) { glColor4us (red, green, blue, alpha); }
inline void color4 (const ushort_t *v)		    { glColor4usv (v); }
inline void index(int_t i)				    { glIndexi(i); }

struct begin_context {
  begin_context(begin_mode m) { gl::begin(m); }
  ~begin_context()  { gl::end(); }

  void vertex  (double_t x, double_t y)		    { glVertex2d (x, y); }
  void vertex2 (double_t x, double_t y)		    { glVertex2d (x, y); }
  void vertex2 (const double_t *v)		    { glVertex2dv (v); }
  void vertex  (float_t x, float_t y)		    { glVertex2f (x, y); }
  void vertex2 (float_t x, float_t y)		    { glVertex2f (x, y); }
  void vertex2 (const float_t *v)		    { glVertex2fv (v); }
  void vertex  (int_t x, int_t y)		    { glVertex2i (x, y); }
  void vertex2 (int_t x, int_t y)		    { glVertex2i (x, y); }
  void vertex2 (const int_t *v)			    { glVertex2iv (v); }
  void vertex  (short_t x, short_t y)		    { glVertex2s (x, y); }
  void vertex2 (short_t x, short_t y)		    { glVertex2s (x, y); }
  void vertex2 (const short_t *v)		    { glVertex2sv (v); }
  void vertex  (double_t x, double_t y, double_t z)  { glVertex3d (x, y, z); }
  void vertex3 (double_t x, double_t y, double_t z)  { glVertex3d (x, y, z); }
  void vertex3 (const double_t *v)		    { glVertex3dv (v); }
  void vertex  (float_t x, float_t y, float_t z)	    { glVertex3f (x, y, z); }
  void vertex3 (float_t x, float_t y, float_t z)	    { glVertex3f (x, y, z); }
  void vertex3 (const float_t *v)		    { glVertex3fv (v); }
  void vertex  (int_t x, int_t y, int_t z)	    { glVertex3i (x, y, z); }
  void vertex3 (int_t x, int_t y, int_t z)	    { glVertex3i (x, y, z); }
  void vertex3 (const int_t *v)			    { glVertex3iv (v); }
  void vertex  (short_t x, short_t y, short_t z)	    { glVertex3s (x, y, z); }
  void vertex3 (short_t x, short_t y, short_t z)	    { glVertex3s (x, y, z); }
  void vertex3 (const short_t *v)		    { glVertex3sv (v); }
  void vertex  (double_t x, double_t y, double_t z, double_t w) { glVertex4d (x, y, z, w); }
  void vertex4 (double_t x, double_t y, double_t z, double_t w) { glVertex4d (x, y, z, w); }
  void vertex4 (const double_t *v)		    { glVertex4dv (v); }
  void vertex  (float_t x, float_t y, float_t z, float_t w) { glVertex4f (x, y, z, w); }
  void vertex4 (float_t x, float_t y, float_t z, float_t w) { glVertex4f (x, y, z, w); }
  void vertex4 (const float_t *v)		    { glVertex4fv (v); }
  void vertex  (int_t x, int_t y, int_t z, int_t w)  { glVertex4i (x, y, z, w); }
  void vertex4 (int_t x, int_t y, int_t z, int_t w)  { glVertex4i (x, y, z, w); }
  void vertex4 (const int_t *v)			    { glVertex4iv (v); }
  void vertex  (short_t x, short_t y, short_t z, short_t w) { glVertex4s (x, y, z, w); }
  void vertex4 (short_t x, short_t y, short_t z, short_t w) { glVertex4s (x, y, z, w); }
  void vertex4 (const short_t *v)		    { glVertex4sv (v); }

  void color  (byte_t red, byte_t green, byte_t blue) { glColor3b (red, green, blue); }
  void color3 (byte_t red, byte_t green, byte_t blue) { glColor3b (red, green, blue); }
  void color3 (const byte_t *v)			    { glColor3bv (v); }
  void color  (double_t red, double_t green, double_t blue) { glColor3d (red, green, blue); }
  void color3 (double_t red, double_t green, double_t blue) { glColor3d (red, green, blue); }
  void color3 (const double_t *v)		    { glColor3dv (v); }
  void color  (float_t red, float_t green, float_t blue) { glColor3f (red, green, blue); }
  void color3 (float_t red, float_t green, float_t blue) { glColor3f (red, green, blue); }
  void color3 (const float_t *v)			    { glColor3fv (v); }
  void color  (int_t red, int_t green, int_t blue)   { glColor3i (red, green, blue); }
  void color3 (int_t red, int_t green, int_t blue)   { glColor3i (red, green, blue); }
  void color3 (const int_t *v)			    { glColor3iv (v); }
  void color  (short_t red, short_t green, short_t blue) { glColor3s (red, green, blue); }
  void color3 (short_t red, short_t green, short_t blue) { glColor3s (red, green, blue); }
  void color3 (const short_t *v)			    { glColor3sv (v); }
  void color  (ubyte_t red, ubyte_t green, ubyte_t blue) { glColor3ub (red, green, blue); }
  void color3 (ubyte_t red, ubyte_t green, ubyte_t blue) { glColor3ub (red, green, blue); }
  void color3 (const ubyte_t *v)			    { glColor3ubv (v); }
  void color  (uint_t red, uint_t green, uint_t blue) { glColor3ui (red, green, blue); }
  void color3 (uint_t red, uint_t green, uint_t blue) { glColor3ui (red, green, blue); }
  void color3 (const uint_t *v)			    { glColor3uiv (v); }
  void color  (ushort_t red, ushort_t green, ushort_t blue) { glColor3us (red, green, blue); }
  void color3 (ushort_t red, ushort_t green, ushort_t blue) { glColor3us (red, green, blue); }
  void color3 (const ushort_t *v)		    { glColor3usv (v); }
  void color  (byte_t red, byte_t green, byte_t blue, byte_t alpha) { glColor4b (red, green, blue, alpha); }
  void color4 (byte_t red, byte_t green, byte_t blue, byte_t alpha) { glColor4b (red, green, blue, alpha); }
  void color4 (const byte_t *v)			    { glColor4bv (v); }
  void color  (double_t red, double_t green, double_t blue, double_t alpha) { glColor4d (red, green, blue, alpha); }
  void color4 (double_t red, double_t green, double_t blue, double_t alpha) { glColor4d (red, green, blue, alpha); }
  void color4 (const double_t *v)		    { glColor4dv (v); }
  void color  (float_t red, float_t green, float_t blue, float_t alpha) { glColor4f (red, green, blue, alpha); }
  void color4 (float_t red, float_t green, float_t blue, float_t alpha) { glColor4f (red, green, blue, alpha); }
  void color4 (const float_t *v)			    { glColor4fv (v); }
  void color  (int_t red, int_t green, int_t blue, int_t alpha) { glColor4i (red, green, blue, alpha); }
  void color4 (int_t red, int_t green, int_t blue, int_t alpha) { glColor4i (red, green, blue, alpha); }
  void color4 (const int_t *v)			    { glColor4iv (v); }
  void color  (short_t red, short_t green, short_t blue, short_t alpha) { glColor4s (red, green, blue, alpha); }
  void color4 (short_t red, short_t green, short_t blue, short_t alpha) { glColor4s (red, green, blue, alpha); }
  void color4 (const short_t *v)			    { glColor4sv (v); }
  void color  (ubyte_t red, ubyte_t green, ubyte_t blue, ubyte_t alpha) { glColor4ub (red, green, blue, alpha); }
  void color4 (ubyte_t red, ubyte_t green, ubyte_t blue, ubyte_t alpha) { glColor4ub (red, green, blue, alpha); }
  void color4 (const ubyte_t *v)			    { glColor4ubv (v); }
  void color  (uint_t red, uint_t green, uint_t blue, uint_t alpha) { glColor4ui (red, green, blue, alpha); }
  void color4 (uint_t red, uint_t green, uint_t blue, uint_t alpha) { glColor4ui (red, green, blue, alpha); }
  void color4 (const uint_t *v)			    { glColor4uiv (v); }
  void color  (ushort_t red, ushort_t green, ushort_t blue, ushort_t alpha) { glColor4us (red, green, blue, alpha); }
  void color4 (ushort_t red, ushort_t green, ushort_t blue, ushort_t alpha) { glColor4us (red, green, blue, alpha); }
  void color4 (const ushort_t *v)		    { glColor4usv (v); }
  void index(int_t i)				    { glIndexi(i); }

  void tex_coord1(double_t s) { glTexCoord1d (s); }
  void tex_coord (double_t s) { glTexCoord1d (s); }
  void tex_coord1 (const double_t *v) { glTexCoord1dv (v); }
  void tex_coord1(float_t s) { glTexCoord1f (s); }
  void tex_coord (float_t s) { glTexCoord1f (s); }
  void tex_coord1 (const float_t *v) { glTexCoord1fv (v); }
  void tex_coord1(int_t s) { glTexCoord1i (s); }
  void tex_coord (int_t s) { glTexCoord1i (s); }
  void tex_coord1 (const int_t *v) { glTexCoord1iv (v); }
  void tex_coord1(short_t s) { glTexCoord1s (s); }
  void tex_coord (short_t s) { glTexCoord1s (s); }
  void tex_coord1 (const short_t *v) { glTexCoord1sv (v); }
  void tex_coord2(double_t s, double_t t) { glTexCoord2d (s, t); }
  void tex_coord (double_t s, double_t t) { glTexCoord2d (s, t); }
  void tex_coord2 (const double_t *v) { glTexCoord2dv (v); }
  void tex_coord2(float_t s, float_t t) { glTexCoord2f (s, t); }
  void tex_coord (float_t s, float_t t) { glTexCoord2f (s, t); }
  void tex_coord2 (const float_t *v) { glTexCoord2fv (v); }
  void tex_coord2(int_t s, int_t t) { glTexCoord2i (s, t); }
  void tex_coord (int_t s, int_t t) { glTexCoord2i (s, t); }
  void tex_coord2 (const int_t *v) { glTexCoord2iv (v); }
  void tex_coord2(short_t s, short_t t) { glTexCoord2s (s, t); }
  void tex_coord (short_t s, short_t t) { glTexCoord2s (s, t); }
  void tex_coord2 (const short_t *v) { glTexCoord2sv (v); }
  void tex_coord3(double_t s, double_t t, double_t r) { glTexCoord3d (s, t, r); }
  void tex_coord (double_t s, double_t t, double_t r) { glTexCoord3d (s, t, r); }
  void tex_coord3 (const double_t *v) { glTexCoord3dv (v); }
  void tex_coord3(float_t s, float_t t, float_t r) { glTexCoord3f (s, t, r); }
  void tex_coord (float_t s, float_t t, float_t r) { glTexCoord3f (s, t, r); }
  void tex_coord3 (const float_t *v) { glTexCoord3fv (v); }
  void tex_coord3(int_t s, int_t t, int_t r) { glTexCoord3i (s, t, r); }
  void tex_coord (int_t s, int_t t, int_t r) { glTexCoord3i (s, t, r); }
  void tex_coord3 (const int_t *v) { glTexCoord3iv (v); }
  void tex_coord3(short_t s, short_t t, short_t r) { glTexCoord3s (s, t, r); }
  void tex_coord (short_t s, short_t t, short_t r) { glTexCoord3s (s, t, r); }
  void tex_coord3 (const short_t *v) { glTexCoord3sv (v); }
  void tex_coord4(double_t s, double_t t, double_t r, double_t q) { glTexCoord4d (s, t, r, q); }
  void tex_coord (double_t s, double_t t, double_t r, double_t q) { glTexCoord4d (s, t, r, q); }
  void tex_coord4 (const double_t *v) { glTexCoord4dv (v); }
  void tex_coord4(float_t s, float_t t, float_t r, float_t q) { glTexCoord4f (s, t, r, q); }
  void tex_coord (float_t s, float_t t, float_t r, float_t q) { glTexCoord4f (s, t, r, q); }
  void tex_coord4 (const float_t *v) { glTexCoord4fv (v); }
  void tex_coord4(int_t s, int_t t, int_t r, int_t q) { glTexCoord4i (s, t, r, q); }
  void tex_coord (int_t s, int_t t, int_t r, int_t q) { glTexCoord4i (s, t, r, q); }
  void tex_coord4 (const int_t *v) { glTexCoord4iv (v); }
  void tex_coord4(short_t s, short_t t, short_t r, short_t q) { glTexCoord4s (s, t, r, q); }
  void tex_coord (short_t s, short_t t, short_t r, short_t q) { glTexCoord4s (s, t, r, q); }
  void tex_coord4 (const short_t *v) { glTexCoord4sv (v); }
};
	
struct points_context : begin_context {
  points_context() : begin_context(bm_points) {}
};
struct lines_context : begin_context {
  lines_context() : begin_context(bm_lines) {}
};
struct begin_points : public begin_context {
  begin_points() : begin_context(bm_points) {}
};
struct begin_lines : public begin_context {
  begin_lines() : begin_context(bm_lines) {}
};
struct begin_line_strip : public begin_context {
  begin_line_strip() : begin_context(bm_line_strip) {}
};
struct begin_line_loop : public begin_context {
  begin_line_loop() : begin_context(bm_line_loop) {}
};
struct begin_triangles   : public begin_context {
  begin_triangles() : begin_context(bm_triangles) {}
};
struct begin_triangle_strip : public begin_context {
  begin_triangle_strip() : begin_context(bm_triangle_strip) {}
};
struct begin_triangle_fan : public begin_context {
  begin_triangle_fan() : begin_context(bm_triangle_fan) {}
};
struct begin_quads : public begin_context {
  begin_quads() : begin_context(bm_quads) {}
};
struct begin_quad_strip   : public begin_context {
  begin_quad_strip() : begin_context(bm_quad_strip) {}
};
struct begin_polygon : public begin_context {
  begin_polygon() : begin_context(bm_polygon) {}
};
  
// clear
	
enum clear_bit {
  color_buffer = GL_COLOR_BUFFER_BIT,
  depth_buffer = GL_DEPTH_BUFFER_BIT,
  stencil_buffer = GL_STENCIL_BUFFER_BIT
};
	
struct clear_mask {
  int value;
  clear_mask() : value(0) {}
  clear_mask(const clear_bit& m) : value(m) {}
};
	
inline clear_mask
operator|(const clear_bit& m1, const clear_bit& m2) {
  clear_mask cmu;
  cmu.value = static_cast<int>(m1) | m2;
  return cmu;
}

inline clear_mask
operator|(const clear_mask& cmu, const clear_bit& m) {
  clear_mask rcmu;
  rcmu.value = cmu.value | static_cast<int>(m);
  return rcmu;
}

inline void clear(const clear_mask& m) { glClear(m.value); DBG}

// push_attrib

enum attribute_bit {
  current = GL_CURRENT_BIT,
  point = GL_POINT_BIT,
  line = GL_LINE_BIT,
  // maybe we shoud add a _bit suffix to all attribute bits, to avoid the conflict with gl::polygon in begin_mode
  // and to be consistent with the C API
  attribute_bit_polygon = GL_POLYGON_BIT, // shared value
  polygon_stipple = GL_POLYGON_STIPPLE_BIT,
  pixel_mode = GL_PIXEL_MODE_BIT,
  lighting = GL_LIGHTING_BIT,
  fog = GL_FOG_BIT,
  //		depth_buffer = GL_DEPTH_BUFFER_BIT,
  //		accum_buffer = GL_ACCUM_BUFFER_BIT,
  //		stencil_buffer = GL_STENCIL_BUFFER_BIT,
  viewport = GL_VIEWPORT_BIT,
  transform = GL_TRANSFORM_BIT,
  enable = GL_ENABLE_BIT,
  //		color_buffer = GL_COLOR_BUFFER_BIT,
  hint = GL_HINT_BIT,
  eval = GL_EVAL_BIT,
  list = GL_LIST_BIT,
  texture = GL_TEXTURE_BIT,
  scissor = GL_SCISSOR_BIT,
  all_attrib = GL_ALL_ATTRIB_BITS,
  /*		multisample = GL_MULTISAMPLE_BIT_EXT*/
};

struct attribute_mask {
  int value;
  attribute_mask() : value(0) {}
  attribute_mask(const attribute_bit& m) : value(m) {}
};
	
inline attribute_mask
operator|(const attribute_bit& m1, const attribute_bit& m2) {
  attribute_mask cmu;
  cmu.value = static_cast<int>(m1) | m2;
  return cmu;
}

inline attribute_mask
operator|(const attribute_mask& cmu, const attribute_bit& m) {
  attribute_mask rcmu;
  rcmu.value = cmu.value | static_cast<int>(m);
  return rcmu;
}

inline void push_attrib(const attribute_mask& atts) { glPushAttrib(atts.value); DBG }
inline void pop_attrib() { glPopAttrib(); DBG }
struct save_attrib {
  save_attrib(const attribute_mask& atts) { push_attrib(atts); }
  ~save_attrib() { pop_attrib(); }
};

enum src_blend_factor {
  zero_src_blend_factor = GL_ZERO, // shared value
  one_src_blend_factor = GL_ONE, // shared value
  src_color = GL_SRC_COLOR,
  one_minus_src_color = GL_ONE_MINUS_SRC_COLOR,
};

enum dst_blend_factor {
  zero_dst_blend_factor = GL_ZERO, // shared value
  one_dst_blend_factor = GL_ONE, // shared value
  dst_color = GL_SRC_COLOR,
  one_minus_dst_color = GL_ONE_MINUS_DST_COLOR
};

inline void
blend_func(src_blend_factor src, dst_blend_factor dst) { glBlendFunc(src,dst); DBG}

enum stencil_operation {
  zero_stencil_operation = GL_ZERO, // shared value
  keep = GL_KEEP,
  replace = GL_REPLACE,
  incr = GL_INCR,
  decr = GL_DECR,
  invert = GL_INVERT
};

inline void
stencil_op(stencil_operation fail, stencil_operation zfail, stencil_operation zpass) { glStencilOp(fail,zfail,zpass); DBG}


// shared values
	
struct zero_value {
  zero_value(int) {}
  operator src_blend_factor() const { return zero_src_blend_factor; }
  operator dst_blend_factor() const { return zero_dst_blend_factor; }
  operator stencil_operation() const { return zero_stencil_operation; }
};
static const zero_value zero=0;

struct one_value {
  one_value(int) {}
  operator src_blend_factor() const { return one_src_blend_factor; }
  operator dst_blend_factor() const { return one_dst_blend_factor; }
};
static const one_value one=0;

struct polygon_value {
  polygon_value(int) {}
  operator begin_mode() const { return bm_polygon; }
  operator attribute_bit() const { return attribute_bit_polygon; }
};
static const polygon_value polygon=0;

inline void raster_pos  (double_t x, double_t y)	    { glRasterPos2d (x, y); }
inline void raster_pos2 (double_t x, double_t y)	    { glRasterPos2d (x, y); }
inline void raster_pos2 (const double_t *v)		    { glRasterPos2dv (v); }
inline void raster_pos  (float_t x, float_t y)	    { glRasterPos2f (x, y); }
inline void raster_pos2 (float_t x, float_t y)	    { glRasterPos2f (x, y); }
inline void raster_pos2 (const float_t *v)		    { glRasterPos2fv (v); }
inline void raster_pos  (int_t x, int_t y)		    { glRasterPos2i (x, y); }
inline void raster_pos2 (int_t x, int_t y)		    { glRasterPos2i (x, y); }
inline void raster_pos2 (const int_t *v)		    { glRasterPos2iv (v); }
inline void raster_pos  (short_t x, short_t y)	    { glRasterPos2s (x, y); }
inline void raster_pos2 (short_t x, short_t y)	    { glRasterPos2s (x, y); }
inline void raster_pos2 (const short_t *v)		    { glRasterPos2sv (v); }
inline void raster_pos  (double_t x, double_t y, double_t z) { glRasterPos3d (x, y, z); }
inline void raster_pos3 (double_t x, double_t y, double_t z) { glRasterPos3d (x, y, z); }
inline void raster_pos3 (const double_t *v)		    { glRasterPos3dv (v); }
inline void raster_pos  (float_t x, float_t y, float_t z) { glRasterPos3f (x, y, z); }
inline void raster_pos3 (float_t x, float_t y, float_t z) { glRasterPos3f (x, y, z); }
inline void raster_pos3 (const float_t *v)		    { glRasterPos3fv (v); }
inline void raster_pos  (int_t x, int_t y, int_t z)	    { glRasterPos3i (x, y, z); }
inline void raster_pos3 (int_t x, int_t y, int_t z)	    { glRasterPos3i (x, y, z); }
inline void raster_pos3 (const int_t *v)		    { glRasterPos3iv (v); }
inline void raster_pos  (short_t x, short_t y, short_t z) { glRasterPos3s (x, y, z); }
inline void raster_pos3 (short_t x, short_t y, short_t z) { glRasterPos3s (x, y, z); }
inline void raster_pos3 (const short_t *v)		    { glRasterPos3sv (v); }
inline void raster_pos  (double_t x, double_t y, double_t z, double_t w) { glRasterPos4d (x, y, z, w); }
inline void raster_pos4 (double_t x, double_t y, double_t z, double_t w) { glRasterPos4d (x, y, z, w); }
inline void raster_pos4 (const double_t *v)		    { glRasterPos4dv (v); }
inline void raster_pos  (float_t x, float_t y, float_t z, float_t w) { glRasterPos4f (x, y, z, w); }
inline void raster_pos4 (float_t x, float_t y, float_t z, float_t w) { glRasterPos4f (x, y, z, w); }
inline void raster_pos4 (const float_t *v)		    { glRasterPos4fv (v); }
inline void raster_pos  (int_t x, int_t y, int_t z, int_t w) { glRasterPos4i (x, y, z, w); }
inline void raster_pos4 (int_t x, int_t y, int_t z, int_t w) { glRasterPos4i (x, y, z, w); }
inline void raster_pos4 (const int_t *v)		    { glRasterPos4iv (v); }
inline void raster_pos  (short_t x, short_t y, short_t z, short_t w) { glRasterPos4s (x, y, z, w); }
inline void raster_pos4 (short_t x, short_t y, short_t z, short_t w) { glRasterPos4s (x, y, z, w); }
inline void raster_pos4 (const short_t *v)		    { glRasterPos4sv (v); }

inline void rect (double_t x1, double_t y1, double_t x2, double_t y2) { glRectd (x1, y1, x2, y2); }
inline void rect (const double_t *v1, const double_t *v2) { glRectdv (v1, v2); }
inline void rect (float_t x1, float_t y1, float_t x2, float_t y2) { glRectf (x1, y1, x2, y2); }
inline void rect (const float_t *v1, const float_t *v2)   { glRectfv (v1, v2); }
inline void rect (int_t x1, int_t y1, int_t x2, int_t y2) { glRecti (x1, y1, x2, y2); }
inline void rect (const int_t *v1, const int_t *v2)	    { glRectiv (v1, v2); }
inline void rect (short_t x1, short_t y1, short_t x2, short_t y2) { glRects (x1, y1, x2, y2); }
inline void rect (const short_t *v1, const short_t *v2)   { glRectsv (v1, v2); }

inline void multmatrix(const double_t * m) 		    { glMultMatrixd(m); }
inline void multmatrix(const float_t * m) 		    { glMultMatrixf(m); }
inline void rotate(double_t angle, double_t x, double_t y, double_t z) { glRotated(angle, x, y, z); }
inline void rotate(float_t angle, float_t x, float_t y, float_t z = 0) { glRotatef(angle, x, y, z); }
inline void scale(double_t x, double_t y, double_t z = 0) { glScaled(x, y, z); }
inline void scale(float_t x, float_t y, float_t z = 0) { glScalef(x, y, z); }
inline void translate(double_t x, double_t y, double_t z = 0) { glTranslated(x, y, z); }
inline void translate(float_t x, float_t y, float_t z = 0) { glTranslatef(x, y, z); }
inline void tex_coord1(double_t s) { glTexCoord1d (s); }
inline void tex_coord (double_t s) { glTexCoord1d (s); }
inline void tex_coord1 (const double_t *v) { glTexCoord1dv (v); }
inline void tex_coord1(float_t s) { glTexCoord1f (s); }
inline void tex_coord (float_t s) { glTexCoord1f (s); }
inline void tex_coord1 (const float_t *v) { glTexCoord1fv (v); }
inline void tex_coord1(int_t s) { glTexCoord1i (s); }
inline void tex_coord (int_t s) { glTexCoord1i (s); }
inline void tex_coord1 (const int_t *v) { glTexCoord1iv (v); }
inline void tex_coord1(short_t s) { glTexCoord1s (s); }
inline void tex_coord (short_t s) { glTexCoord1s (s); }
inline void tex_coord1 (const short_t *v) { glTexCoord1sv (v); }
inline void tex_coord2(double_t s, double_t t) { glTexCoord2d (s, t); }
inline void tex_coord (double_t s, double_t t) { glTexCoord2d (s, t); }
inline void tex_coord2 (const double_t *v) { glTexCoord2dv (v); }
inline void tex_coord2(float_t s, float_t t) { glTexCoord2f (s, t); }
inline void tex_coord (float_t s, float_t t) { glTexCoord2f (s, t); }
inline void tex_coord2 (const float_t *v) { glTexCoord2fv (v); }
inline void tex_coord2(int_t s, int_t t) { glTexCoord2i (s, t); }
inline void tex_coord (int_t s, int_t t) { glTexCoord2i (s, t); }
inline void tex_coord2 (const int_t *v) { glTexCoord2iv (v); }
inline void tex_coord2(short_t s, short_t t) { glTexCoord2s (s, t); }
inline void tex_coord (short_t s, short_t t) { glTexCoord2s (s, t); }
inline void tex_coord2 (const short_t *v) { glTexCoord2sv (v); }
inline void tex_coord3(double_t s, double_t t, double_t r) { glTexCoord3d (s, t, r); }
inline void tex_coord (double_t s, double_t t, double_t r) { glTexCoord3d (s, t, r); }
inline void tex_coord3 (const double_t *v) { glTexCoord3dv (v); }
inline void tex_coord3(float_t s, float_t t, float_t r) { glTexCoord3f (s, t, r); }
inline void tex_coord (float_t s, float_t t, float_t r) { glTexCoord3f (s, t, r); }
inline void tex_coord3 (const float_t *v) { glTexCoord3fv (v); }
inline void tex_coord3(int_t s, int_t t, int_t r) { glTexCoord3i (s, t, r); }
inline void tex_coord (int_t s, int_t t, int_t r) { glTexCoord3i (s, t, r); }
inline void tex_coord3 (const int_t *v) { glTexCoord3iv (v); }
inline void tex_coord3(short_t s, short_t t, short_t r) { glTexCoord3s (s, t, r); }
inline void tex_coord (short_t s, short_t t, short_t r) { glTexCoord3s (s, t, r); }
inline void tex_coord3 (const short_t *v) { glTexCoord3sv (v); }
inline void tex_coord4(double_t s, double_t t, double_t r, double_t q) { glTexCoord4d (s, t, r, q); }
inline void tex_coord (double_t s, double_t t, double_t r, double_t q) { glTexCoord4d (s, t, r, q); }
inline void tex_coord4 (const double_t *v) { glTexCoord4dv (v); }
inline void tex_coord4(float_t s, float_t t, float_t r, float_t q) { glTexCoord4f (s, t, r, q); }
inline void tex_coord (float_t s, float_t t, float_t r, float_t q) { glTexCoord4f (s, t, r, q); }
inline void tex_coord4 (const float_t *v) { glTexCoord4fv (v); }
inline void tex_coord4(int_t s, int_t t, int_t r, int_t q) { glTexCoord4i (s, t, r, q); }
inline void tex_coord (int_t s, int_t t, int_t r, int_t q) { glTexCoord4i (s, t, r, q); }
inline void tex_coord4 (const int_t *v) { glTexCoord4iv (v); }
inline void tex_coord4(short_t s, short_t t, short_t r, short_t q) { glTexCoord4s (s, t, r, q); }
inline void tex_coord (short_t s, short_t t, short_t r, short_t q) { glTexCoord4s (s, t, r, q); }
inline void tex_coord4 (const short_t *v) { glTexCoord4sv (v); }

inline void copy_pixels (int_t x, int_t y, sizei_t width, sizei_t height,
			 PixelCopyType type)
{ glCopyPixels(x, y, width, height, type); }
inline void draw_pixels (sizei_t width, sizei_t height,
			 PixelFormat format, PixelType type, const void *pixels)
{ glDrawPixels(width, height, format, type, pixels); }
inline void read_pixels (int_t x, int_t y, sizei_t width, sizei_t height,
			 PixelFormat format, PixelType type, void *pixels)
{ glReadPixels(x, y, width, height, format, type, pixels); } 
    

#undef DBG
}

#endif // __glcc__
 


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
#ifndef INFOVIS_DRAWING_IMAGE_HPP
#define INFOVIS_DRAWING_IMAGE_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/gl.hpp>
#include <string>

namespace infovis {

/**
 * Loading and saving of images to and from OpenGL.
 */
class Image : public gc_cleanup
{
public:
  Image();
  Image(unsigned w, unsigned h, 
	gl::PixelFormat format = gl::pf_rgb,
	gl::PixelType type = gl::pt_unsigned_byte);
  virtual ~Image();
  virtual int getWidth() const;
  virtual int getHeight() const;
  virtual gl::PixelFormat getFormat() const;
  virtual gl::PixelType getType() const;
    
  virtual void * getPixels() const;
  virtual void render(float x, float y);
  virtual void render();
  virtual void read(int x, int y);
  virtual void read(int x, int y, gl::PixelFormat format);

  static Image * load(const std::string& filename);
  static Image * read(int x, int y, unsigned w, unsigned h,
		      gl::PixelFormat format = gl::pf_rgb,
		      gl::PixelType type = gl::pt_unsigned_byte);
  static int size(gl::PixelFormat format, gl::PixelType type);
protected:
  unsigned width_, height_;
  gl::PixelFormat format_;
  gl::PixelType type_;
  unsigned char * pixels_;
    
public:
  /**
   * Loader class for images
   */
  class Loader {
  protected:
    static Loader * first;
    Loader * next;

    Loader();
    virtual ~Loader();
    friend class Image;
  public:
    virtual Image * load(const std::string& name) = 0;
  };

};

} // namespace infovis

#endif // INFOVIS_DRAWING_IMAGE_HPP

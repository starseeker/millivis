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
#include <infovis/drawing/Texture.hpp>
#
#include <GL/glu.h>
#include <iostream>

namespace infovis {

  static int next_pow2(int s)
  {
    int p = 0;
    while (s != 0) {
      p++;
      s >>= 1;
    }
    return 1 << p;
  }

  Texture::Texture() { }

  Texture::Texture(unsigned w, unsigned h,
		   gl::PixelFormat format,
		   gl::PixelType type)
    : Image(next_pow2(w), next_pow2(h), format, type)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, format_, 
		 width_, height_, 0, format_, type_, 0);
  }

  Texture::Texture(const Image * img)
    : Image(next_pow2(img->getWidth()),
	    next_pow2(img->getHeight()),
	    img->getFormat(), img->getType())
  {
    unsigned char * out = new unsigned char[width_ * height_ *
					   size(format_, type_)];
    if (gluScaleImage(format_,
		      img->getWidth(), img->getHeight(),
		      type_, img->getPixels(),
		      width_, height_, type_, out) != 0)
      std::cerr << "Error scaling image\n";
    pixels_ = out;
    glTexImage2D(GL_TEXTURE_2D, 0, format_, 
		 width_, height_, 0, format_, type_, pixels_);
  }


} // namespace infovis


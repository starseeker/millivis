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
#include <infovis/drawing/ImagePNG.hpp>
#include <infovis/drawing/gl.hpp>
#include <png.h>
#include <stdio.h>

namespace infovis {

static int align(int w)
{
  switch(w&3) {
  case 1:
    return w+3;
  case 2:
    return w+2;
  case 3:
    return w+1;
  }
  return w;
}

Image *
ImagePNG::Loader::load(const std::string& name)
{
  png_structp png_ptr;
  png_infop info_ptr;
  png_infop end_info;

  FILE * file = ::fopen(name.c_str(), "rb");
  if (file == 0)
    return 0;
  {
    png_byte header[9];
    fread(header, 1, 8, file);
    if(png_sig_cmp(header, 0, 8)) {
      //STDDBG;
      return 0;
    }
  }
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png_ptr) {
    return 0;
  }
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr,
			    (png_infopp)NULL, (png_infopp)NULL);
    return 0;
  }
  end_info = png_create_info_struct(png_ptr);
  if (!end_info) {
    png_destroy_read_struct(&png_ptr, &info_ptr,
			    (png_infopp)NULL);
    return 0;
  }
  if (setjmp(png_ptr->jmpbuf)) {
    png_destroy_read_struct(&png_ptr, &info_ptr,
			    &end_info);
    return 0;
  }
  png_init_io(png_ptr, file);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  ImagePNG * res = new ImagePNG;
  int width, height;
  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  res->width_ = width;
  res->height_ = height;
  int depth = png_get_bit_depth(png_ptr, info_ptr);
  int color_type = png_get_color_type(png_ptr, info_ptr);
  bool ok=false;
  unsigned int size = 0;
  unsigned int stride = 0;

  switch (color_type) {
  case PNG_COLOR_TYPE_RGB:
    res->format_ = gl::pf_rgb;
    switch(depth) {
    case 8:
      res->type_ = gl::pt_unsigned_byte;
      stride = align(width * 3);
      ok=true;
      break;
    case 16:
      res->type_ = gl::pt_unsigned_short;
      stride = align(width * 3 * 2);
      ok=true;
      break;
    }
    break;

  case PNG_COLOR_TYPE_RGB_ALPHA:
    res->format_  = gl::pf_rgba;
    switch(depth) {
    case 8:
      res->type_ = gl::pt_unsigned_byte;
      stride = align(width * 4);
      ok=true;
      break;
    case 16:
      res->type_ = gl::pt_unsigned_short;
      stride = align(width * 4 * 2);
      ok=true;
      break;
    }
    break;

  case PNG_COLOR_TYPE_GRAY:
    res->format_ = gl::pf_luminance;
    switch(depth) {
    case 1:
    case 2:
    case 4:
      break;
    case 8:
      res->type_ = gl::pt_unsigned_byte;
      stride = align(width);
      ok=true;
      break;
    case 16:
      res->type_ = gl::pt_unsigned_short;
      stride = align(width * 2);
      ok=true;
      break;
    }
    break;

  case PNG_COLOR_TYPE_GRAY_ALPHA:
    res->format_ = gl::pf_luminance_alpha;
    switch(depth) {
    case 8:
      res->type_ = gl::pt_unsigned_byte;
      stride = align(width * 2);
      ok=true;
      break;
    case 16:
      res->type_ = gl::pt_unsigned_short;
      stride = width * 2 * 2; // align(width * 2 * 2);
      ok=true;
      break;
    }
    break;

  case PNG_COLOR_TYPE_PALETTE:
    // Expand
    png_set_expand(png_ptr);
    res->format_ = gl::pf_rgba;
    res->type_ = gl::pt_unsigned_byte;
    stride = align(width * 4);
    ok=true;

  default:
    break;

  }
  if(!ok) {
    png_destroy_read_struct(&png_ptr, &info_ptr,
			    &end_info);
    delete res;
    return 0;
  }
  res->pixels_ = new unsigned char[stride * height];
  {
    int i;
    for(i=height-1; i>=0;--i) {
      png_read_row(png_ptr, &res->pixels_[i*stride], 0);
    }
  }
  png_read_end(png_ptr, end_info);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  return res;
}

static void error_throw_fn(struct png_struct_def *, const char *)
{
  throw "error";
}

static void warning_ignore_fn(struct png_struct_def *, const char *)
{ }

bool
ImagePNG::Loader::save(const std::string& name, Image * img)
{
  png_structp png_ptr;
  png_infop info_ptr;
  bool ret = false;

  /* open the file */
  FILE * fp = fopen(name.c_str(), "wb");
  if (fp == NULL)
    return ret;

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				    NULL,
				    error_throw_fn,
				    warning_ignore_fn);

  if (png_ptr == NULL) {
    fclose(fp);
    return ret;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    fclose(fp);
    png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
    return ret;
  }
   
  png_init_io(png_ptr, fp);

  png_set_IHDR(png_ptr, info_ptr,
	       img->getWidth(), img->getHeight(), 8,
	       PNG_COLOR_TYPE_RGB,
	       PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_DEFAULT,
	       PNG_FILTER_TYPE_DEFAULT);
  png_color_8 sig_bit;
  sig_bit.red = 8;
  sig_bit.green = 8;
  sig_bit.blue = 8;
  png_set_sBIT(png_ptr, info_ptr, &sig_bit);

  png_write_info(png_ptr, info_ptr);
  png_byte * image = (png_byte*)img->getPixels();
  png_uint_32 width = img->getWidth();
  png_uint_32 height = img->getHeight();
  png_bytep * row_pointers = new png_bytep[height];

  for (int k = 0; k < height; k++)
    row_pointers[k] = image + (height-k-1)*width*3;

  try {
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);
    ret = true;
  }
  catch(const char* ) {
    /* If we get here, we had a problem reading the file */
    ret = false;
  }

  delete[] row_pointers;
  fclose(fp);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  return ret;
}

  
static ImagePNG::Loader * loader;

void installImagePNG()
{
  if (loader == 0)
    loader = new ImagePNG::Loader;
}


} // namespace infovis



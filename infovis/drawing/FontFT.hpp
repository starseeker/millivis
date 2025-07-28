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
#ifndef INFOVIS_DRAWING_FONTFT_HPP
#define INFOVIS_DRAWING_FONTFT_HPP

#include <infovis/drawing/Font.hpp>

namespace infovis {

/**
 * Implementation of FreeType based fonts.
 */
class FontFTOutline : public Font
{
protected:
  unsigned int list_base;
  float ascent;
  float descent;
  int widths[256];
  FontFTOutline(const string& name, Style style, float size)
    : Font(name, style, size) { }
public:
  FontFTOutline(void * t,
		const string& name, Style style, float size);

  virtual bool install();
  virtual void finish();

  virtual Format getFormat() const;
  virtual bool isFixedWidth() const;
  virtual void paint(const string& str, float x, float y);
  virtual float getLeading() const;
  virtual float getAscent() const;
  virtual float getDescent() const;
  virtual float charWidth(int ch) const;
  virtual float stringWidth(const string& str);
  virtual Box getStringBounds(const string& str);
};

/**
 * Bitmap Font using FreeType
 */
class FontFTBitmap : public FontFTOutline
{
protected:
  unsigned int texture;
  int tex_width, tex_height;
  unsigned int gtexture;
  int gtex_width, gtex_height;
  int char_width, char_height;
  bool is_bitmap;
  struct CharInfo {
    int xmin, xmax, ymin, ymax, advance, xbearing, ybearing;
    unsigned char * pixmap;
  };
  CharInfo info[256];
  void fill_texture(unsigned char * tex,
		    int tex_width, int tex_height,
		    int from_x, int from_y,
		    const CharInfo& info);
  void dilate_and_fill_texture(unsigned char * tex,
			       int tex_width, int tex_height,
			       int from_x, int from_y,
			       const CharInfo& info);
  void create_textures();
public:

  FontFTBitmap(void * t,
	       const string& name, Style style, float size);

  virtual Format getFormat() const;
  virtual void paint(const string& str, float x, float y);
  virtual void paintGrown(const string& str, float x, float y);
};

extern void installFontFT();

/**
 * Creator of FreeType fonts
 */
class FontFTCreator : public Font::Creator {
protected:
  FontFTCreator();
  ~FontFTCreator();
  friend void installFontFT();
public:
  Font * create(const string& name, Font::Style style, float size);
  FontFTOutline * createOutline(const string& name, Font::Style style);
  FontFTBitmap * createBitmap(const string& name, Font::Style style, float size);
};


} // namespace infovis

#endif // INFOVIS_DRAWING_FONTFT_HPP

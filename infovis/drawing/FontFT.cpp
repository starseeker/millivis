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
#include <infovis/drawing/FontFT.hpp>
#include <infovis/drawing/gl.hpp>
#include <boost/directory.h>

#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftimage.h>

#include <iostream>
#include <map>

#include <cassert>

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

//#define PRINT
//#define SAVE

using std::string;

namespace infovis {

static int inited;
static FT_Library _FT_Library;
static FontFTCreator * font_ft_creator;
static const char * font_dirs[] = {
  "c:/winnt/fonts",
  "c:/WINDOWS/Fonts",
  NULL
};

FontFTOutline::FontFTOutline(void * t,
			     const string& name, Font::Style style, float sz)
  : Font(name, style, sz),
    list_base(0)
{
  FT_Face face = static_cast<FT_Face>(t);
  size = face->units_per_EM;
  ascent = face->ascender/64.0f;
  descent = -face->descender/64.0f;


  FT_Done_Face(face);
}

bool
FontFTOutline::install() { return true; }

void
FontFTOutline::finish() {}

Font::Format
FontFTOutline::getFormat() const { return format_outline; }

bool
FontFTOutline::isFixedWidth() const { return false; } // fixme

void
FontFTOutline::paint(const string& str, float x, float y)
{
  glListBase(list_base);
  glPushMatrix();
  glTranslatef(x, y, 0);
  glCallLists (str.length(), GL_UNSIGNED_BYTE, str.c_str());
  glPopMatrix();
}

float
FontFTOutline::getLeading() const
{
  return 0;
}

float
FontFTOutline::getAscent() const
{
  return ascent;
}

float
FontFTOutline::getDescent() const
{
  return descent;
}

float
FontFTOutline::charWidth(int ch) const
{
  if (ch < 0 || ch > 255)
    return 0;
  return widths[ch];
}

float
FontFTOutline::stringWidth(const string& str)
{
  float w = 0;
  for (string::const_iterator i = str.begin();
       i != str.end(); i++) {
    w += charWidth(*i);
  }
  return w;
}

Box
FontFTOutline::getStringBounds(const string& str)
{
  return Box(0, -descent,
	     stringWidth(str), ascent);
}


#define  FLOOR(x)    ((x) & -64)
#define  CEILING(x)  (((x)+63) & -64)

static void
chose_best_tex_size(int cw, int ch, int max_tex,
		    int& tex_width, int& tex_height)
{
  int min_surf = cw * ch * 256;
  tex_width = tex_height = max_tex; // conservative first estimate
  int cur_surf = tex_width * tex_height;
  int min_lost = cur_surf - min_surf;
  if (min_lost < 0)
    return;
  for (int width = max_tex; width > cw; width /= 2) {
    int num_per_line = width / cw;
    int fit_w = num_per_line * cw;
    int min_height = ((256 + num_per_line -1) / num_per_line) * ch;

    for (int height = tex_height; height >= min_height; height /= 2) {
      int lost = (width * height) - min_surf;
      if (lost < 0)
	break;
      if (lost < min_lost) {
	tex_width = width;
	tex_height = height;
	min_lost = lost;
      }
    }
  }
}

void
FontFTBitmap::fill_texture(unsigned char * tex,
			   int tex_width, int tex_height,
			   int from_x, int from_y,
			   const CharInfo& info)
{
  tex += from_y * tex_width + from_x;
  int to_x = from_x + info.xmax - info.xmin;
  int to_y = from_y + info.ymax - info.ymin;
  int stride = tex_width - (info.xmax - info.xmin);

  const unsigned char * from = info.pixmap;
  for (int y = info.ymin; y < info.ymax; y++) {
    for (int x = info.xmin; x < info.xmax; x++) {
      *tex++ = *from++;
    }
    tex += stride;
  }
  float round_x = 0; // 0.5f / tex_width;
  float round_y = 0; // 0.5f / tex_height;

#ifdef USE_LIST_FOR_BITMAPS
  glBegin(GL_QUADS);
  glTexCoord2f(float(from_x) / tex_width + round_x,
	       float(to_y) / tex_height + round_y);
  glVertex2f(info.xmin, info.ymin);
  glTexCoord2f(float(from_x) / tex_width + round_x,
	       float(from_y) / tex_height + round_y);
  glVertex2f(info.xmin, info.ymax);
  glTexCoord2f(float(to_x) / tex_width + round_x,
	       float(from_y) / tex_height + round_y);
  glVertex2f(info.xmax, info.ymax);
  glTexCoord2f(float(to_x) / tex_width + round_x,
	       float(to_y) / tex_height + round_y);
  glVertex2f(info.xmax, info.ymin);
  glEnd();
  glTranslatef(info.advance, 0, 0);
#endif
}

static inline unsigned char
dilate_compose(unsigned char c1, unsigned char c2)
{
  return ((c1 != 0) || (c2 != 0)) ? 255 : 0;
  //return std::max(c1, c2);
}

static void
dilate(unsigned char * to_image, int to_next_pixel, int to_next_line,
       unsigned char * from_image, int from_next_pixel, int from_next_line,
       int width, int height)
{
  unsigned char max0;
  unsigned char cur_pixel;

  for (int y = 0; y < height; y++) {
    cur_pixel = *from_image;
    from_image += from_next_pixel;
    max0 = cur_pixel;
    *to_image = max0;	// fill in border
    to_image += to_next_pixel;
    for (int x = 0; x < (width-1); x++) {
      unsigned char next_pixel = *from_image;
      from_image += from_next_pixel;
      unsigned char max1 = dilate_compose(cur_pixel, next_pixel);
      *to_image = dilate_compose(max0, max1);
      to_image += to_next_pixel;
      max0 = max1;
      cur_pixel = next_pixel;
    }
    *to_image = max0;
    to_image += to_next_pixel;
    *to_image = cur_pixel;
    to_image += to_next_pixel;
    to_image += to_next_line;
    from_image += from_next_line;
  }
}

void
FontFTBitmap::dilate_and_fill_texture(unsigned char * tex,
				      int tex_width, int tex_height,
				      int from_x, int from_y,
				      const CharInfo& info)
{
  tex += from_y * tex_width + from_x;
  int width = info.xmax - info.xmin;
  int height = info.ymax - info.ymin;

#ifdef SAVE
  {
    FILE * out = fopen("texture0.pgm", "wb");
    fprintf(out, "P5 %d %d 255\n", width, height);
    const unsigned char * p = info.pixmap;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
	fputc(*p++, out);
      }
    }
    fclose(out);
  }
#endif
  dilate(tex + tex_width, 1, tex_width - (width + 2),
	 info.pixmap, 1, 0,
	 width, height);
#ifdef SAVE
  {
    FILE * out = fopen("texture1.pgm", "wb");
    fprintf(out, "P5 %d %d 255\n", width+2, height);
    const unsigned char * p = tex + tex_width;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width+2; x++) {
	fputc(*p++, out);
      }
      p += tex_width - (width + 2);
    }
    fclose(out);
  }
#endif
  dilate(tex, tex_width,  1 - tex_width * (height + 2),
	 tex + tex_width, tex_width, 1-tex_width * (height),
	 height, width+2);
#ifdef SAVE
  {
    FILE *out = fopen("texture2.pgm", "wb");
    const unsigned char * p = tex;
    fprintf(out, "P5 %d %d 255\n", width+2, height+2);
    for (int y = 0; y < height+2; y++) {
      for (int x = 0; x < width+2; x++) {
	fputc(*p++, out);
      }
      p += tex_width - (width + 2);
    }
    fclose(out);
  }
#endif
}

static inline void
select_texture(unsigned int tex)
{
  //std::cerr << "Selecting FontFT texture # " << tex << std::endl;
  glBindTexture(GL_TEXTURE_2D, tex);
}

void
FontFTBitmap::create_textures()
{
  if (texture != 0) return;

#ifdef USE_LIST_FOR_BITMAPS
  list_base = glGenLists(256);
#endif
  DBG;
  glGenTextures(1, &texture);
  DBG;
  int max_tex_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
  if (max_tex_size > 1024)
    max_tex_size = 1024;
  chose_best_tex_size(char_width, char_height, max_tex_size,
		      tex_width, tex_height);
  int chars_per_line = tex_width / char_width;
  unsigned char * tex = new unsigned char[tex_width*tex_height];
  memset(tex, 0, tex_width*tex_height);
  int n;
  for (n = 0; n < 256; n++) {
#ifdef USE_LIST_FOR_BITMAPS
    glNewList(list_base+n, GL_COMPILE);
#endif
    fill_texture(tex,
		 tex_width, tex_height,
		 (n % chars_per_line) * char_width,
		 (n / chars_per_line) * char_height,
		 info[n]);
#ifdef USE_LIST_FOR_BITMAPS
    glEndList();
#endif
  }

  select_texture(texture);
  DBG;
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, tex_width, tex_height, 0,
	       GL_ALPHA, GL_UNSIGNED_BYTE, tex);
  DBG;
  delete[] tex;

  glGenTextures(1, &gtexture);
  DBG;
  chose_best_tex_size(char_width+2, char_height+2, max_tex_size,
		      gtex_width, gtex_height);
  chars_per_line = gtex_width / (char_width+2);
  tex = new unsigned char[gtex_width*gtex_height];
  memset(tex, 0, gtex_width*gtex_height);
  for (n = 0; n < 256; n++) {
    dilate_and_fill_texture(tex,
			    gtex_width, gtex_height,
			    (n % chars_per_line) * (char_width+2),
			    (n / chars_per_line) * (char_height+2),
			    info[n]);
  }

  select_texture(gtexture);
  DBG;
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, gtex_width, gtex_height, 0,
	       GL_ALPHA, GL_UNSIGNED_BYTE, tex);
  DBG;

  delete[] tex;
}

FontFTBitmap::FontFTBitmap(void * t,
			   const string& name, Font::Style style, float sz)
  : FontFTOutline(name, style, sz),
    texture(0), tex_width(0), tex_height(0),
    gtexture(0), gtex_width(0), gtex_height(0),
    char_width(0), char_height(0)
{
  FT_Face face = static_cast<FT_Face>(t);
  ascent = face->size->metrics.ascender/64.0f;
  descent = -face->size->metrics.descender/64.0f;
  FT_Glyph glyph;
  FT_BBox bbox;
  int n;

  for (n = 0; n < 256; n++) {
    int error = FT_Load_Char(face, n, FT_LOAD_RENDER);
    if (error) {
      widths[n] = 0;
      info[n].pixmap = 0;
      info[n].xmax = 0;
      info[n].xmin = 0;
      info[n].ymax = 0;
      info[n].ymin = 0;
      continue;
    }
    FT_Get_Glyph(face->glyph, &glyph);
    FT_Glyph_Get_CBox(glyph, 0, &bbox );
    info[n].xmax = CEILING(bbox.xMax) >> 6;
    info[n].xmin = FLOOR(bbox.xMin) >> 6;
    info[n].ymax = CEILING(bbox.yMax) >> 6;
    info[n].ymin = FLOOR(bbox.yMin) >> 6;
    char_width = std::max(char_width, info[n].xmax - info[n].xmin);
    char_height = std::max(char_height, info[n].ymax - info[n].ymin);
    widths[n] = info[n].advance = face->glyph->metrics.horiAdvance >> 6;
    int l;
    if (face->glyph->bitmap.pixel_mode == ft_pixel_mode_mono) {
      is_bitmap = true;
      l = face->glyph->bitmap.width * face->glyph->bitmap.rows / 8;
    }
    else {
      is_bitmap = false;
      l = face->glyph->bitmap.width * face->glyph->bitmap.rows;
    }
    info[n].xbearing = face->glyph->metrics.horiBearingX >> 6;
    info[n].ybearing = face->glyph->metrics.horiBearingY >> 6;

    info[n].pixmap = new unsigned char[l];
    memcpy(info[n].pixmap, face->glyph->bitmap.buffer, l);
  }
}

Font::Format
FontFTBitmap::getFormat() const
{
  return format_bitmap;
}

void
FontFTBitmap::paint(const string& str, float x, float y)
{
#ifndef USE_PIXMAP
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
  create_textures();
  DBG;
  select_texture(texture);
  DBG;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  int chars_per_line = tex_width / char_width;
  glBegin(GL_QUADS);
  for (string::const_iterator i = str.begin(); i != str.end(); i++) {
    int n = (unsigned char)(*i);
    const CharInfo& inf = info[n];
    int from_x = (n % chars_per_line) * char_width;
    int from_y = (n / chars_per_line) * char_height;
    int to_x = from_x + inf.xmax - inf.xmin;
    int to_y = from_y + inf.ymax - inf.ymin;
    
    glTexCoord2f(float(from_x) / tex_width, float(to_y) / tex_height);
    glVertex2f(inf.xmin+x, inf.ymin+y);
    glTexCoord2f(float(from_x) / tex_width, float(from_y) / tex_height);
    glVertex2f(inf.xmin+x, inf.ymax+y);
    glTexCoord2f(float(to_x) / tex_width, float(from_y) / tex_height);
    glVertex2f(inf.xmax+x, inf.ymax+y);
    glTexCoord2f(float(to_x) / tex_width, float(to_y) / tex_height);
    glVertex2f(inf.xmax+x, inf.ymin+y);
    x += inf.advance;
  }
  glEnd();
  glPopAttrib();
#else // USE_PIXMAP
  glRasterPos2i(x, y);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (string::const_iterator i = str.begin(); i != str.end(); i++) {
    unsigned int n = *i;
    if (is_bitmap) {
      glBitmap(info[n].xmax-info[n].xmin,
	       info[n].ymax-info[n].ymin,
	       info[n].xbearing, info[n].ybearing,
	       info[n].advance, 0, info[n].pixmap);
    }
    else {
      glRasterPos2i(x + info[n].xmin, y + info[n].ymin);
      glDrawPixels(info[n].xmax-info[n].xmin,
		 info[n].ymax-info[n].ymin,
		 GL_LUMINANCE, GL_UNSIGNED_BYTE, info[n].pixmap);
      x += info[n].advance;
      glRasterPos2i(x, y);
    }
  }
#endif // USE_PIXMAP
}

void
FontFTBitmap::paintGrown(const string& str, float x, float y)
{
  const float dx = 1, dy = 1;
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
  create_textures();
  DBG;
  select_texture(gtexture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  DBG;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  int chars_per_line = gtex_width / (char_width+2);
  glBegin(GL_QUADS);
  for (string::const_iterator i = str.begin(); i != str.end(); i++) {
    int n = (unsigned char)(*i);
    const CharInfo& inf = info[n];
    int from_x = (n % chars_per_line) * (char_width+2);
    int from_y = (n / chars_per_line) * (char_height+2);
    int to_x = from_x + inf.xmax - inf.xmin + 2;
    int to_y = from_y + inf.ymax - inf.ymin + 2;
    
    glTexCoord2f(float(from_x) / gtex_width, float(to_y) / gtex_height);
    glVertex2f(inf.xmin+x-dx, inf.ymin+y-dy);
    glTexCoord2f(float(from_x) / gtex_width, float(from_y) / gtex_height);
    glVertex2f(inf.xmin+x-dx, inf.ymax+y+dy);
    glTexCoord2f(float(to_x) / gtex_width, float(from_y) / gtex_height);
    glVertex2f(inf.xmax+x+dx, inf.ymax+y+dy);
    glTexCoord2f(float(to_x) / gtex_width, float(to_y) / gtex_height);
    glVertex2f(inf.xmax+x+dx, inf.ymin+y-dy);
    x += inf.advance;
  }
  glEnd();
  glPopAttrib();
}



void
installFontFT()
{
  if (font_ft_creator != 0)
    return;
  int error = FT_Init_FreeType( &_FT_Library );
  if (error)
    return;
  font_ft_creator = new FontFTCreator();
}

typedef Font::FontInfo FTFontInfo;

struct CachedValue {
  string filename;
  FT_Face face;
  FontFTBitmap * bm_font;
  FontFTOutline * ol_font;

  CachedValue() : face(0), bm_font(0), ol_font(0) {}
  CachedValue(const string& fn)
    : filename(fn), face(0), bm_font(0), ol_font(0)
  { }
  
};


typedef std::map<FTFontInfo,CachedValue> FontMap;
static FontMap font_map;

FontFTCreator::FontFTCreator()
{
  using namespace boost::filesystem;
#ifdef PRINT
  std::cout << "Starting scanning of font dir\n";
#endif
  // Probably should try to look at an environment variable
  // for a local font path
  for (int d = 0; font_dirs[d] != NULL; d++) {
    const char * font_dir = font_dirs[d];
    for (dir_it it(font_dir); it != dir_it(); ++it) {
      if (boost::filesystem::get<is_directory>(it))
	continue;
      string fname = string(font_dir)+"/"+*it;
      FT_Face face;
      int res = FT_New_Face(_FT_Library, fname.c_str(), 0, &face);
      if (res == 0) {
	FTFontInfo fi(face->family_name,
		      (face->style_flags & FT_STYLE_FLAG_BOLD) ?
		      ((face->style_flags & FT_STYLE_FLAG_ITALIC) ?
		       Font::style_bold_italic : Font::style_bold)
		      : (face->style_flags & FT_STYLE_FLAG_ITALIC) ?
		      Font::style_italic : Font::style_plain,
		      FT_IS_SCALABLE(face) ?
		      Font::format_outline : Font::format_bitmap,
		      0);
#ifdef PRINT
	std::cout << " File: " << fname << std::endl;
	std::cout << "	Name: " << face->family_name
		  << " " << face->style_name << std::endl;
	std::cout << "	Style: ";
	switch (fi.style) {
	case Font::style_plain: std::cout << "plain"; break;
	case Font::style_bold: std::cout << "bold"; break;
	case Font::style_italic: std::cout << "italic"; break;
	case Font::style_bold_italic: std::cout << "bold italic"; break;	
	}
	std::cout << std::endl;
	std::cout << "	Format: ";
	switch(fi.format) {
	case Font::format_bitmap: std::cout << "bitmap"; break;
	case Font::format_outline: std::cout << "outline"; break;
	}
	std::cout << std::endl;
	if (FT_IS_SCALABLE(face)) {
	  std::cout << "	Size: 0\n";
	}
#endif
	font_map[fi] = fname;	// add it with 0 size
	FT_Done_Face(face);
      }
    }
  }
}

FontFTCreator::~FontFTCreator()
{
  FT_Done_FreeType(_FT_Library);
  _FT_Library = 0;
  font_ft_creator = 0;
  font_map.clear();		// ++FIXME should free font faces
}

FontFTBitmap *
FontFTCreator::createBitmap(const string& name, Font::Style style, float size)
{
  // Check the exact request first
  FontMap::iterator i;
  i = font_map.find(FTFontInfo(name, style, Font::format_bitmap, size));
  if (i != font_map.end()) {
    return i->second.bm_font;
  }

  // Check bitmap only font then
  i = font_map.find(FTFontInfo(name, style, Font::format_bitmap, 0));
  if (i == font_map.end()) {
    // Check outline then
    i = font_map.find(FTFontInfo(name, style, Font::format_outline, 0));
  }
  if (i == font_map.end())
    return 0;

  if (i->second.face == 0) {
    int res = FT_New_Face(_FT_Library, i->second.filename.c_str(), 0,
			  &(i->second.face));
    if (res != 0) {		// cannot load it! Remove from map
      font_map.erase(i);
      return 0;
    }
  }
  if (FT_Set_Pixel_Sizes(i->second.face, 0, int(size)))
    return 0;
  FontFTBitmap * bm = new FontFTBitmap(i->second.face,
				       name, style, size);
  FT_Done_Face(i->second.face);
  i->second.face = 0;
  if (bm != 0) {
    CachedValue& c =
      font_map[FTFontInfo(name, style, Font::format_bitmap, size)];
    c = i->second;
    c.bm_font = bm;
  }
  
  return bm;
}

FontFTOutline *
FontFTCreator::createOutline(const string& name, Font::Style style)
{
  return 0;
}

Font *
FontFTCreator::create(const string& name, Font::Style style, float size)
{
  if (size != 0)
    return createBitmap(name, style, size);
  else
    return createOutline(name, style);
}

} // namespace infovis

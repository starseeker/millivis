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
#ifndef TREEMAP2_STRUETYPEFONT_HPP
#define TREEMAP2_STRUETYPEFONT_HPP

#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/gl.hpp>
#include "font_backend/struetype.h"
#include "font_backend/fontstash.h"

namespace infovis {

/**
 * Font implementation using struetype and fontstash
 */
class StrueTypeFont : public Font {
public:
  StrueTypeFont(const string& name, Style style, float size);
  virtual ~StrueTypeFont();

  // Font interface implementation
  virtual bool install();
  virtual void finish();
  virtual Format getFormat() const { return format_outline; }
  virtual bool isFixedWidth() const { return false; }
  virtual void paint(const string& str, float x, float y);
  virtual float getLeading() const;
  virtual float getAscent() const;
  virtual float getDescent() const;
  virtual float charWidth(int ch) const;
  virtual float stringWidth(const string& str);
  virtual Box getStringBounds(const string& str);

  // Static initialization
  static void initialize();
  static void cleanup();

private:
  strue_font_t* strue_font_;
  static FONScontext* fons_context_;
  static unsigned int texture_id_;
  static bool initialized_;
  int font_id_;
  bool installed_;

  // OpenGL rendering callbacks
  static int renderCreate(void* uptr, int width, int height);
  static int renderResize(void* uptr, int width, int height);
  static void renderUpdate(void* uptr, int* rect, const unsigned char* data);
  static void renderDraw(void* uptr, const float* verts, const float* tcoords, 
                        const unsigned int* colors, int nverts);
  static void renderDelete(void* uptr);

  void ensureInitialized();
};

/**
 * Font creator for StrueTypeFont
 */
class StrueTypeFontCreator : public Font::Creator {
public:
  StrueTypeFontCreator();
  virtual Font* create(const string& name, Font::Style style, float size);
};

} // namespace infovis

#endif // TREEMAP2_STRUETYPEFONT_HPP
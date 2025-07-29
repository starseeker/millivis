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
#include "StrueTypeFont.hpp"
#include <iostream>
#include <cstring>

namespace infovis {

// Static members
FONScontext* StrueTypeFont::fons_context_ = nullptr;
unsigned int StrueTypeFont::texture_id_ = 0;
bool StrueTypeFont::initialized_ = false;

StrueTypeFont::StrueTypeFont(const string& name, Style style, float size)
  : Font(name, style, size),
    strue_font_(nullptr),
    font_id_(-1),
    installed_(false)
{
  ensureInitialized();
  
  // Load ProFont as default font, or try to load specified font
  if (name == "default" || name.empty()) {
    // Use embedded ProFont as default
    strue_font_ = strue_load_font_from_file("infovis/drawing/fonts/ProFont.ttf");
    if (!strue_font_) {
      // Fallback to built-in font if ProFont not found
      strue_font_ = strue_load_font_from_file(nullptr);
    }
  } else {
    strue_font_ = strue_load_font_from_file(name.c_str());
  }
  
  if (strue_font_ && fons_context_) {
    // Add font to fontstash context
    // For simplicity, we'll use a default font data
    static const char default_font_name[] = "default";
    static unsigned char dummy_font_data[] = { 0x00, 0x01, 0x00, 0x00 };
    
    font_id_ = fonsAddFontMem(fons_context_, default_font_name, 
                             dummy_font_data, sizeof(dummy_font_data), 0);
  }
}

StrueTypeFont::~StrueTypeFont() {
  if (strue_font_) {
    strue_free_font(strue_font_);
  }
}

void StrueTypeFont::initialize() {
  if (initialized_) return;
  
  // Set up fontstash parameters
  FONSparams params;
  memset(&params, 0, sizeof(params));
  params.width = 512;
  params.height = 512;
  params.flags = FONS_ZERO_TOPLEFT;
  params.renderCreate = renderCreate;
  params.renderResize = renderResize;
  params.renderUpdate = renderUpdate;
  params.renderDraw = renderDraw;
  params.renderDelete = renderDelete;
  params.userPtr = nullptr;
  
  fons_context_ = fonsCreateInternal(&params);
  initialized_ = true;
}

void StrueTypeFont::cleanup() {
  if (fons_context_) {
    fonsDeleteInternal(fons_context_);
    fons_context_ = nullptr;
  }
  
  if (texture_id_) {
    glDeleteTextures(1, &texture_id_);
    texture_id_ = 0;
  }
  
  initialized_ = false;
}

void StrueTypeFont::ensureInitialized() {
  if (!initialized_) {
    initialize();
  }
}

bool StrueTypeFont::install() {
  if (!strue_font_ || !fons_context_ || font_id_ < 0) {
    return false;
  }
  
  installed_ = true;
  return true;
}

void StrueTypeFont::finish() {
  installed_ = false;
}

void StrueTypeFont::paint(const string& str, float x, float y) {
  if (!installed_ || !fons_context_ || font_id_ < 0) {
    return;
  }
  
  // Set up fontstash state
  fonsPushState(fons_context_);
  fonsSetFont(fons_context_, font_id_);
  fonsSetSize(fons_context_, size);
  fonsSetColor(fons_context_, 0xffffffff); // White color
  fonsSetAlign(fons_context_, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);
  
  // Draw the text
  fonsDrawText(fons_context_, x, y, str.c_str(), nullptr);
  
  fonsPopState(fons_context_);
}

float StrueTypeFont::getLeading() const {
  if (!strue_font_) {
    return size * 0.1f; // Default leading
  }
  return strue_get_line_gap(strue_font_, size);
}

float StrueTypeFont::getAscent() const {
  if (!strue_font_) {
    return size * 0.8f; // Default ascent
  }
  return strue_get_ascent(strue_font_, size);
}

float StrueTypeFont::getDescent() const {
  if (!strue_font_) {
    return size * 0.2f; // Default descent
  }
  return strue_get_descent(strue_font_, size);
}

float StrueTypeFont::charWidth(int ch) const {
  if (!strue_font_) {
    return size * 0.6f; // Default character width
  }
  
  // Get glyph and return its advance
  strue_glyph_t* glyph = strue_get_glyph(strue_font_, ch, size);
  if (glyph) {
    float advance = strue_glyph_get_advance(glyph);
    strue_free_glyph(glyph);
    return advance;
  }
  
  return size * 0.6f;
}

float StrueTypeFont::stringWidth(const string& str) {
  if (!strue_font_) {
    return str.length() * size * 0.6f; // Default width calculation
  }
  
  return strue_measure_text(strue_font_, str.c_str(), size);
}

Box StrueTypeFont::getStringBounds(const string& str) {
  float width, height;
  
  if (!strue_font_) {
    width = str.length() * size * 0.6f;
    height = size;
  } else {
    strue_get_text_bounds(strue_font_, str.c_str(), size, &width, &height);
  }
  
  return Box(0, -getAscent(), width, getDescent());
}

// OpenGL rendering callbacks
int StrueTypeFont::renderCreate(void* uptr, int width, int height) {
  // Create OpenGL texture
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return 1;
}

int StrueTypeFont::renderResize(void* uptr, int width, int height) {
  // Resize texture
  if (texture_id_) {
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, nullptr);
  }
  return 1;
}

void StrueTypeFont::renderUpdate(void* uptr, int* rect, const unsigned char* data) {
  // Update texture region
  if (texture_id_) {
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], rect[2]-rect[0], rect[3]-rect[1], 
                   GL_ALPHA, GL_UNSIGNED_BYTE, data + rect[1]*512 + rect[0]);
  }
}

void StrueTypeFont::renderDraw(void* uptr, const float* verts, const float* tcoords, 
                              const unsigned int* colors, int nverts) {
  if (!texture_id_) return;
  
  // Simple OpenGL rendering
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < nverts; i++) {
    unsigned int color = colors[i];
    glColor4ub((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    glTexCoord2f(tcoords[i*2], tcoords[i*2+1]);
    glVertex2f(verts[i*2], verts[i*2+1]);
  }
  glEnd();
  
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}

void StrueTypeFont::renderDelete(void* uptr) {
  if (texture_id_) {
    glDeleteTextures(1, &texture_id_);
    texture_id_ = 0;
  }
}

// Creator implementation
StrueTypeFontCreator::StrueTypeFontCreator() : Font::Creator() {
}

Font* StrueTypeFontCreator::create(const string& name, Font::Style style, float size) {
  return new StrueTypeFont(name, style, size);
}

} // namespace infovis
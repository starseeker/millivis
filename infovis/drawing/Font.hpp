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
#ifndef INFOVIS_DRAWING_FONT_HPP
#define INFOVIS_DRAWING_FONT_HPP

#include <infovis/drawing/drawing.hpp>

namespace infovis {

/**
 * Abstract Font type for drawing fonts with OpenGL.
 */
class Font {
public:
  /// Font style
  enum Style {
    style_plain		= 0,	/// Plain Style
    style_bold		= 1,	/// Bold style
    style_italic	= 2,	/// Italic style
    style_bold_italic	= 3	/// Bold and Italic style
  };

  /// Font format
  enum Format {
    format_none,		/// Unknown format
    format_composite,		/// Composite format
    format_bitmap,		/// Bitmap format
    format_outline,		/// Outline format
    format_plotter		/// Plotter format
  };

  /**
   * Information on a font.  Used for indexing in a map.
   */
  struct FontInfo {
    string name;		/// Font name
    Style style;		/// Font style
    Format format;		/// Font format
    float size;			/// Size in pixels
    FontInfo() { }		/// Empty constructor
    FontInfo(const string& n, Style s, Format f, float sz)
      : name(n), style(s), format(f), size(sz)
    { }				/// Full constructor

    /// Equals method
    bool operator == (const FontInfo& info) const {
      return
	name == info.name &&
	style == info.style &&
	format == info.format &&
	size == info.size;
    }

    /// less_than method
    bool operator < (const FontInfo& info) const {
      return 
	name < info.name ||
	(name == info.name &&
	 (style < info.style ||
	  (style == info.style &&
	   (format < info.format ||
	    (format == info.format &&
	     size < info.size)))));
    }
  };
  
  /**
   * Static method for creating a font by name, style and size.
   * @param name the name
   * @param style the style
   * @param size the size
   * @return a font or null
   */
  static Font * create(const string& name = "default",
		       Style style = style_plain, float size = 10);
  /**
   * Static method for creating a font by name, style and size.
   * @param name the name
   * @param style the string name of the style
   * @param size the size
   * @return a font or null
   */
  static Font * create(const string& name,
		       const string& style, float size = 10);
  /**
   * Set the point size of the font.
   * @param ptsz the point size
   */
  static void setPointSize(float ptsz);
  /**
   * Get the point size of the font.
   * @return the point size
   */
  static float getPointSize();

  /**
   * Install the font for drawing.
   *
   * Call that method to install the font in the OpenGL context.
   * @return false if the font cannot be installed.
   */
  virtual bool install() = 0;

  /**
   * Finish using the font in the OpenGL context.
   *
   * Call that method when the font is no longer user so that it can
   * be removed from the OpenGL memory if needed.
   */
  virtual void finish() = 0;

  /**
   * Return the format of the font.
   * @return the format of the font
   */
  virtual Format getFormat() const  = 0;

  /**
   *
   */
  virtual bool isBitmap() const { return getFormat() == format_bitmap; }
  /**
   *
   */
  virtual bool isOutline() const { return getFormat() == format_outline; }
  /**
   *
   */
  virtual bool isFixedWidth() const = 0;
  /**
   *
   */
  virtual void paint(const string& str, float x, float y) = 0;
  /**
   *
   */
  virtual void paintGrown(const string& str, float x, float y);
  /**
   *
   */
  virtual float getLeading() const = 0;
  /**
   *
   */
  virtual float getAscent() const = 0;
  /**
   *
   */
  virtual float getDescent() const = 0;
  /**
   *
   */
  virtual float getHeight() const;
  /**
   *
   */
  virtual float charWidth(int ch) const = 0;
  /**
   *
   */
  virtual float stringWidth(const string& str) = 0;
  /**
   *
   */
  virtual Box getStringBounds(const string& str) = 0;

  /**
   *
   */
  virtual void center(const string& str, float x, float y);
  /**
   *
   */
  virtual void rightJustify(const string& str, float x, float y);

  /**
   * Class for creating fonts.
   */
  class Creator {
  protected:
    static Creator * first;
    Creator * next;

    Creator();
    virtual ~Creator();
    friend class Font;
  public:

  /**
   *
   */
    virtual Font * create(const string& name, Style style, float size) = 0;
  };
protected:

  /**
   *
   */
  Font(const string& name, Style style, float size);
  /**
   *
   */
  virtual ~Font();

  string name;
  Style style;
  float size;
  static float point_size;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_FONT_HPP

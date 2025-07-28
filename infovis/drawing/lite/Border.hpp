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
#ifndef INFOVIS_DRAWING_LITE_BORDER_HPP
#define INFOVIS_DRAWING_LITE_BORDER_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/lite/Lite.hpp>

namespace infovis {

/**
 * Border for frames
 */
class Border {
public:
  enum Style {
    border_normal,
    border_raised,
    border_lower
  };
  Border(float w = 5);
  Border(const Color& inside, float w = 5);
  Border(const Color& inside, const Color& outside, float w = 5);
  virtual ~Border();

  virtual Box growBox(const Box& bounds) const;
  virtual Box shrinkBox(const Box& bounds) const;
  
  virtual float getWidth() const;
  virtual void setWidth(float w);
  
  virtual Style getStyle() const;
  virtual void setStyle(Style v);

  virtual const Color& getInside() const;
  virtual void setInside(const Color& v);

  
  virtual const Color& getOutside() const;
  virtual void setOutside(const Color& v);
  
  virtual void render(const Lite::RenderContext& c,
		      const Box& inBounds);
  
protected:
  
  Style style_;
  float width_;
  Color inside_;
  Color outside_;
};

} // namespace infovis 

#endif // INFOVIS_DRAWING_LITE_BORDER_HPP

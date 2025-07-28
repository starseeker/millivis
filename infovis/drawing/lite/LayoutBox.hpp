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
#ifndef INFOVIS_DRAWING_LITE_LAYOUTBOX_HPP
#define INFOVIS_DRAWING_LITE_LAYOUTBOX_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/lite/Lite.hpp>
#include <infovis/drawing/lite/LayoutStack.hpp>

namespace infovis {

/**
 * Layout Lites in a given direction.
 */
class LayoutBox : public LayoutStack
{
public:
  enum Direction {
    horizontal,
    vertical
  };
  LayoutBox(Direction dir = vertical, float gap = 0);
  ~LayoutBox();

  virtual Layout * clone() const;

  virtual Box doLayoutIn(Lite * container);

  Direction getDirection() const {return dir_; }
  void setDirection(Direction v) {dir_ = v; }
  
protected:
  Direction dir_;
  float gap_;
};

} // namespace infovis 

#endif // INFOVIS_DRAWING_LITE_LAYOUTBOX_HPP

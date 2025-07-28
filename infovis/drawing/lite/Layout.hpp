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
#ifndef INFOVIS_DRAWING_LITE_LAYOUT_HPP
#define INFOVIS_DRAWING_LITE_LAYOUT_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/lite/Lite.hpp>
#include <vector>

namespace infovis {

/**
 * Base class for layout management
 */
class Layout : virtual public gc_cleanup
{
public:
  Layout() {}
  virtual ~Layout();

  virtual Layout * clone() const = 0;
  virtual void setLite(int i, Lite * l) = 0;
  virtual void addLite(Lite * lite) = 0;
  virtual void removeLite(Lite * lite) = 0;
  virtual void insertLite(int i, Lite * l) = 0;
  virtual void eraseLite(int i) = 0;
  virtual Box doLayoutIn(Lite * container) = 0;
};
  

} // namespace infovis 

#endif // INFOVIS_DRAWING_LITE_LAYOUT_HPP

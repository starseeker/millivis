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
#ifndef INFOVIS_DRAWING_LITE_LAYOUTSTACK_HPP
#define INFOVIS_DRAWING_LITE_LAYOUTSTACK_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/lite/Lite.hpp>
#include <infovis/drawing/lite/Layout.hpp>

namespace infovis {

/**
 * Layout object as if stacked.
 */
class LayoutStack : public Layout
{
public:
  LayoutStack();
  ~LayoutStack();

  virtual Layout * clone() const;

  virtual void setLite(int i, Lite * l);
  virtual void addLite(Lite * l);
  virtual void removeLite(Lite * l);
  virtual void insertLite(int i, Lite * l) ;
  virtual void eraseLite(int i);
  virtual Box doLayoutIn(Lite * container);
protected:
  Lite::List lite_;
};

} // namespace infovis 

#endif // INFOVIS_DRAWING_LITE_LAYOUTSTACK_HPP

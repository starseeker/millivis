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
#ifndef INFOVIS_DRAWING_LITE_MENUSIMPLE_HPP
#define INFOVIS_DRAWING_LITE_MENUSIMPLE_HPP

#include <infovis/drawing/lite/LiteMenu.hpp>
#include <vector>
#include <string>

namespace infovis {
class Font;

/**
 * Simple lite menu
 */
class LiteMenuSimple : public LiteMenu
{
public:
  LiteMenuSimple(Font * font = 0,
		 const Color& fg = color_black,
		 const Color& c = color_white,
		 const Color& gfg = color_none);

  Lite * clone() const;

  virtual void addItem(const string& str);
  virtual const string& getItem(int i) const;

  // Trap these functions to update the labels
  virtual void setChild(int index, Lite * l);
  virtual void addChild(Lite *);
  virtual void insertChild(int pos, Lite *);
  virtual void eraseChild(int pos);

protected:
  Lite * createLabel(const string& label);
  Font * font_;
  Color foreground_color_;
  Color grown_color_;
  std::vector<string,gc_alloc<string> > labels_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_MENUSIMPLE_HPP

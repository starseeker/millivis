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
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/Font.hpp>

#include <iostream>

using namespace infovis;

int main(int argc, char * argv[])
{
  LiteWindow::init(argc, argv);
  if (argc < 3) {
    std::cerr << "syntax: " << argv[0] << " Font Text ...\n";
    return 1;
  }

  LiteWindow win(argv[0], Box(0, 0, 800, 600));
  win.setColor(color_red);
  Font * font = Font::create(argv[1], Font::style_plain, 20);
  if (font == 0) {
    std::cerr << "cannot create font " << argv[1] << std::endl;
    return 1;
  }
  string text = argv[2];
  for (int i = 3; i < argc; i++)
    text.append(string(" ")+argv[i]);

  LiteLabel lab(text, LiteLabel::just_left, font,
		color_yellow, color_red, color_black);
  lab.setPosition(Point(10, 10));
  win.addChild(&lab);
  win.run();
  return 0;
}

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
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/Font.hpp>

using namespace infovis;

int main(int argc, char *argv[])
{
  LiteWindow::init(argc, argv);
  LiteWindow win(argv[0], Box(0, 0, 640, 400));

  Font * font;
  if (argc > 1)
    font = Font::create(argv[1], Font::style_plain, 20);
  else
    font = Font::create();
  LiteLabel * lab = new LiteLabel("Label test",
				  LiteLabel::just_left, font,
				  color_red, color_blue);
  lab->setPosition(Point(100, 100));
  win.addChild(lab);
  win.run();
  return 0;
}

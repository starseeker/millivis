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
#include <infovis/drawing/lite/LiteMover.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <GL/glut.h>
#include <iostream>

using namespace infovis;

#if 0
class MyWindow : public LiteWindow
{
public:
  MyWindow(const string& name, const Box& box)
    : LiteWindow(name, box), target(0) { }

  Lite * target;
  Vector delta;


  virtual void doMotion(int x, int y) {
    if (target == 0)
      return;
    target->setPosition(Point(x, y) + delta);
    repaint();
  }
  virtual void doMouse(i button, ButtonState state, int x, int y) {
    //std::cout << "Button\n";
    if (button != button_left)
      return;
    if (state == button_down) {
      target = pick(x, y);
      //std::cout << "Target = " << target << std::endl;
      if (target != 0) {
	Point start(x, y);
	delta = target->getPosition() - start;
      }
    }
    else {
      target = 0;
    }
  }
  Lite * pick(int x, int y) {
    Box box(x-3, y-3, x+3, y+3);
    PathList hit;
    if (LiteWindow::pick(box, hit) != 0) {
      return getPath(hit.back());
    }
    return 0;
  }
};
#endif  

int main(int argc, char *argv[])
{
  LiteWindow::init(argc, argv);
  LiteWindow win("test lite", Box(0, 0, 640, 400));

  LiteLabel * lab = new LiteLabel("Label test");
  lab->setPosition(Point(100, 100));
  win.addChild(new LiteMover(lab, lab));
  win.run();
  return 0;
}

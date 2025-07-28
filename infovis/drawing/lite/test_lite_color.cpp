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
#include <infovis/drawing/lite/LiteSliderExt.hpp>
#include <infovis/drawing/lite/LiteColorChooser.hpp>
#include <infovis/drawing/lite/LiteFrame.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <iostream>

using namespace infovis;

class MyLiteWindow : public LiteWindow
{
  Interactor3States * inter_;
  ColorScheme scheme_;
  LiteColorSlider slider_;
  LiteColorSurface surface_;
  
public:
  MyLiteWindow(const string& name, const Box& box)
    : LiteWindow(name, box),
      inter_(0),
      slider_(&scheme_,
	      Box(100, 100, 120, 200),
	      top_to_bottom, color_red),
      surface_(&scheme_,
	       Box(0, 100, 100, 200))
  {
    addChild(&surface_);
    addChild(&slider_);
  }

#if 0
  virtual void doMouse(Button button, ButtonState state, int x, int y) {
    if (inter_ == 0) {
      PathList pl;
      pick(Box(x-1, y-1, x+1, y+1), pl);
      List l;
      sortPathList(pl, l);
      for (List::reverse_iterator i = l.rbegin(); i != l.rend(); i++) {
	inter_ =
	  dynamic_cast<Interactor3States*>((*i)->interactor("3States",
							    mouse_id));
	if (inter_ != 0) {
	  break;
	}
      }
      if (inter_ == 0)
	inter_ =
	  dynamic_cast<Interactor3States*>(slider_.interactor("3States",
							      mouse_id));
    }
    if (inter_ != 0) {
      if (state == button_down) {
	inter_->start(Point(x, y));
      }
      else {
	inter_->finish(Point(x, y));
	inter_ = 0;
      }
      repaint();
    }
  }
  void doMotion(int x, int y) {
    if (inter_ != 0) {
      inter_->drag(Point(x, y));
      repaint();
    }
  }  
#endif
};

int main(int argc, char *argv[])
{
  LiteWindow::init(argc, argv);
  MyLiteWindow win("test color", Box(0, 0, 640, 400));

  win.run();
  return 0;
}

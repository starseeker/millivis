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
#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/Image.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <infovis/drawing/inter/Interactor3States.hpp>
#include <infovis/drawing/inter/KeyCodes.hpp>
#include <infovis/drawing/inter/KeyboardHandler.hpp>
#include <infovis/drawing/inter/MouseHandler.hpp>
#include <infovis/drawing/inter/MouseCodes.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>

#include <cstdlib>
#include <iostream>

using namespace infovis;

#undef DBG
#ifndef NDEBUG
#define DBG {						\
  int err = glGetError();				\
  if (err)						\
    std::cerr << __FILE__ << "(" << __LINE__ << ") : "	\
	      << "gl error: " << gl::glErrorString(err)	\
	      << std::endl;				\
}
#else
#define DBG
#endif

class FocusImageWindow : public LiteWindow,
	 		 public Interactor3States,
	 		 public MouseHandler,
			 public KeyboardHandler
{
  Image * img_;

public:
  FocusImageWindow(const string& name,
		   const Box& b)
    : LiteWindow(name, b, cap_double | cap_rgb)
  {
    img_ = Image::load(name);
    if (img_ == 0) {
      std::cerr << "Cannot load file " << name << std::endl;
      exit(1);
    }
    
  }

  virtual void doRender(const RenderContext&);

  void doKeyboard(int key, bool down, int x, int y) {
    static bool is_saving = false;

    switch(key) {
    case 27:			// escape
      exit(0);
    }
  }

  virtual void doMouse(int button, bool down, int x, int y) {
  }

 
};

void
FocusImageWindow::doRender(const RenderContext& ctx)
{
}

int main(int argc, char * argv[])
{
  LiteWindow::init(argc, argv);
  const char * toload = 0;
  bool fullscreen = true;
  int width = 720, height = 480;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'f':
	fullscreen = ! fullscreen;
	break;
      case 'w':
	i++;
	if (i < argc) {
	  width = atoi(argv[i]);
	}
	break;
      case 'h':
	i++;
	if (i < argc) {
	  height = atoi(argv[i]);
	}
	break;
      default:
	std::cerr << "syntax: " << argv[0]
		  << "[-f] [-w <width>] [-h height] imagefile\n";
	exit(1);
      }
    }
    else {
      toload = argv[i];
    }
  }

  if (toload == 0) {
    std::cerr << "syntax: " << argv[0]
	      << "[-f] [-w <width>] [-h height] imagefile\n";
    exit(1);
  }

  FocusImageWindow win(toload, Box(0, 0, width, height));

  if (fullscreen)
    win.setFullscreen();
  try {
    win.run();
  }
  catch(...) {
    return 1;
  }
  return 0;
}  

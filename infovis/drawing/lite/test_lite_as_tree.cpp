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
#include <infovis/drawing/lite/Lite_as_tree.hpp>
#include <infovis/drawing/box_io.hpp>
#include <infovis/drawing/point_vec_io.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/tree/visitor.hpp>
#include <iostream>

using namespace infovis;

struct PrintVisitor
{
  int indent;

  PrintVisitor() : indent(0) {}

  static void tab(int i) {
    while (i-- != 0)
      std::cout << ' ';
  }
  void preorder(tree_traits< const Lite*>::node_descriptor n) {
    tab(indent++);
    std::cout << n << n->getPosition() << std::endl;
  }
  void inorder(tree_traits< const Lite*>::node_descriptor n) {
  }
  void postorder(tree_traits< const Lite*>::node_descriptor n) {
    indent--;
  }
};


int main(int argc, char * argv[])
{
  LiteWindow::init(argc, argv);
  LiteWindow win("test_lite_as_tree", Box(0, 0, 800, 600));
  LiteLabel hello("hello");
  hello.setPosition(Point(10, 10));
  LiteLabel world("world");
  world.setPosition(Point(20, 20));
  win.addChild(&hello);
  win.addChild(&world);

  PrintVisitor vis;
  traverse_tree<const Lite *,PrintVisitor>(&win, &win, vis);
  win.run();
  return 0;
}

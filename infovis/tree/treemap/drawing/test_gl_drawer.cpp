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
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#ifdef VECTOR_AS_TREE
#include <infovis/tree/vector_as_tree.hpp>
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#else
#include <infovis/tree/tree.hpp>
#include <infovis/tree/dir_tree.hpp>
#include <infovis/tree/xml_tree.hpp>
#endif
#include <infovis/tree/treemap/slice_and_dice.hpp>
#include <infovis/tree/treemap/squarified.hpp>
#include <infovis/tree/treemap/drawing/gl_drawer.hpp>
#include <infovis/tree/treemap/drawing/debug_drawer.hpp>
#include <iostream>
#include <cmath>

using namespace infovis;

#ifdef VECTOR_AS_TREE
typedef std::vector<float,gc_alloc<float,true> > WeightMap;
typedef vector_as_tree Tree;
Tree t(1);
std::vector<std::string,gc_alloc<std::string> > names;
WeightMap wm;
//WeightMap swm;
#else
typedef FloatColumn WeightMap;
typedef tree Tree;
Tree t;
WeightMap * wm;
//WeightMap * swm;
#endif
int win_width, win_height;

struct compare_weight {
  typedef tree_traits< Tree >::node_descriptor node_descriptor;
  const WeightMap& weight;

  compare_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] < weight[n2];
  }
};

// GLFW globals
static GLFWwindow* window = nullptr;

typedef debug_drawer<Tree, Box, gl_drawer<Tree,Box> > Drawer;

// GLFW callback functions
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  win_width = width;
  win_height = height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
#if 0
  static treemap_slice_and_dice<Tree,Box,WeightMap&, Drawer>
    tmsd(t, *wm);
  tmsd.start();
  tmsd.visit(left_to_right, Box(0, 0, win_width, win_height), root(t));
  tmsd.finish();
#else
  static treemap_squarified<Tree,Box,WeightMap&, Drawer >
    tmsq(t, *wm);
  tmsq.start();
  tmsq.visit(Box(0, 0, win_width, win_height), root(t));
  tmsq.finish();
#endif
  glFlush();
  if (window) {
    glfwSwapBuffers(window);
  }
}

static void init()
{
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, win_width, 0, win_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void reshape(int w, int h)
{
  win_width = w;
  win_height = h;
  init();
}

int main(int argc, char * argv[])
{
  int i;
#ifdef VECTOR_AS_TREE
  if (argc == 1) {
    dir_property_tree(".", t);
  }
  else {
    if (xml_property_tree(argv[1], t) == 1) {
      dir_property_tree(argv[1], t);
    }
  }
#else
  if (argc == 1) {
    dir_tree(".", t);
  }
  else {
    if (xml_tree(argv[1], t) == 1) {
      dir_tree(argv[1], t);
    }
  }
#endif
  wm = 0;
  for (i = 0; i < t.column_count(); i++) {
    wm = FloatColumn::cast(t.get_column(i));
    if (wm != 0)
      break;
  }
  if (wm == 0) {
    std::cerr << "Cannot find a float column\n";
    return 1;
  }
  FloatColumn * lwm = FloatColumn::find("log_weight", t);

  for (i = 0; i < wm->size(); i++) {
    (*lwm)[i] = log((*wm)[i] + 1.0);
  }

  //swm = FloatColumn::find("sum_weight", t);
  sum_weights(t, *lwm);
  sort(t, compare_weight(*lwm));
  wm = lwm;

  win_height = 768;
  win_width = 1024;
  
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  
  // Configure GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  
  // Create window
  window = glfwCreateWindow(win_width, win_height, argv[0], nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glClearColor (1, 1, 0, 1);
  glShadeModel (GL_FLAT);
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, win_width, 0, win_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  init();
  
  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    display();
  }
  
  glfwTerminate();
  return 0;
}

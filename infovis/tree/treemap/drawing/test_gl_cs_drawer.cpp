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
#include <iostream>
#include <infovis/tree/vector_as_tree.hpp>
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/treemap/slice_and_dice.hpp>
#include <infovis/tree/treemap/squarified.hpp>
#include <infovis/tree/treemap/drawing/gl_cs_drawer.hpp>
#include <infovis/tree/treemap/drawing/debug_drawer.hpp>
#include <filesystem>
#include <iostream>
#include <cmath>

using namespace infovis;

// GLFW globals
static GLFWwindow* window = nullptr;

typedef std::vector<float,gc_alloc<float,true> > WeightMap;
typedef vector_as_tree Tree;
Tree t(1);
std::vector<std::string,gc_alloc<std::string> > names;
WeightMap wm;
WeightMap swm;
int win_width, win_height;
bool changed = true;

static void build_tree(tree_traits< vector_as_tree >::node_descriptor parent,
		       vector_as_tree& tree, const std::string& dirname,
		       std::vector<std::string,gc_alloc<std::string> >& names,
		       WeightMap& wm)
{
  // C++17 std::filesystem modernization
  try {
    for (const auto& entry : std::filesystem::directory_iterator(dirname)) {
      const auto filename = entry.path().filename().string();
      if (filename[0] == '.')
        continue;
      
      auto n = add_node(parent, tree);
      names.push_back(filename);
      
      // Get file size, defaulting to 0 for directories or on error
      std::error_code ec;
      auto size = entry.is_regular_file(ec) ? std::filesystem::file_size(entry, ec) : 0;
      wm.push_back(static_cast<float>(ec ? 0 : size));
      
      if (entry.is_directory(ec) && !ec)
        build_tree(n, tree, entry.path().string(), names, wm);
    }
  } catch (const std::filesystem::filesystem_error&) {
    // Directory iteration failed, continue silently
  }
}

struct compare_weight {
  typedef tree_traits< vector_as_tree >::node_descriptor node_descriptor;
  const WeightMap& weight;

  compare_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] < weight[n2];
  }
};

typedef debug_drawer<Tree,Box,gl_cs_drawer<Tree,Box> > Drawer;

// Forward declaration
static void init();

// GLFW callback functions
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  win_width = width;
  win_height = height;
  init();
}

static void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  Drawer drawer(t);
#if 0
  treemap_slice_and_dice<Tree,Box,WeightMap, WeightMap, Drawer&>
    tmsd(t, wm, swm, drawer);
  tmsq.start();
  tmsd.visit(left_to_right, Box(0, 0, win_width, win_height), root(t));
#else
  treemap_squarified<Tree,Box,WeightMap, Drawer& >
    tmsq(t, wm, drawer);
  tmsq.start();
  tmsq.visit(Box(0, 0, win_width, win_height), root(t));
#endif
  tmsq.finish();
  glFlush();
  if (window) {
    glfwSwapBuffers(window);
  }

}

static void init()
{
  glClearColor (1, 1, 0, 1);
  glShadeModel (GL_FLAT);
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, win_width, 0, win_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  changed = true;
}

int main(int argc, char * argv[])
{

  WeightMap lwm;
  lwm.push_back(0);		// weight of root is 0

  if (argc == 1) {
    names.push_back(".");
    build_tree(root(t), t, ".", names, lwm);
  }
  else {
    names.push_back(argv[1]);
    build_tree(root(t), t, argv[1], names, lwm);
  }
  
  wm.resize(lwm.size());
  for (int i = 0; i < wm.size(); i++) {
    wm[i] = log(lwm[i] + 1.0);
  }
  sum_weights(t, wm);
  sort(t, compare_weight(wm));

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

  init();
  
  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    display();
  }
  
  glfwTerminate();
  return 0;
}

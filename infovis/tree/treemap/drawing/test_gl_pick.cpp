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
#include <GL/glut.h>
//#include <GL/glu.h>
#include <infovis/tree/vector_as_tree.hpp>
#include <infovis/tree/treemap/slice_and_dice.hpp>
#include <infovis/tree/treemap/squarified.hpp>
#include <infovis/tree/treemap/drawing/gl_drawer.hpp>
#include <infovis/tree/treemap/drawing/debug_drawer.hpp>
#include <infovis/tree/treemap/drawing/pick_drawer.hpp>
#include <filesystem>
#include <iostream>
#include <cmath>

using namespace infovis;
//#define DOUBLE_BUFFER

typedef vector_as_tree Tree;
typedef tree_traits<Tree>::node_descriptor node_descriptor;
typedef tree_traits<Tree>::children_iterator children_iterator;
Tree t(1);
std::vector<std::string,gc_alloc<std::string> > names;
typedef std::vector<float,gc_alloc<float,true> > WeightMap;

WeightMap wm;

int win_width, win_height;
Box treemap_box;
Box label_box;
node_descriptor labeled_node;

static void build_tree(node_descriptor parent,
		       Tree& tree, const std::string& dirname,
		       std::vector<std::string,gc_alloc<std::string> >& names,
		       std::vector<float,gc_alloc<float,true> >& wm)
{
  namespace fs = std::filesystem;
  std::error_code ec;
  
  for (const auto& entry : fs::directory_iterator(dirname, ec)) {
    if (ec) {
      // Handle error gracefully - skip directory if we can't read it
      continue;
    }
    
    auto filename = entry.path().filename().string();
    if (filename[0] == '.')
      continue;
      
    node_descriptor n = add_node(parent, tree);
    names.push_back(filename);
    
    // Get file size safely
    std::uintmax_t file_size = 0;
    if (entry.is_regular_file(ec) && !ec) {
      file_size = entry.file_size(ec);
    }
    if (ec) {
      file_size = 0; // Default to 0 if we can't get size
    }
    wm.push_back(static_cast<float>(file_size));
    
    if (entry.is_directory(ec) && !ec) {
      build_tree(n, tree, entry.path().string(), names, wm);
    }
  }
}

struct compare_weight {
  const WeightMap& weight;

  compare_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] < weight[n2];
  }
};

typedef debug_drawer<Tree, Box, gl_drawer<Tree,Box> > Drawer;

static void display()
{
#ifdef SLICE_AND_DICE
  static treemap_slice_and_dice<
    Tree,Box,WeightMap, Drawer>
    tmsd(t, wm);
  tmsd.start();
  tmsd.visit(left_to_right, treemap_box, root(t));
  tmsd.finish();
#else
  static treemap_squarified<
    Tree,Box,WeightMap, Drawer >
    tmsq(t, wm);
  tmsq.start();
  tmsq.visit(treemap_box, root(t));
  tmsq.finish();
#endif
  gl::color(1.0, 1.0, 1.0);
  draw_box(label_box);
#ifdef DOUBLE_BUFFER
  glutSwapBuffers();
#else
  glFlush();
#endif

}

static void passive_motion(int x, int y)
{
  y = win_height - y;
  typedef debug_drawer<Tree,Box, pick_drawer<Tree,Box> > Picker;
  Picker hit;
  hit.reset(x, y);
  
#ifdef SLICE_AND_DICE
  static treemap_slice_and_dice<
    Tree,Box,WeightMap,Picker&>
    tm(t, wm, hit);
  tm.start();
  tm.visit(left_to_right, treemap_box, root(t));
  tm.finish();
#else
  static treemap_squarified<
    Tree,Box,WeightMap,Picker&>
    tm(t, wm, hit);
  tm.start();
  tm.visit(treemap_box, root(t));
  tm.finish();
#endif
  if (hit.has_picked()) {
    node_descriptor h = hit.picked_node();
    if (h != labeled_node) {
      labeled_node = h;
      gl::color(1.0, 1.0, 1.0);
      draw_box(label_box);
      gl::color(0.0, 0.0, 0.0);
      gl::raster_pos(5, 5);
      const std::string& name = names[h];
      for (char c : name) {
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
      }
#ifdef DOUBLE_BUFFER
      glDrawBuffer(GL_FRONT);
      gl::raster_pos(xmin(label_box), ymin(label_box));
      glCopyPixels(int(xmin(label_box)), int(ymin(label_box)),
		   int(width(label_box)), int(height(label_box)),
		   GL_COLOR);
      glDrawBuffer(GL_BACK);
#endif
    }
  }
  glFlush();
}

static void init()
{
  treemap_box = Box(0, 20, win_width, win_height);
  label_box = Box(0, 0, win_width, 20);
  glClearColor (1, 1, 0, 1);
  glShadeModel (GL_FLAT);
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, win_width, 0, win_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  labeled_node = root(t);
}

static void reshape(int w, int h)
{
  win_width = w;
  win_height = h;
  init();
}

int main(int argc, char * argv[])
{

  std::vector<float,gc_alloc<float,true> > lwm;
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
  for (size_t i = 0; i < wm.size(); ++i) {
    wm[i] = std::log(lwm[i] + 1.0f);
  }
  sum_weights(t, wm);
  sort(t, compare_weight(wm));

  win_height = 768;
  win_width = 1024;
  glutInit(&argc, argv);
#ifdef DOUBLE_BUFFER
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
#else
  glutInitDisplayMode(GLUT_RGB);
#endif
  glutInitWindowSize (win_width, win_height); 
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[0]);

  init();
  glutDisplayFunc(display);
  glutPassiveMotionFunc(passive_motion);
  glutReshapeFunc(reshape);
  
  glutMainLoop();
  return 0;
}

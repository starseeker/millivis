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
#include <infovis/tree/tree.hpp>
#include <infovis/tree/dir_tree.hpp>
#include <infovis/tree/xml_tree.hpp>
#include <infovis/tree/treemap/slice_and_dice.hpp>
#include <infovis/tree/treemap/squarified.hpp>
#include <infovis/tree/treemap/drawing/gl_drawer.hpp>
#include <infovis/tree/treemap/drawing/debug_drawer.hpp>
#include <boost/directory.h>
#include <iostream>
#include <cmath>

using namespace infovis;

typedef FloatColumn WeightMap;
typedef tree Tree;
Tree t;
WeightMap * wm;
//WeightMap * swm;
static bool double_buffer = true;
static bool send_pointers = true;
static GLint list = 0;
static bool use_list = false;
static bool update_list = true;
static bool use_depth = true;
static bool use_shading = false;
static bool use_fog = false;
static bool use_fullscreen = false;
static bool use_layout = true;
static bool use_texture = false;
static bool split_array = false;

static int win_width, win_height;
static int prev_width, prev_height;

static int state;

struct compare_weight
{
  typedef tree_traits< Tree >::node_descriptor node_descriptor;
  const WeightMap& weight;

  compare_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] < weight[n2];
  }
};

#ifndef NDEBUG
typedef debug_drawer<Tree, Box, gl_drawer<Tree,Box> > Drawer;
#else
typedef gl_drawer<Tree,Box> Drawer;
#endif

struct box_drawer : public gl_drawer<Tree,Box>
{
  bool begin_box(const Box& b,
		 tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
    return true;
  }
  void draw_box(const Box& b,
		tree_traits<Tree>::node_descriptor n,
		unsigned depth) {
    boxes_.push_back(b);
    depths_.push_back(depth);
  }
  const std::vector<Box>& getBoxes() const { return boxes_; }
  const std::vector<unsigned>& getDepths() const { return depths_; }

protected:
  std::vector<Box> boxes_;
  std::vector<unsigned> depths_;
};

std::vector<GLshort> vertices;
std::vector<Color> colors;
std::vector<float> textures;

static void
create_vertices()
{
  treemap_squarified<Tree,Box,WeightMap&, box_drawer >
    tmsq(t, *wm);
  tmsq.start();
  tmsq.visit(Box(0, 0, win_width, win_height), root(t));
  tmsq.finish();
  const std::vector<Box>& boxes = tmsq.drawer_.getBoxes();
  const std::vector<unsigned>& depths =tmsq.drawer_.getDepths();
  const color_drawer<Tree> color;
  vertices.clear();
  vertices.reserve(4 * 2 * boxes.size());

  colors.clear(); 
  colors.reserve(4 * boxes.size());

  textures.clear();
  textures.reserve(4 * boxes.size());

  for (int i = 0; i < boxes.size(); i++) {
    const Box& b = boxes[i];
    int depth = depths[i];
    const Color& c = color(0, depth);
    float tex = (depth % 6) / 7.0f;

    colors.push_back(c);
    vertices.push_back(unsigned(xmin(b)+0.5f));
    vertices.push_back(unsigned(ymin(b)+0.5f));
    if (use_depth) vertices.push_back(unsigned(-depth));
    textures.push_back(unsigned(tex));

    colors.push_back(c);
    vertices.push_back(unsigned(xmax(b)+0.5f));
    vertices.push_back(unsigned(ymin(b)+0.5f));
    if (use_depth) vertices.push_back(unsigned(-depth-1));
    textures.push_back(unsigned(tex));

    colors.push_back(c);
    vertices.push_back(unsigned(xmax(b)+0.5f));
    vertices.push_back(unsigned(ymax(b)+0.5f));
    if (use_depth) vertices.push_back(unsigned(-depth));
    textures.push_back(unsigned(tex));

    colors.push_back(c);
    vertices.push_back(unsigned(xmin(b)+0.5f));
    vertices.push_back(unsigned(ymax(b)+0.5f));
    if (use_depth) vertices.push_back(unsigned(-depth+1));
    textures.push_back(unsigned(tex));
  }
  send_pointers = true;
}

static void
manage_state()
{
  int s;

  if (state > 20) {
    if (state == 21) {
      use_fullscreen = true;
      glDisable(GL_FOG);
      glDisable(GL_TEXTURE_1D);
      glEnableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glutFullScreen();
    }
    s = state - 21;
  }
  else
    s = state;
  if (state == 41)
    exit(0);

  switch(s) {
  case 0:
    use_depth = false;
    create_vertices();
  case 1: case 2:
    double_buffer = true;
    use_shading = false;
    use_fog = false;
    use_layout = true;
    use_texture = false;
    split_array = false;
    break;
  case 3: case 4: case 5:
    use_layout = false;
    break;
  case 6:
    use_depth = true;
    create_vertices();
  case 7: case 8:
    break;
  case 9: case 10: case 11:
    split_array = true;
    break;
  case 12:
    glEnable(GL_FOG);
  case 13: case 14:
    split_array = false;
    use_fog = true;
    break;
  case 15:
    use_fog = false;
    glDisable(GL_FOG);
    glEnable(GL_TEXTURE_1D);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  case 16: case 17:
    use_texture = true;
    break;
  case 18:
    glEnable(GL_FOG);
  case 19: case 20:
    use_fog = true;
    break;
    // need to change shading and full screen
  }
  state++;
  glutPostRedisplay();
}

static void
display()
{
  int time = glutGet(GLUT_ELAPSED_TIME);

  if (vertices.size() == 0)
    create_vertices();
  glClear(GL_COLOR_BUFFER_BIT);
  static treemap_squarified<Tree,Box,WeightMap&, Drawer >
    tmsq(t, *wm);
  if (use_layout) {
    tmsq.start();
    tmsq.visit(Box(0, 0, win_width, win_height), root(t));
    tmsq.finish();
  }
  else {
    if (use_list && update_list) {
      glNewList(list, GL_COMPILE);
    }
    glVertexPointer(use_depth ? 3 : 2, GL_SHORT, 0, &(*vertices.begin()));
    if (use_texture)
      glTexCoordPointer(1, GL_FLOAT, 0, &(*textures.begin()));
    else
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(*colors.begin()));
    if (split_array) {
      for (int i = 0; i < colors.size(); i += 10240) {
	glDrawArrays(GL_QUADS, i, std::min(int(colors.size()), i + 10240) - i);
      }
    }
    else
      glDrawArrays(GL_QUADS, 0, colors.size());
    if (use_list && update_list) {
      glEndList();
      update_list = false;
    }
    if (use_list)
      glCallList(list);
  }
  if (double_buffer)
    glutSwapBuffers();
  else
    glFlush();
  float t = (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0f;
  std::cout << "Time "
	    << t << "s "
	    << (colors.size())/t << " vertices/s "
	    << colors.size() / 4 << " rectangles\n";
  std::cout << "Configuration:\n"
	    << "\t" << "resolution " << win_width << "x" << win_height << "\n"
	    << "\t" << (use_layout ? "layout+drawing" : "drawing") << "\n"
	    << "\t" << (use_fullscreen ? "fullscreen" : "windowed") << "\n"
	    << "\t" << (double_buffer ? "double buffer" : "single buffer") << "\n"
	    << "\t" << (use_depth ? "use depth" : "no depth") << "\n"
	    << "\t" << (use_shading ? "use shading" : "no shading") << "\n"
	    << "\t" << (use_texture ? "use texture" : "no texture") << "\n"
	    << "\t" << (split_array ? "split array" : "no split array") << "\n"
	    << "\t" << (use_fog ? "use fog" : "no fog") << "\n";
  manage_state();
}

static void init()
{
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, win_width, 0, win_height, 0, 20);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void reshape(int w, int h)
{
  win_width = w;
  win_height = h;
  init();
  vertices.clear();
  colors.clear();
  send_pointers = true;
  update_list = true;
}

static void key(unsigned char key, int x, int y)
{
  switch(key) {
  case 'a':
    split_array = ! split_array;
    break;
  case 'f':
    use_fullscreen = ! use_fullscreen;
    if (use_fullscreen) {
      prev_width = win_width;
      prev_height = win_height;
      glutFullScreen();
    }
    else
      glutReshapeWindow(prev_width, prev_height);
    break;
  case 'p':
    send_pointers = true;
    break;
  case 27:
    exit(0);
    break;
  case 't':
    use_texture = ! use_texture;
    if (use_texture) {
      glEnable(GL_TEXTURE_1D);
      glDisableClientState(GL_COLOR_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    }
    else {
      glDisable(GL_TEXTURE_1D);
      glEnableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    break;
  case 'l':
    use_layout = ! use_layout;
    break;
  case '2':
    use_depth = false;
    create_vertices();
    break;
  case '3':
    use_depth = true;
    create_vertices();
    break;
  case 'd':
    double_buffer = !double_buffer;
    if (double_buffer)
      glDrawBuffer(GL_BACK);
    else
      glDrawBuffer(GL_FRONT);
    break;
  case 's':
    use_shading = ! use_shading;
    if (use_shading)
      glShadeModel(GL_SMOOTH);
    else
      glShadeModel(GL_FLAT);
    break;
  case 'o':
    use_fog = ! use_fog;
    if (use_fog)
      glEnable(GL_FOG);
    else
      glDisable(GL_FOG);
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char * argv[])
{
  int i;
  if (argc == 1) {
    dir_tree(".", t);
  }
  else {
    if (xml_tree(argv[1], t) == 1) {
      dir_tree(argv[1], t);
    }
  }
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

  prev_width = win_height = 768;
  prev_height = win_width = 1024;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize (win_width, win_height);
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[0]);
  //glutFullScreen();

  glShadeModel (GL_FLAT);
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, win_width, 0, win_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glClearColor(0, 0, 0, 1);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogi(GL_FOG_START, 0);
  glFogi(GL_FOG_END, 15);
  GLfloat fogColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  glFogfv(GL_FOG_COLOR, fogColor);
  glHint (GL_FOG_HINT, GL_DONT_CARE);
  Color texture[8];
  texture[0] = color_red;
  texture[1] = color_green;
  texture[2] = color_blue;
  texture[3] = color_cyan;
  texture[4] = color_magenta;
  texture[5] = color_yellow;
  texture[6] = color_half_grey;
  texture[7] = color_white;

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
  //glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  list = glGenLists(1);
  init();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  
  glutMainLoop();
  return 0;
}

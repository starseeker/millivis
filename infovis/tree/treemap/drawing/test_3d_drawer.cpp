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
#include <infovis/drawing/drawing.hpp>
#include <infovis/tree/dir_property_tree.hpp>
#include <infovis/tree/xml_property_tree.hpp>
#include <infovis/tree/vector_as_tree.hpp>
#include <infovis/tree/treemap/slice_and_dice.hpp>
#include <infovis/tree/treemap/squarified.hpp>
#include <infovis/tree/treemap/drawing/gl_cs_3d_drawer.hpp>
#include <infovis/tree/treemap/drawing/debug_drawer.hpp>
#include <boost/directory.h>
#include <cmath>
#include <iostream>


//#define USE_CUBES
#define TRIANGLES
#define USE_ARRAYS
//#define PRINT
#ifdef PRINT
#include <infovis/drawing/box_io.hpp>
#endif
using namespace infovis;

typedef std::vector<float,gc_alloc<float,true> > WeightMap;
typedef property_tree Tree;
typedef tree_traits< vector_as_tree >::node_descriptor node_descriptor;
Tree t;
std::vector<std::string,gc_alloc<std::string> > names;
int win_width, win_height;
int moving, startx, starty;
GLuint list;
bool use_list;
static const float DEPTH_INCR = 100;
Tree::prop_id weight_prop, sum_weight_prop;
static float depth_factor;

struct BorderDrawer : public border_drawer<Tree,Box>
{
  const Tree& tree_;
  BorderDrawer(const Tree& tree) : tree_(tree) { }
  bool begin_border(const Box& b, 
		    tree_traits<Tree>::node_descriptor n,
		    unsigned depth) const {
    if (degree(n, tree_) == 0)
      return false;
    else
      return (width(b) > 3 && height(b) > 3);
  }
};

static const int BufSize = 65536;

struct ThreeD_drawer : public null_drawer<Tree,Box>
{
  typedef box_traits<Box>::coord_type Coord;
  struct point {
    Coord x, y, z;
    point() { }
    point(Coord X, Coord Y, Coord Z)
      : x(X), y(Y), z(Z) { }
  };

  ThreeD_drawer(const Tree& tree)
  : border(tree) {
    init();
  }

  void init() {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    current = 0;
  }

  void flush() {
    glDrawArrays(GL_QUADS, 0, current);
    current = 0;
  }
  void start() {
    current = 0;
    glColorPointer(Color::last_channel, type_of(colors[0][0]), 0, colors);
    glVertexPointer(3, type_of(vertices[0].x), 0, vertices);
  }

  void push(const Box& b, unsigned depth, const Color& c) {
    if ((BufSize - current) <= 4) {
      flush();
    }
    vertices[current] = point(xmin(b), ymin(b), depth);
    colors[current] = c;
    current++;
    vertices[current] = point(xmax(b), ymin(b), depth+1);
    colors[current] = c;
    current++;
    vertices[current] = point(xmax(b), ymax(b), depth);
    colors[current] = c;
    current++;
    vertices[current] = point(xmin(b), ymax(b), depth-1);
    colors[current] = c;
    current++;
  }

  bool begin_box(const Box& b,
		 tree_traits<Tree>::node_descriptor n,
		 unsigned depth) {
    return (width(b) > 0 && height(b) > 0);
  }
  void draw_box(const Box& b,
		tree_traits<Tree>::node_descriptor n,
		unsigned depth) {
    push(b, depth, color(n, depth));
  }
  void draw_border(Box& b, 
		   tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    if (border.begin_border(b, n, depth)) {
#if 0
      const Color& c = (degree(n, t) > 0) ? color_white : color_black;
      Box b_box = b;
      if (border.left_border(b_box, n, t, depth)) {
        push(b_box, depth, c);
	b_box = b;
      }
      if (border.top_border(b_box, n, t, depth)) {
        push(b_box, depth, c);
	b_box = b;
      }
      if (border.right_border(b_box, n, t, depth)) {
        push(b_box, depth, c);
	b_box = b;
      }
      if (border.bottom_border(b_box, n, t, depth)) {
        push(b_box, depth, c);
      }
#endif
      border.remaining_box(b, n, depth);
    }
  }
  void remove_border(Box& b, 
		     tree_traits<Tree>::node_descriptor n,
		   unsigned depth) {
    if (border.begin_border(b, n, depth)) {
      border.remaining_box(b, n, depth);
    }
  }
      
  void end_box(const Box& b, 
	       tree_traits<Tree>::node_descriptor n,
	       unsigned depth) { }

  void finish() {
    flush();
  }

  point vertices[BufSize];
  Color colors[BufSize];
  int current;
  color_drawer<Tree> color;
  BorderDrawer border;
};

struct compare_weight {
  const WeightMap& weight;

  compare_weight(const WeightMap& wm)
    : weight(wm) {}

  bool operator()(node_descriptor n1, node_descriptor n2) {
    return weight[n1] < weight[n2];
  }
};

typedef ThreeD_drawer Drawer;
Drawer drawer(t);

static void display()
{
#if 1
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  if (use_list) {
    //std::cerr << "Using list\n";
    glCallList(list);
  }
  else {
    //if (list == 0)
    //list = glGenLists(1);
    //glNewList(list, GL_COMPILE_AND_EXECUTE);
    //std::cerr << "Creating list\n";
    glScalef(1, 1, 1-40*depth_factor);
    drawer.start();
    WeightMap& wm = t.get_prop_numeric(weight_prop);
    WeightMap& swm = t.get_prop_numeric(sum_weight_prop);

#if 0
    treemap_slice_and_dice<Tree,Box,WeightMap&, WeightMap&,Drawer& >
      tmsd(t, wm, swm,drawer);
    tmsd.visit(left_to_right, Box(0, 0, win_width, win_height), root(t));
#else
    treemap_squarified<Tree,Box,WeightMap&, WeightMap&,Drawer& >
      tmsq(t, wm, swm,drawer);
    tmsq.visit(Box(0, 0, win_width, win_height), root(t));
#endif
    drawer.finish();
    //glEndList();
    //use_list = true;
  }
  glPopMatrix();
  glutSwapBuffers();
#endif
}

/* ARGSUSED2 */
static void
mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
  else if (button == GLUT_RIGHT_BUTTON) {
    if (state == GLUT_DOWN) {
      depth_factor = float(win_height - y) / win_height;
    }
    glutPostRedisplay();
  }
}

static void
motion(int x, int y)
{
  if (moving) {
    glTranslatef(win_width/2, win_height/2, 0);
    glRotatef((x - startx), 0.0, 1.0, 0.0);
    glRotatef(y-starty, 1.0, 0.0, 0.0);
    glTranslatef(-win_width/2, -win_height/2, 0);
    startx = x;
    starty = y;
  }
  else {
    depth_factor = float(win_height - y) / win_height;
  }    
  glutPostRedisplay();
}

static void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#if 1
  gluPerspective(45, (float)win_width / (float)win_height,
		 100, win_width+2000);
#else
  glOrtho(0, win_width, 0, win_height, 0, 1000);
#endif
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glShadeModel (GL_SMOOTH);
  glTranslatef(-win_width/2, -win_height/2, -win_width);
  glFogi(GL_FOG_START, win_width);
  glFogi(GL_FOG_END, win_width + 100);
}

static void
special(int key, int x, int y)
{
  switch(key) {
  case GLUT_KEY_PAGE_UP:
    glTranslatef(0, 0, DEPTH_INCR);
    break;
  case GLUT_KEY_PAGE_DOWN:
    glTranslatef(0, 0, -DEPTH_INCR);
    break;
  case GLUT_KEY_HOME:
    init();
    glLoadIdentity();
    glTranslatef(-win_width/2, -win_height/2, -win_width);  
    break;
  case GLUT_KEY_UP:
    glTranslatef(0, DEPTH_INCR, 0);
    break;
  case GLUT_KEY_DOWN:
    glTranslatef(0, -DEPTH_INCR, 0);
    break;
  case GLUT_KEY_LEFT:
    glTranslatef(DEPTH_INCR, 0, 0);
    break;
  case GLUT_KEY_RIGHT:
    glTranslatef(-DEPTH_INCR, 0, 0);
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

static void reshape(int w, int h)
{
  win_width = w;
  win_height = h;
  //use_list = false;
  init();
}

struct log_fn {
  const WeightMap& wm;
  log_fn(const WeightMap& w) : wm(w) { }
  float operator()(int i) const { return log(wm[i]+1.0); }
};

WeightMap& create_sum_weight(Tree& tree, const string& prop)
{
  Tree::prop_id sum_weight = tree.add_property(prop+"_sum", Tree::type_numeric);
  WeightMap& swm = tree.get_prop_numeric(sum_weight);
  sum_weights(tree,
	      tree.get_prop_numeric(prop),
	      swm);
  return swm;
}

int main(int argc, char * argv[])
{

  Tree::prop_id names = t.add_property("name", property_tree::type_string);
  if (argc == 1) {
    std::cout << "Loading current directory\n";
    dir_property_tree(".", t);
  }
  else {
    std::cout << "Trying to load " << argv[1] << " as an xml file\n";
    if (xml_property_tree(argv[1], t) == 1) {
      std::cout << "Not XML, trying as a directory\n";
      dir_property_tree(argv[1], t);
    }
    t.get_prop_string(names)[root(t)] = argv[1];
  }
  string prop;
  Tree::prop_id weight;
  for (Tree::names_iterator n = t.begin_names();
       n != t.end_names(); n++) {
    prop = *n;
    weight = t.get_prop_id(prop);
    if (weight.is_numeric())
      break;
  }
  if (! weight.is_numeric()) {
    exit(1);
  }
  Tree::prop_id log_id =
    t.add_property(prop+"_log", property_tree::type_numeric);
  WeightMap& sw = t.get_prop_numeric(log_id);
  props_fill(sw, log_fn(t.get_prop_numeric(weight)));

  WeightMap& swm = create_sum_weight(t, prop+"_log");
  WeightMap& swm2 = create_sum_weight(t, prop);
  weight_prop = t.get_prop_id(prop+"_log");
  sum_weight_prop = t.get_prop_id(prop+"_log_sum");

  win_height = 600;
  win_width = 800;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize (win_width, win_height); 
  glutInitWindowPosition (0, 0);
  glutCreateWindow ("test 3d");
  //glutFullScreen();

  glShadeModel (GL_SMOOTH);
  glClearColor (0.0, 0.0, 0.0, 1);
  glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0};
  glFogfv(GL_FOG_COLOR, fogColor);
  glHint (GL_FOG_HINT, GL_DONT_CARE);
  glFogi(GL_FOG_START, win_width);
  glFogi(GL_FOG_END, 2000);

  glClearDepth(3000);

  init();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutSpecialFunc(special);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glutMainLoop();
  return 0;
}

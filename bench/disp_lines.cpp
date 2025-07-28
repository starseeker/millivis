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
#include <infovis/drawing/gl_support.hpp>
#include <infovis/table/csv_loader.hpp>
#include <infovis/drawing/drawing.hpp>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>


#undef min
#undef max

using namespace infovis;

table t;

static int win_width, win_height;
static int prev_width, prev_height;

static Box bounds;
static float * values;


static void
update_bounds()
{
  bounds = Box(0, 10000,
	       -10000, -10000);

  for (int i = 0; i < t.column_count(); i++) {
    FloatColumn * col = FloatColumn::cast(t.get_column(i));
    if (col == 0 || col->get_name()[0] == '$')
      continue;
    set_xmax(bounds, std::max(float(col->size()), xmax(bounds)));
    set_ymin(bounds, std::min(col->min(), ymin(bounds)));
    set_ymax(bounds, std::max(col->max(), ymax(bounds)));
  }
  values = new float [2 * int(xmax(bounds))];
}

static void
display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  const int n = int(xmax(bounds));

  int time = glutGet(GLUT_ELAPSED_TIME);

  glPushMatrix();
  glScalef(win_width / width(bounds), win_height / height(bounds), 1);
  glTranslatef(xmin(bounds), ymin(bounds), 0);

  for (int i = 0; i < t.column_count(); i++) {
    FloatColumn * col = FloatColumn::cast(t.get_column(i));
    if (col == 0 || col->get_name()[0] == '$')
      continue;
    for (int j = 0; j < n; j ++) {
      values[2*j] = j;
      values[2*j+1] = col->fast_get(j);
    }
    glVertexPointer(2, GL_FLOAT, 0, values);
    glDrawArrays(GL_LINE_STRIP, 0, n);
  }
  glPopMatrix();
  glutSwapBuffers();
  float t = (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0f;
  std::cout << "Time "
	    << t << "s ";
}

static void init()
{
  glViewport (0, 0, win_width, win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, win_width, 0, win_height, 0, 20);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  set_color(color_white);
}

static void reshape(int w, int h)
{
  win_width = w;
  win_height = h;
  init();
}

int main(int argc, char * argv[])
{
  if (argc != 2 || ! tqd_table(argv[1], t)) {
    std::cerr << "synatx: " << argv[0] << " <valid tqd file>\n";
    return 1;
  }
  update_bounds();
  prev_width = win_height = 768;
  prev_height = win_width = 1024;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize (win_width, win_height);
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[0]);
  glShadeModel (GL_FLAT);
  init();
  glEnableClientState(GL_VERTEX_ARRAY);
  glClearColor(0, 0, 0, 1);
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
  
      
    

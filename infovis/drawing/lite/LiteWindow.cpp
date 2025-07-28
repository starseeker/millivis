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
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/inter/KeyCodes.hpp>
#include <infovis/drawing/inter/Manager3State.hpp>
#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/ImagePNG.hpp>
#include <infovis/drawing/inter/MouseHandler.hpp>
#include <infovis/drawing/inter/KeyboardHandler.hpp>
#include <infovis/drawing/inter/TimerHandler.hpp>
#include <infovis/drawing/inter/SysHandlers.hpp>
#include <infovis/drawing/inter/DefaultManagerIdle.hpp>
#include <GL/glut.h>
#include <set>
//#define PRINT
//#ifdef PRINT
#include <iostream>
#include <cstdio>
//#endif

#undef DBG
#ifndef NDEBUG
#include <infovis/drawing/gl.hpp>
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

namespace infovis {

LiteWindow * LiteWindow::instance;
static string save_fname;
static int save_frame;
static float time_divisor = 1.0f;
static int time_shift = 0;
static int cursor_x, cursor_y;

void
LiteWindow::init(int& argc, char **argv)
{
  static bool inited;
  if (! inited) {
    glutInit(&argc, argv);
    inited = true;
  }
}

LiteWindow::LiteWindow(const string& name, const Box& box, unsigned int cap)
  : LiteGroup(Box(0, 0, width(box), height(box))),
    near_plane(0),
    far_plane(20),
    clear_mask(buffer_color),
    cursor_(0),
    redisplay_count_(0),
    modifiers_(0)
{
  glutInitDisplayMode(cap);
  glutInitWindowPosition(int(xmin(box)), int(ymin(box)));
  glutInitWindowSize(int(width(box)), int(height(box)));
  win = glutCreateWindow(name.c_str());
  instance = this;
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardup);
  glutSpecialFunc(special);
  glutSpecialUpFunc(specialup);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(passive_motion);
  glutVisibilityFunc(visibility);
  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glutIgnoreKeyRepeat(1);
  // Don't know how to set that
  Font::setPointSize(LiteWindow::screenResolutionPT());
  
  glClearColor (0, 0, 0, 1);
  glShadeModel (GL_FLAT);
  SystemMouseHandler * mouse = new SystemMouseHandler();
  mouse->connect(this);

  SystemEnterLeaveHandler * enterLeave = new SystemEnterLeaveHandler();
  enterLeave->connect(this);

  DefaultManagerIdle * idle = new DefaultManagerIdle();
  idle->setInteractor3States(mouse);
  idle->connect(this);

  DBG;
}

LiteWindow::~LiteWindow()
{
  glutDestroyWindow(win);
  if (instance == this)
    instance = 0;
}

void
LiteWindow::beginPick(const Box&  box,
		      unsigned int * hitBuffer, unsigned sz)
{
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glSelectBuffer(sz, hitBuffer);
  glRenderMode(GL_SELECT);
  glInitNames();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  Point cntr(center(box));
  gluPickMatrix(x(cntr), y(cntr), width(box), height(box), viewport);
  glOrtho(0, width(bounds), 0, height(bounds), near_plane, far_plane);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  DBG;
}

int
LiteWindow::endPick(PathList& hit,
		    unsigned int * hitBuffer, unsigned sz)
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  GLint hits = glRenderMode(GL_RENDER);
  if (hits == -1)
    hits = sz / sizeof(unsigned int);
  DBG;
  hit.resize(hits);
  GLuint * ptr = hitBuffer;
  GLuint * end = hitBuffer + sz;
  int i;
  for (i = 0; i < hits && ptr != end; i++) {
    int names = *ptr++;
    int z1 = *ptr++;
    int z2 = *ptr++;
    hit[i].resize(names);
#ifdef PRINT
    std::cerr << "Found " << names
	      << "[" << z1 << "," << z2 << "] ";
#endif
    for (int j = 0; j < names; j++) {
#ifdef PRINT
      std::cerr << *ptr << ' ';
#endif
      hit[i][j] = *ptr++;
    }
#ifdef PRINT
      std::cerr << std::endl;
#endif
  }
  hit.resize(i);
  return hits;
}

int
LiteWindow::pick(const Box&  box, PathList& hit)
{
  GLuint hitBuffer[1024];// assumes sizeof(Lite*) == sizeof(unsigned int)
  beginPick(box, hitBuffer, 1024);

  RenderContext c(true);
  glPushName(0);
  render(c);

  return endPick(hit, hitBuffer, 1024);
}

void
LiteWindow::pick(const Box& b, List& sorted)
{
  static long cache_redisplay_count = 0;
  static List cache_sorted;
  static Box cache_box;

  if (redisplay_count_ == cache_redisplay_count &&
      b == cache_box) {
    sorted = cache_sorted;
  }
  else {
    PathList hit;
    pick(b, hit);
    sortPathList(hit, sorted);
    cache_sorted = sorted;
    cache_box = b;
  }
}

void
LiteWindow::sortPathList(const PathList& hit, List& sorted)
{
  std::set<Lite*> s;
  Lite * l = this;
  sorted.clear();
  s.insert(l);
  sorted.push_back(this);
  for (PathList::const_iterator path = hit.begin();
       path != hit.end(); path++) {
    Path::const_iterator p = path->begin();
    l = this;
    for (p++; p != path->end(); p++) {
      l = l->getChild(*p);
      if (s.find(l) == s.end())
	sorted.push_back(l);
    }
  }
}


void
LiteWindow::display()
{
#ifdef PRINT
  std::cerr << "display " << std::endl;
#endif
  if (instance == 0)
    return;
  unsigned int mask = 0;
  if ((instance->clear_mask & int(buffer_color)) != 0)
    mask |= GL_COLOR_BUFFER_BIT;
  if ((instance->clear_mask & int(buffer_depth)) != 0)
    mask |= GL_DEPTH_BUFFER_BIT;
  if ((instance->clear_mask & int(buffer_accum)) != 0)
    mask |= GL_ACCUM_BUFFER_BIT;
  if ((instance->clear_mask & int(buffer_stencil)) != 0)
    mask |= GL_STENCIL_BUFFER_BIT;
  
  glClear(mask);
  instance->redisplay_count_++;
  instance->render(RenderContext());
  if (instance->cursor_ != 0) {
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    instance->cursor_->render(cursor_x,
			      cursor_y-instance->cursor_->getHeight());
    glPopAttrib();
  }
  DBG;

  if (! save_fname.empty()) {
    char buffer[256];
    int now = time();
    sprintf(buffer, "%s%04d.png", save_fname.c_str(), save_frame++);
    Box bounds = instance->getBounds();

    Image * img = Image::read(0, 0,
			      unsigned(width(bounds)),
			      unsigned(height(bounds)));
    ImagePNG::Loader::save(buffer, img);
    delete img;
    time_shift += time() - now;
  }
  glutSwapBuffers();
}

void
LiteWindow::doReshape(int w, int h)
{
}

void
LiteWindow::do_reshape(int w, int h)
{
#ifdef PRINT
  std::cerr << "do_reshape " << std::endl;
#endif
  set_width(bounds, Coord(w));
  set_height(bounds, Coord(h));
#ifdef PRINT
  std::cerr << "size set, resizing "
	    << " width=" << int(width(bounds))
	    << " height=" << int(height(bounds))
	    << std::endl;
#endif
  glViewport (0, 0, int(width(bounds)), int(height(bounds)));
#ifdef PRINT
  std::cerr << "resized " << std::endl;
#endif
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#if 1
  glOrtho(0, width(bounds), 0, height(bounds), near_plane, far_plane);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
#else
  gluPerspective(45, width(bounds) / height(bounds),
		 100, 2*width(bounds)+100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(-width(bounds)/2, -height(bounds)/2, -width(bounds));  
#endif
#ifdef PRINT
  std::cerr << "Ortho installed " << std::endl;
#endif
  doReshape(w, h);
  DBG;
}

void
LiteWindow::reshape(int w, int h)
{
  if (instance == 0)
    return;
  instance->do_reshape(w, h);
}

void
LiteWindow::modifiers(int x, int y)
{
  int mods = glutGetModifiers();
  if (mods == modifiers_)
    return;
#ifdef PRINT
  std::cout << "Modifiers = " << mods << std::endl;
#endif
  if ((modifiers_&GLUT_ACTIVE_SHIFT) != (mods&GLUT_ACTIVE_SHIFT)) {
    doKeyboard(keycode_shift,
	       (mods&GLUT_ACTIVE_SHIFT)!=0,
	       x, y);
  }
  if ((modifiers_&GLUT_ACTIVE_CTRL) != (mods&GLUT_ACTIVE_CTRL)) {
    doKeyboard(keycode_ctrl,
	       (mods&GLUT_ACTIVE_CTRL)!=0,
	       x, y);
  }
  if ((modifiers_&GLUT_ACTIVE_ALT) != (mods&GLUT_ACTIVE_ALT)) {
    doKeyboard(keycode_alt,
	       (mods&GLUT_ACTIVE_ALT)!=0,
	       x, y);
  }
  modifiers_ = mods;
}

void 
LiteWindow::doKeyboard(int key, bool down, int x, int y)
{
  fireKeyboardHandler(key, down);
}

void 
LiteWindow::keyboard(unsigned char key, int x, int y)
{
  if (instance == 0)
    return;
  
  instance->modifiers(x, y);
  instance->doKeyboard(key, true, x, int(height(instance->bounds)-y));
}

void 
LiteWindow::keyboardup(unsigned char key, int x, int y)
{
  if (instance == 0)
    return;
  
  instance->modifiers(x, y);
  instance->doKeyboard(key, false, x, int(height(instance->bounds)-y));
}

static int
glut_to_keycode(int key)
{
  switch(key) {
  case GLUT_KEY_F1: key = keycode_f1; break;
  case GLUT_KEY_F2: key = keycode_f2; break;
  case GLUT_KEY_F3: key = keycode_f3; break;
  case GLUT_KEY_F4: key = keycode_f4; break;
  case GLUT_KEY_F5: key = keycode_f5; break;
  case GLUT_KEY_F6: key = keycode_f6; break;
  case GLUT_KEY_F7: key = keycode_f7; break;
  case GLUT_KEY_F8: key = keycode_f8; break;
  case GLUT_KEY_F9: key = keycode_f9; break;
  case GLUT_KEY_F10: key = keycode_f10; break;
  case GLUT_KEY_F11: key = keycode_f11; break;
  case GLUT_KEY_F12: key = keycode_f12; break;
  case GLUT_KEY_LEFT: key = keycode_left; break;
  case GLUT_KEY_UP: key = keycode_up; break;
  case GLUT_KEY_RIGHT: key = keycode_right; break;
  case GLUT_KEY_DOWN: key = keycode_down; break;
  case GLUT_KEY_PAGE_UP: key = keycode_up; break;
  case GLUT_KEY_PAGE_DOWN: key = keycode_down; break;
  case GLUT_KEY_HOME: key = keycode_home; break;
  case GLUT_KEY_END: key = keycode_end; break;
  case GLUT_KEY_INSERT: key = keycode_insert; break;
  default:
    key = keycode_none;
  }
  return key;
}

void
LiteWindow::special(int key, int x, int y)
{
  if (instance == 0)
    return;
  
  instance->modifiers(x, y);
  instance->doKeyboard(glut_to_keycode(key),
		       true, x, int(height(instance->bounds)-y));
}

void
LiteWindow::specialup(int key, int x, int y)
{
  if (instance == 0)
    return;
  
  instance->modifiers(x, y);
  instance->doKeyboard(glut_to_keycode(key), 
		       false, x, int(height(instance->bounds)-y));
}


void 
LiteWindow::doMouse(int button, bool down, int x, int y)
{
  fireMouseHandlerButton(int(button), down, x, y);
}

void 
LiteWindow::mouse(int button, int state, int x, int y)
{
  cursor_x = x;
  cursor_y = int(height(instance->bounds)-y);
  if (instance == 0)
    return;
  instance->modifiers(x, y);
#ifdef PRINT
  std::cout << "Mouse\n";
#endif
  instance->doMouse(button,
		    state == GLUT_DOWN,
		    x, cursor_y);
}

void 
LiteWindow::doMotion(int x, int y)
{
  fireMouseHandlerMotion(x, y);
}

void 
LiteWindow::motion(int x, int y)
{
  cursor_x = x;
  cursor_y = int(height(instance->bounds)-y);
  if (instance == 0)
    return;
  instance->modifiers(x, y);
  instance->doMotion(x, cursor_y);
}

void 
LiteWindow::doPassiveMotion(int x, int y)
{
  fireMouseHandlerMotion(x, y);
}

void 
LiteWindow::passive_motion(int x, int y)
{
  cursor_x = x;
  cursor_y = int(height(instance->bounds)-y);
  if (instance == 0)
    return;
  instance->modifiers(x, y);
  instance->doPassiveMotion(x, cursor_y);
}

void 
LiteWindow::doTimer(int value)
{
  fireTimerHandler(value);
}

void 
LiteWindow::timer(int value)
{
  if (instance == 0)
    return;
  instance->doTimer(value);
}

void
LiteWindow::doVisible(bool visible)
{
  setVisible(visible);
  //std::cerr << "Visibility = " << visible << std::endl;
}

void
LiteWindow::visibility(int state)
{
  if (instance == 0)
    return;
  instance->doVisible(state == GLUT_VISIBLE);
}

void
LiteWindow::run()
{
  glutMainLoop();
}

void
LiteWindow::setName(const string& n)
{
  name = n;
  glutSetWindowTitle(name.c_str());  
}

void
LiteWindow::setColor(const Color& c)
{
  color = c;
  glClearColor(c[Color::red], c[Color::green], c[Color::blue],
	       c[Color::alpha]);
}

void
LiteWindow::setFullscreen(bool b)
{
  glutFullScreen();
}

void
LiteWindow::setClear(Buffer buf, bool t)
{
  if (t)
    clear_mask |= int(buf);
  else
    clear_mask &= (~int(buf));
}

bool
LiteWindow::getClear(Buffer b)
{
  return (clear_mask & int(b)) != 0;
}

LiteWindow::Cursor
LiteWindow::getCursor() const
{
  return Cursor(glutGet(GLUT_WINDOW_CURSOR));
}

static int glut_cursor[] = {
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
  13,
  14,
  15,
  16,
  17,
  18,
  19,
  100,
  101,
  102
};

void
LiteWindow::setCursor(Cursor c)
{
  glutSetCursor(glut_cursor[int(c)]);
}

void
LiteWindow::setCursor(Image * c)
{
  if (c != 0) {
    setCursor(cursor_none);
  }
  cursor_ = c;
}

int
LiteWindow::time()
{
  return int((glutGet(GLUT_ELAPSED_TIME) - time_shift) / time_divisor);
}

int
LiteWindow::screenWidth()
{
  return glutGet(GLUT_SCREEN_WIDTH);
}

int 
LiteWindow::screenHeight()
{
  return glutGet(GLUT_SCREEN_HEIGHT);
}

int 
LiteWindow::screenWidthMM()
{
  return glutGet(GLUT_SCREEN_WIDTH_MM);
}

int 
LiteWindow::screenHeightMM()
{
  return glutGet(GLUT_SCREEN_HEIGHT_MM);
}

float
LiteWindow::screenResolutionMM()
{
  return screenWidth() / screenWidthMM();
}

float
LiteWindow::screenResolutionDPI()
{
  return 25.4f * screenResolutionMM();
}

float 
LiteWindow::screenResolutionPT()
{
  return screenResolutionDPI() / 72;
}

string
LiteWindow::getVendor()
{
  return string(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
}

string
LiteWindow::getRenderer()
{
  return string(reinterpret_cast<const char *>(glGetString(GL_RENDER)));
}

string
LiteWindow::getVersion()
{
  return string(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}  

string
LiteWindow::getExtensions()
{
  return string(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));
}

void
LiteWindow::beginSave(const string& fname)
{
  save_fname = fname;
}

void
LiteWindow::endSave()
{
  save_fname = "";
}

void
LiteWindow::setTimeDivisor(float d)
{
  if (d != 0)
    time_divisor = d;
}

float
LiteWindow::getTimeDivisor()
{
  return time_divisor;
}

void
LiteWindow::addMouseHandler(MouseHandler * h)
{
  mouse_handler_.push_back(h);
}

bool
LiteWindow::removeMouseHandler(MouseHandler * h)
{
  std::vector<MouseHandler*>::iterator i =
    std::find(mouse_handler_.begin(), mouse_handler_.end(), h);
  if (i != mouse_handler_.end()) {
    mouse_handler_.erase(i);
    return true;
  }
  return false;
}

void
LiteWindow::removeAllMouseHandlers()
{
  mouse_handler_.clear();
}

void
LiteWindow::fireMouseHandlerButton(int button, bool down,
				   float x, float y) const
{
  if (mouse_handler_.empty()) return;
  for (std::vector<MouseHandler*>::const_iterator i = mouse_handler_.begin();
       i != mouse_handler_.end(); i++)
    (*i)->button(button, down, x, y);
}

void
LiteWindow::fireMouseHandlerMotion(float x, float y) const
{
  if (mouse_handler_.empty()) return;
  for (std::vector<MouseHandler*>::const_iterator i = mouse_handler_.begin();
       i != mouse_handler_.end(); i++)
    (*i)->motion(x, y);
}


void
LiteWindow::addKeyboardHandler(KeyboardHandler * h)
{
  keyboard_handler_.push_back(h);
}

bool
LiteWindow::removeKeyboardHandler(KeyboardHandler * h)
{
  std::vector<KeyboardHandler*>::iterator i =
    std::find(keyboard_handler_.begin(), keyboard_handler_.end(), h);
  if (i != keyboard_handler_.end()) {
    keyboard_handler_.erase(i);
    return true;
  }
  return false;
}

void
LiteWindow::removeAllKeyboardHandlers()
{
  keyboard_handler_.clear();
}

void
LiteWindow::fireKeyboardHandler(int key, bool down) const
{
  if (keyboard_handler_.empty()) return;
  for (std::vector<KeyboardHandler*>::const_iterator i = keyboard_handler_.begin();
       i != keyboard_handler_.end(); i++)
    (*i)->keyboard(key, down);
}


void
LiteWindow::addTimerHandler(unsigned long millis, TimerHandler * h)
{
  int v;
  std::vector<TimerHandler*>::iterator i =
    std::find(timer_handler_.begin(), timer_handler_.end(),
	      (TimerHandler*)(0));
  if (i == timer_handler_.end()) {
    v = timer_handler_.size();
    timer_handler_.push_back(h);
  }
  else {
    v = i - timer_handler_.begin();
    *i = h;
  }
  glutTimerFunc(millis, timer, v);
}

static TimerHandler waiting_timer_handler;

bool
LiteWindow::removeTimerHandler(TimerHandler * h)
{
  std::vector<TimerHandler*>::iterator i =
    std::find(timer_handler_.begin(), timer_handler_.end(), h);
  if (i != timer_handler_.end()) {
    *i = &waiting_timer_handler;
    return true;
  }
  return false;
}

void
LiteWindow::removeAllTimerHandlers()
{
  timer_handler_.clear();
}

void
LiteWindow::fireTimerHandler(int value) 
{
  if (timer_handler_.size() < value)
    return;
  TimerHandler * h = timer_handler_[value];
  timer_handler_[value] = 0;
  if (h != 0)
    h->timer();
}

} // namespace infovis

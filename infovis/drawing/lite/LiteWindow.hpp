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
#ifndef INFOVIS_DRAWING_LITE_LITEWINDOW_HPP
#define INFOVIS_DRAWING_LITE_LITEWINDOW_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/lite/LiteGroup.hpp>

namespace infovis {
class Image;
class Manager3State;
class MouseHandler;
class KeyboardHandler;
class TimerHandler;

/**
 * Root lite, created from a window-system window.
 */
class LiteWindow : public LiteGroup
{
public:
  typedef std::vector<Path, gc_alloc<Path> > PathList;
  enum Buffer {
    buffer_color	= 1,
    buffer_depth	= 2,
    buffer_accum	= 4,
    buffer_stencil	= 8,
  };
  enum Capabilities {
    cap_rgb		= 0,
    cap_rgba		= 0,
    cap_index		= 1,
    cap_double		= 2,
    cap_accum		= 4,
    cap_alpha		= 8,
    cap_depth		= 16,
    cap_stencil		= 32,
    cap_multisample	= 128,
    cap_stereo		= 256,
    cap_luminance	= 512
  };

  enum Cursor {
    cursor_right_arrow,
    cursor_left_arrow,
    cursor_info,
    cursor_destroy,
    cursor_help,
    cursor_cycle,
    cursor_spray,
    cursor_wait,
    cursor_text,
    cursor_crosshair,
    cursor_up_down,
    cursor_left_right,
    cursor_top_side,
    cursor_bottom_side,
    cursor_left_side,
    cursor_right_side,
    cursor_top_left_corner,
    cursor_top_right_corner,
    cursor_bottom_right_corner,
    cursor_bottom_left_corner,
    cursor_inherit,
    cursor_none,
    cursor_full_crosshair,
    cursor_last
  };
  static void init(int& argc, char **argv);
  LiteWindow(const string& name, const Box& box,
	     unsigned int cap = (cap_double | cap_rgba));
  virtual ~LiteWindow();

  int pick(const Box& b, PathList& hit);
  void pick(const Box& b, List& sorted);
  void beginPick(const Box& b,
		 unsigned int * hitBuffer, unsigned sz);
  int endPick(PathList& hit,
	      unsigned int * hitBuffer, unsigned sz);

  void sortPathList(const PathList& hit, List& sorted);

  virtual void doReshape(int w, int h);
  virtual void doKeyboard(int key, bool down, int x, int y);
  virtual void doMouse(int button, bool down, int x, int y);
  virtual void doMotion(int x, int y);
  virtual void doPassiveMotion(int x, int y);
  virtual void doTimer(int value);
  virtual void doVisible(bool visible);

  virtual void run();

  const string& getName() const { return name; }
  void setName(const string& name);
  double getNearPlane() const { return near_plane; }
  void setNearPlane(double p) { near_plane = p; }
  double getFarPlane() const { return far_plane; }
  void setFarPlane(double p) { far_plane = p; }
  void setColor(const Color& c);
  const Color& getColor() const { return color; }
  void setFullscreen(bool b = true);
  void setClear(Buffer buf, bool t = true);
  bool getClear(Buffer b);

  unsigned getModifiers() const { return modifiers_; }

  Cursor getCursor() const;
  void setCursor(Cursor c);
  void setCursor(Image *);

  static int time();
  static int screenWidth();
  static int screenHeight();
  static int screenWidthMM();
  static int screenHeightMM();
  static float screenResolutionMM(); // pixels per MM
  static float screenResolutionDPI(); // pixels per inch
  static float screenResolutionPT(); // pt per inch

  static string getVendor();
  static string getRenderer();
  static string getVersion();
  static string getExtensions();

  static void beginSave(const string& fname);
  static void endSave();
  static void setTimeDivisor(float div = 1.0f);
  static float getTimeDivisor();

  static LiteWindow * getInstance() { return instance; }

  void addMouseHandler(MouseHandler *);
  bool removeMouseHandler(MouseHandler *);
  void removeAllMouseHandlers();
  void fireMouseHandlerButton(int button, bool down, float x, float y) const;
  void fireMouseHandlerMotion(float x, float y) const;
  

  void addKeyboardHandler(KeyboardHandler *);
  bool removeKeyboardHandler(KeyboardHandler *);
  void removeAllKeyboardHandlers();
  void fireKeyboardHandler(int key, bool down) const;

  void addTimerHandler(unsigned long milli, TimerHandler *);
  bool removeTimerHandler(TimerHandler *);
  void removeAllTimerHandlers();
  void fireTimerHandler(int value);

protected:
  int win;
  string name;
  double near_plane;
  double far_plane;
  static LiteWindow * instance;
  Color color;
  void do_reshape(int w, int h);
  unsigned int clear_mask;
  Image * cursor_;
  long redisplay_count_;
  unsigned modifiers_;
  std::vector<MouseHandler*> mouse_handler_;
  std::vector<KeyboardHandler*> keyboard_handler_;
  std::vector<TimerHandler*> timer_handler_;

  void modifiers(int x, int y);
  static void display();
  static void reshape(int w, int h);
  static void keyboard(unsigned char key, int x, int y);
  static void keyboardup(unsigned char key, int x, int y);
  static void special(int key, int x, int y);
  static void specialup(int key, int x, int y);
  static void mouse(int button, int state, int x, int y);
  static void motion(int x, int y);
  static void passive_motion(int x, int y);
  static void timer(int value);
  static void visibility(int state);
};

} // namespace infovis

#endif

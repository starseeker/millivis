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
#ifndef INFOVIS_DRAWING_EVENTCODES_HPP
#define INFOVIS_DRAWING_EVENTCODES_HPP

#include <infovis/drawing/inter/Event.hpp>

namespace infovis {

/**
 * Common methods for storing and retrieving informations in events.
 */
class EventCodes
{
public:
  static const string field_x;	// float field
  static const string field_y;	// float field
  static const string field_mod; // int field
  static const string field_on;	// bool field

  static float getFieldX(const Event& ev) {
    return ev[field_x].getFloatValue();
  }
  static void setFieldX(Event& ev, float v) {
    ev[field_x].setFloatValue(v);
  }
  static float getFieldY(const Event& ev) {
    return ev[field_y].getFloatValue();
  }
  static void setFieldY(Event& ev, float v) {
    ev[field_y].setFloatValue(v);
  }
  static int getFieldMod(const Event& ev) {
    return ev[field_mod].getIntValue();
  }
  static void setFieldMod(Event& ev, int v) {
    ev[field_mod].setIntValue(v);
  }

  static Point getFieldPosition(const Event& ev) {
    return Point(getFieldX(ev), getFieldY(ev));
  }
  static void setFieldPosition(Event& ev, const Point& p) {
    setFieldX(ev, x(p));
    setFieldY(ev, y(p));
  }

  static bool getFieldOn(const Event& ev) {
    return ev[field_on].getBoolValue();
  }

  static void setFieldOn(Event& ev, bool v) {
    ev[field_on].setBoolValue(v);
  }

};
} // namespace infovis 

#endif // INFOVIS_DRAWING_EVENTCODES_HPP

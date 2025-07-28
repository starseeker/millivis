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
#ifndef INFOVIS_DRAWING_LITE_MENU_HPP
#define INFOVIS_DRAWING_LITE_MENU_HPP

#include <infovis/drawing/lite/LiteBox.hpp>
#include <infovis/drawing/lite/Border.hpp>
#include <infovis/drawing/notifiers/BeginEnd.hpp>
#include <infovis/drawing/notifiers/BoundedRange.hpp>
#include <infovis/drawing/inter/Interactor3States.hpp>

namespace infovis {

/**
 * Lite menu
 */
class LiteMenu : public LiteBox,
		 public Interactor3States,
		 public BoundedRange,
		 public AbstractBoundedRangeObservable,
		 public DefaultBeginEndObservable
{
public:
  LiteMenu(const Color& c = color_white);
  LiteMenu(const Color& c, float max_width, float max_height = 0);

  Lite * clone() const;

  virtual Box getBounds() const;
  virtual Box getInBounds() const;
  
  virtual Interactor * interactor(const string& name, int tool_id);
  virtual void render(const RenderContext& c);

  // BoundedRange
  virtual float min() const;
  virtual float max() const;
  virtual float value() const;
  virtual float range() const;

  // AbstractBoundedRangeObservable
  virtual const BoundedRange * getBoundedRange() const;

  // Interactor3States
  virtual void doMove(const Event& ev);
  virtual bool doStart(const Event& ev);
  virtual void doDrag(const Event& ev);
  virtual void doFinish(const Event& ev);

  // Interactor
  virtual void doAbort(const Event& ev);

  virtual void setSelected(int s);
  virtual int getSelected() const;

  virtual const Color& getColor() const;
  virtual void setColor(const Color& c);

  const Vector& getMaxSize() const { return max_size_; }
protected:
  virtual void doRender(const RenderContext&);
  virtual int selectedFromPos(const Point& pos);
  Vector max_size_;
  Color color_;
  int selected_;
  bool was_visible_;
  Border border_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_MENU_HPP

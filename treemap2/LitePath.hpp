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
#ifndef TREEMAP2_LITEPATH_HPP
#define TREEMAP2_LITEPATH_HPP

#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/lite/Lite.hpp>
#include <infovis/drawing/lite/Border.hpp>
#include <infovis/drawing/inter/Interactor3States.hpp>
#include <infovis/drawing/notifiers/BoundedRange.hpp>

#include <types.hpp>

namespace infovis {

class LitePath : public Lite,
		 public Interactor3States,
		 public BoundedRange,
		 public AbstractBoundedRangeObservable
{
public:
  LitePath(node_descriptor n,
	   const Tree& tree,
	   const StringColumn& name,
	   Font * font);
  
  // BoundedRange
  virtual float min() const;
  virtual float max() const;
  virtual float value() const;
  virtual float range() const;

  virtual const BoundedRange * getBoundedRange() const;
  void setReference(int i);
  int getReference()			{ return reference_; }
  node_descriptor getPath() const 	{ return node_; }
  void setPath(node_descriptor n);
  void setVisible(bool b);
  Box getBounds() const;
  Box getInBounds() const;
  void setBounds(const Box& b);
  Point getPosition() const		{ return position_; }
  void setPosition(const Point& p)	{ position_ = p; }

  Lite * clone() const;
  void setSelected(int s)		{ selected_ = s; }
  int getSelected() const		{ return selected_; }
  node_descriptor getSelectedRoot() const;
  BoundedRangeObservable& getLeftRight() { return left_right_; }

  Interactor * interactor(const string& name, int tool_id);

  void moveSmart(const Point& pos);
  int selectedFromPos(const Point& pos);

  // Interactor3States
  virtual void doMove(const Event& ev);
  virtual bool doStart(const Event& ev);
  virtual void doDrag(const Event& ev);
  virtual void doFinish(const Event& ev);
  virtual void doAbort(const Event& ev);
  
protected:
  void updateBounds();
  void doRender(const RenderContext& rc);

  const Tree& tree_;
  node_descriptor node_;
  const StringColumn& name_;
  Point position_;
  Font * font_;
  int depth_;
  int selected_;
  Vector size_;
  bool was_inside_;
  bool was_visible_;
  int reference_;
  Border border_;
  DefaultBoundedRangeObservable left_right_;
};

} // namespace infovis

#endif // TREEMAP2_LITEPATH_HPP

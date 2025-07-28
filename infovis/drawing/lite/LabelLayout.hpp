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
#ifndef INFOVIS_DRAWING_LITE_LABELLAYOUT_HPP
#define INFOVIS_DRAWING_LITE_LABELLAYOUT_HPP

#include <infovis/drawing/lite/LiteDisplacedLabel.hpp>

namespace infovis {

/**
 * Layout manager for LiteBox
 */
class LabelLayout : public gc	// no ressources so no need to call destructor
{
public:
  typedef std::vector<LiteDisplacedLabel*,gc_alloc<LiteDisplacedLabel*> > List;

  enum LayoutResult {
    layout_dont_know,
    layout_dont_overlap,
    layout_overlap
  };

  LabelLayout(const Box& box);
  virtual ~LabelLayout();

  LayoutResult layout(const List& list, const Point& center, float radius); // not virtual

  virtual LayoutResult doLayout() = 0;

  virtual Box getBounds() const;
  virtual void setBounds(const Box& box);
  
  virtual const List& getList() const;
  virtual void setList(const List& v);

  virtual const Point& getCenter() const;
  virtual void setCenter(const Point& v);
  
  virtual float getRadius() const;
  virtual void setRadius(float v);

  virtual LiteDisplacedLabel * getLabel(int i) const;
  virtual int getLabelCount() const;

  
protected:
  Box bounds_;
  List list_;
  Point center_;
  float radius_;
};  

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_LABELLAYOUT_HPP

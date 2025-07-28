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
#ifndef INFOVIS_DRAWING_ANIMATEBOXLIST_HPP
#define INFOVIS_DRAWING_ANIMATEBOXLIST_HPP

#include <infovis/drawing/Animate.hpp>
#include <vector>

namespace infovis {

/**
 * Animation for interpolating a list of boxes
 */
class AnimateBoxList : public Animate
{
public:
  typedef std::vector<Box, gc_alloc<Box,true> > BoxList;
  AnimateBoxList(int size);

  virtual void setStartList(const BoxList& bl);
  BoxList& getStartList() { return start_; }
  const BoxList& getStartList() const { return start_; }

  virtual void setEndList(const BoxList& el);
  BoxList& getEndList() { return end_; }
  const BoxList& getEndList() const { return end_; }

  virtual void setTexCoords(const BoxList& tc);
  BoxList& getTexCoords() { return tex_coords_; }
  const BoxList& getTexCoords() const { return tex_coords_; }

  virtual void computeTexCoords(const Box& tex_bounds);
  virtual void setTexture(unsigned int t);
  virtual void swap();

  virtual void render(float param = 0.0f);


protected:
  BoxList start_;
  BoxList end_;
  BoxList tex_coords_;
  unsigned int texture_;
};

} // namespace infovis


#endif // INFOVIS_DRAWING_ANIMATEBOXLIST_HPP


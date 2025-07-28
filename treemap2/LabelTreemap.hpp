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
#ifndef TREEMAP2_LABELTREEMAP_HPP
#define TREEMAP2_LABELTREEMAP_HPP

#include <infovis/drawing/drawing.hpp>
#include <infovis/drawing/lite/LiteGroup.hpp>
#include <infovis/drawing/notifiers/Change.hpp>
#include <types.hpp>
#include <LiteTreemap.hpp>

namespace infovis {

class Font;
class LiteButton;

class LabelTreemap : public LiteGroup,
		     public ChangeObserver
{
public:
  LabelTreemap(LiteTreemap *, const string& name, Font * font);

  Lite * clone() const;
  void doRender(const RenderContext& );

  void setBoxes(const PathBoxes& boxes) {
    boxes_ = boxes;
  }
  const PathBoxes& getBoxes() const { return boxes_; }

  void setHit(node_descriptor hit) { hit_ = hit; }
  node_descriptor getHit() const { return hit_; }
  bool isLabelOver(const Box& box) const;
  bool addLabel(const Box& box);

  void clear();
  bool layoutLabel(const string& label, const Box& box);
  void layoutLabels();
    
  // ChangeObserver
  virtual void changed(ChangeObservable *);

protected:
  LiteTreemap * treemap_;
  const Tree& tree_;
  string name_;
  Font * font_;
  PathBoxes boxes_;
  PathBoxes label_boxes_;
  typedef std::vector<node_descriptor> Labels;
  Labels labels_;
  node_descriptor hit_;
};

} // namespace infovis

#endif // TREEMAP2_LABELTREEMAP_HPP

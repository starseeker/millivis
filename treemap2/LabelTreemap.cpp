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
#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/lite/LiteButton.hpp>
#include <infovis/drawing/lite/LiteFrame.hpp>
#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/inter/KeyCodes.hpp>
#include <infovis/drawing/gl_support.hpp>

#include <LabelTreemap.hpp>

#include <iostream>

namespace infovis {

LabelTreemap::LabelTreemap(LiteTreemap * treemap,
			   const string& name,
			   Font * font)
  : treemap_(treemap),
    tree_(treemap->tree_),
    name_(name),
    font_(font),
    hit_(root(tree_))
{ }

Lite *
LabelTreemap::clone() const
{
  return new LabelTreemap(treemap_, name_, font_);
}

void
LabelTreemap::doRender(const RenderContext& rc)
{
  if (boxes_.empty()) return;

  set_color(color_white);
  int i;
  for (i = 0; i != boxes_.size(); i++) {
    const Box& b = boxes_[i];
    stroke_box(b);
  }
}

bool
LabelTreemap::isLabelOver(const Box& box) const
{
  for (PathBoxes::const_iterator i = label_boxes_.begin();
       i != label_boxes_.end(); i++) {
    const Box& b = *i;
    if (infovis::intersects(b, box))
      return true;
  }
  return false;
}

bool
LabelTreemap::addLabel(const Box& box)
{
  if (! infovis::contains(boxes_[0], box)) // not inside treemap
    return false;
  if (isLabelOver(box))
    return false;
  label_boxes_.push_back(box);
  return true;
}

bool
LabelTreemap::layoutLabel(const string& label, const Box& box)
{
  Box bounds;
  if (font_)
      bounds = font_->getStringBounds(label);
  Vector size(width(bounds), height(bounds));

  // Try to put it top-right, bottom-right, top-left, bottom-left in
  // that order
  const Point top_right_min(xmax(box), ymax(box)-height(bounds));
  const Box top_right(top_right_min, size);
  const Point top_left_min(xmin(box), ymax(box)-height(bounds));
  const Box top_left(top_left_min, size);
  const Point bottom_right_min(xmax(box), ymin(box));
  const Box bottom_right(bottom_right_min, size);
  const Point bottom_left_min(xmin(box), ymin(box));
  const Box bottom_left(bottom_left_min, size);

  if (width(bounds) <= width(box) &&
      height(bounds) <= height(box)) {
    return (addLabel(top_left) ||
	    addLabel(bottom_left) ||
	    addLabel(top_right) ||
	    addLabel(bottom_right));
  }
  return (addLabel(top_right) ||
	  addLabel(bottom_right) ||
	  addLabel(top_left) ||
	  addLabel(bottom_left));
}

void
LabelTreemap::changed(ChangeObservable * obs)
{
  std::cout << "changed called\n";
  for (int j = 0; j < childCount(); j++) {
    LiteButton * but = dynamic_cast<LiteButton*>(getChild(j));
    if (obs == but) {
      std::cout << "found root=" << labels_[j] << std::endl;
      treemap_->update_root(labels_[j]);
      repaint();
      return;
    }
  }
  std::cout << "found no root" << std::endl;
}

void
LabelTreemap::clear()
{
  label_boxes_.clear();
  labels_.clear();
  
  for (int j = 0; j < childCount(); j++) {
    Lite * l = getChild(j);
    delete l;
  }
  group_.clear();		// +++ FIXME
}

void
LabelTreemap::layoutLabels() 
{
  StringColumn * col = StringColumn::cast(tree_.find_column(name_));
  if (col == 0 || boxes_.empty()) return;

  clear();
  
  int i = boxes_.size()-1;
  for (node_descriptor n = hit_;
       n != root(tree_) && i >= 0;
       n = parent(n, tree_), i--) {
    const string& label = (*col)[n];
    if (layoutLabel(label, boxes_[i])) {
      LiteLabel * lab = new LiteLabel(label,
				      LiteLabel::just_left,
				      font_,
				      color_black,
				      color_none,
				      color_white);
      LiteFrame * frame = new LiteFrame(lab, color_black, 2);
      frame->setPosition(min(label_boxes_[label_boxes_.size()-1]));
      LiteButton * but = new LiteButton(frame);
      but->addChangeObserver(this);
      labels_.push_back(n);
      addChild(but);
    }
  }
}


} // namespace infovis

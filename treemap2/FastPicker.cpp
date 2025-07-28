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
#include <infovis/drawing/gl_support.hpp>
#include <infovis/drawing/lite/LabelLayoutRect.hpp>
#include <infovis/drawing/lite/LabelLayoutSimple.hpp>

#include <FastPicker.hpp>

namespace infovis {

FastPicker::FastPicker(const Tree& tree, const BorderDrawer& b)
  : x_pos(-1),
    y_pos(-1),
    hit_(root(tree)),
    label_level(1),
    border(b),
    show_all_labels_(false)
{ }

void
FastPicker::start()
{
  hit_ = tree_traits<Tree>::nil();
  path_boxes.clear();
  labels.clear();
  label_centers.clear();
}

void
FastPicker::set_pos(int x, int y)
{
  x_pos = x;
  y_pos = y;
  labels.clear();
  label_centers.clear();
}


void
FastPicker::set_label_level(int l)
{
  label_level = l;
}

void
FastPicker::set_labels_clip(Box b)
{
  labels_clip = b;
}


void
FastPicker::finish(const Box& bounds,
		   Font * font,
		   float line_alpha,
		   const StringColumn& name)
{
#if 1
  LabelLayoutRect::List disp_labels;
  for(int i = 0; i < label_centers.size(); i++) {
    LiteDisplacedLabel * ld = new LiteDisplacedLabel(name[labels[i]],
						     label_centers[i],
						     LiteLabel::just_center,
						     font,
						     color_black,
						     color_none,
						     color_white
						     );
    ld->setLineAlpha(line_alpha);
    disp_labels.push_back(ld);
  }
  if (empty(labels_clip)) {
    LabelLayoutSimple simple_layout(bounds);
    simple_layout.layout(disp_labels, center(labels_clip), width(labels_clip)/2);
  }
  else {
    LabelLayoutRect rect_layout(bounds);
    rect_layout.layout(disp_labels, center(labels_clip), width(labels_clip)/2);
  }
  Lite::RenderContext rc;
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (LabelLayoutRect::List::const_iterator j = disp_labels.begin();
       j != disp_labels.end(); j++) {
    (*j)->render(rc);
    delete *j;
  }
  glPopAttrib();
#else
  float desc = font->getDescent();
  for(int i = 0; i < label_centers.size(); i++) {
    const string& str = name[labels[i]];
    float w = font->stringWidth(str);
    set_color(color_white);
    font->paintGrown(str,
		     x(label_centers[i]) - w/2,
		     y(label_centers[i])+desc);
    set_color(color_black);
    font->paint(str,
		x(label_centers[i]) - w/2,
		y(label_centers[i])+desc);
  }
#endif
}

} // namespace infovis

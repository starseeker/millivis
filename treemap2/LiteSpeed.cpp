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
#include <LiteSpeed.hpp>
#include <list>
#include <iostream>

namespace infovis {

LiteSpeed::LiteSpeed(LiteTreemap * tm, const Box& bounds, Font * font)
  : LiteBounded(bounds),
    tm_(tm),
    font_(font)
{ }

Interactor *
LiteSpeed::interactor(const string& name, int tool_id)
{
  if (isA(name) && tool_id == mouse_id) {
    return this;
  }
  return 0;
}

void
LiteSpeed::doRender(const RenderContext& rc)
{
  if (rc.is_picking) {
    draw_box(bounds);
    return;
  }
  char buffer[1024];
  //if (tm_->getFps() == 0) return;
  sprintf(buffer, "Speed: %03.3ffps/%03.3fspf %d items",
	  tm_->getFps(), 1.0f/tm_->getFps(),
	  tm_->getDisplayedItems());
  set_color(color_white);
  draw_box(bounds);
  set_color(color_black);
  float baseline = ymin(bounds);
  if (font_) {
     baseline += font_->getDescent();
     const float xright = xmax(bounds)-font_->stringWidth(buffer);
     font_->paint(buffer, xright, baseline);
  }

  const StringColumn& name = tm_->getNames();

  std::list<string> path;
  const Tree& tree = tm_->getTree();
  for (node_descriptor n = tm_->getMenuPath();
       n != root(tree);
       n = parent(n, tree)) {
    path.push_front(name[n]);
  }
  path.push_front(name[root(tree)]);
  float x = 0;
  bars_.clear();
  for (std::list<string>::const_iterator i = path.begin();
       i != path.end() && x < xmax(bounds);
       i++) {
    const string& n = *i;
    bars_.push_back(x);
    if (font_) {
       font_->paint(n, x+1, baseline);
       x += font_->stringWidth(n) + 1;
    }
    glRectf(x-1, ymin(bounds),x,ymax(bounds));
  }
  bars_.push_back(x);
}

Lite *
LiteSpeed::clone() const
{
  return new LiteSpeed(tm_, getBounds(), font_);
}
  
LiteTreemap *
LiteSpeed::getTreemap() const {return tm_; }

void
LiteSpeed::setTreemap(LiteTreemap* v)
{
  tm_ = v;
}

void
LiteSpeed::doFinish(const Event& ev)
{
  //if (getFieldMod(ev) == 0) return;
  float x = getFieldX(ev);
  if (x > bars_.back()) {
    tm_->update_root(root(tm_->tree_));
    repaint();
    return;
  }

  node_descriptor n = tm_->getMenuPath();
  for (int i = bars_.size()-2; i >= 0; i--) {
    if (x > bars_[i]) {
      //std::cout << "root =" << n << std::endl;
      tm_->update_root(n);
      repaint();
      return;
    }
    n = parent(n, tm_->tree_);
  }
}

} // namespace infovis

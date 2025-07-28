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
#ifndef INFOVIS_DRAWING_LITE_LITEDISPLACEDLABEL_HPP
#define INFOVIS_DRAWING_LITE_LITEDISPLACEDLABEL_HPP

#include <infovis/drawing/lite/LiteLabel.hpp>
#include <vector>

namespace infovis {

/**
 * Label with a line.  Used for dynamic labeling.
 */
class LiteDisplacedLabel : public LiteLabel
{
public:
  typedef std::vector<Point, gc_alloc<Point,true> > Path;
  LiteDisplacedLabel(const string& label,
		     const Point& orig,
		     justification j = just_left,
		     Font * font = 0,
		     const Color& fg = color_black,
		     const Color& bg = color_white,
		     const Color& gfg = color_none);

  virtual void doRender(const RenderContext& c);
  virtual Lite * clone() const;

  const Point& getOrig() const { return orig_; }
  const Path& getPath() const { return path_; }
  void addPath(const Point& p) { path_.push_back(p); }
  void setPath(const Path& p) { path_ = p; } // at your own risk
  void setLineAlpha(float alpha);
  float getLineAlpha() const { return line_alpha_; }
protected:
  Point orig_;
  Path path_;
  float line_alpha_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_LITEDISPLACEDLABEL_HPP

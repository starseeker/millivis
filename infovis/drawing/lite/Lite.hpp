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
#ifndef INFOVIS_DRAWING_LITE_LITE_HPP
#define INFOVIS_DRAWING_LITE_LITE_HPP

#include <infovis/drawing/drawing.hpp>
#include <vector>

namespace infovis {
class Interactor;

enum {
  mouse_id,
  keyboard_id
};

/**
 * A Lite is the root class for all retained graphics objects.
 *
 * Lite objects form a tree.
 */
class Lite : public virtual gc
{
public:
  typedef std::vector<Lite*, gc_alloc<Lite*> > List;
  typedef std::vector<int, gc_alloc<int,true> > Path;
  typedef List::iterator iterator;
  typedef List::const_iterator const_iterator;

  /**
   * Context for rendering and picking in OpenGL
   */ 
  struct RenderContext {
    bool is_picking;
    float quality;
    RenderContext()
      : is_picking(false), quality(1.0f) { }
    RenderContext(bool p, float q = 1.0f)
      : is_picking(p), quality(q) {}
  };

  Lite() : is_visible(true) {}
  virtual ~Lite();

  virtual Box getBounds() const = 0;
  virtual void setBounds(const Box& b) = 0;
  virtual Box getMinBounds() const;
  virtual Point getPosition() const = 0;
  virtual void setPosition(const Point& p) = 0;
  virtual void movePosition(const Vector& v);
  virtual Vector getSize() const;
  virtual void setSize(const Vector& s);
  virtual bool intersects(const Box& b) const;
  virtual bool contains(const Box& b) const;
  virtual bool inside(const Point& p) const;
  virtual void render(const RenderContext&);
  virtual Lite * clone() const = 0;
  virtual Interactor * interactor(const string& name, int toolid);
  virtual int childCount() const;
  virtual Lite * getChild(int index) const;
  virtual void setChild(int index, Lite *);
  virtual int indexOf(Lite *) const;
  virtual Lite * getPath(const Path& p);
  virtual void addChild(Lite *);
  virtual void removeChild(Lite *);
  virtual void insertChild(int pos, Lite *);
  virtual void eraseChild(int pos);
  virtual void removeAll();
  virtual iterator begin();
  virtual const_iterator begin() const;
  virtual iterator end();
  virtual const_iterator end() const;

  virtual void repaint() const;
  virtual void setVisible(bool b);
  bool isVisible() const { return is_visible; }

  virtual void invalidate();
  virtual void pack();
protected:
  virtual void doRender(const RenderContext&);
  bool is_visible;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_LITE_LITE_HPP

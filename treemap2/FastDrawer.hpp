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
#ifndef TREEMAP2_FASTDRAWER_HPP
#define TREEMAP2_FASTDRAWER_HPP

#include <infovis/tree/tree_traits.hpp>
#include <infovis/tree/treemap/drawing/gl_drawer.hpp>
#include <infovis/tree/treemap/drawing/color_drawer.hpp>
#include <infovis/tree/treemap/drawing/border_drawer.hpp>
#include <infovis/drawing/drawing.hpp>
#include <BorderDrawer.hpp>
#include <types.hpp>

#define VERTEX_INFO 4
#define NO_TEXTURE_TRANSFORM
//#define NO_TEXTURE

namespace infovis {


class FastDrawer : public BorderDrawer, public null_drawer<Tree,Box>
{
public:
  struct Point3D {
    Coord x, y, z;
    Point3D() { }
    Point3D(Coord X, Coord Y, Coord Z)
      : x(X), y(Y), z(Z) { }
  };

  FastDrawer(const Tree& t,
	     const FloatColumn * color_prop,
	     unsigned size = (1<<15)); // use large buffer
  ~FastDrawer();

  void set_color_ramp(const std::vector<Color>& colors);
  const std::vector<Color>& get_color_ramp() const;

  void set_color_prop(const FloatColumn * prop);
  
  void set_color_smooth(bool smooth = false);
  bool get_color_smooth() const;

  void set_color_range(float min_value = 0, float range = 0);
  void get_color_range(float& min_value, float& range);

  void start(gl::begin_mode mode = gl::bm_quads);

#ifdef NO_TEXTURE
  static inline float interp(float t, float v1, float v2) {
    return (1-t)*v1 + t*v2;
  }

  static inline Color interp(float t, const Color& c1, const Color& c2) {
    Color ret;
    detail::round(ret[Color::red],
		  interp(t, c1[Color::red], c2[Color::red]));
    detail::round(ret[Color::green],
		  interp(t, c1[Color::green], c2[Color::green]));
    detail::round(ret[Color::blue],
		  interp(t, c1[Color::blue], c2[Color::blue]));
    return ret;
  }
#endif
  inline float compute_color(float c) {
#ifdef NO_TEXTURE_TRANSFORM
    return (c - color_min_) * color_scale_;
#endif
#ifdef NO_TEXTURE
    c = (c - color_min_) * color_scale_;
    int index = int(c);
    if (index < 0)
      index = 0;
    else if (index >= color_ramp_.size())
      index = color_ramp_.size()-1;
    if (color_smooth_) {
      float t = c - index;
      if (t == 0)
	c = *reinterpret_cast<float*>(&color_ramp_[index]);
      else {
	const Color& c1 = color_ramp_[index];
	const Color& c2 = color_ramp_[index+1];
	Color col(interp(t, c1, c2));
	c = *reinterpret_cast<float*>(&col);
      }
    }
    else {
      c = *reinterpret_cast<float*>(&color_ramp_[index]);
    }
    return c;
#endif
  }

  void check_flush(int i) {
    if ((size_ - current_) <= (VERTEX_INFO * i)) {
      flush();
    }
  }

  inline void push_vertex(float x, float y, float depth, float tex) {
    data_[current_++] = x;
    data_[current_++] = y;
    data_[current_++] = depth;
    data_[current_++] = tex;
  }

  inline void push(const Box& b, unsigned depth, float c) {
    if (dryrun_) return;
    check_flush(4);
    c = compute_color(c);
    push_vertex(xmin(b), ymin(b), depth, c);
    push_vertex(xmax(b), ymin(b), depth+0.5f, c);
    push_vertex(xmax(b), ymax(b), depth, c);
    push_vertex(xmin(b), ymax(b), depth-0.5f, c);
  }

  bool begin_box(const Box& b,
		 node_descriptor n,
		 unsigned depth) {
    return
      (depth <= max_depth_) &&
      (int(xmin(b)) != int(xmax(b))) &&
      (int(ymin(b)) != int(ymax(b)));
  }
  void draw_box(const Box& b,
		node_descriptor n,
		unsigned depth) {
    if (filter_->fast_get(n) == 0)
      push(b, depth, (*color_prop_)[n]);
  }
  void draw_border(Box& b, 
		   node_descriptor n,
		   unsigned depth) {
    if (begin_border(b, n, depth)) {
#if 1
      if (! is_leaf(n, tree_)) {
	float c = (*color_prop_)[n];
	Box b_box = b;
	if (left_border(b_box, n, depth)) {
	  push(b_box, depth, c);
	  b_box = b;
	}
	if (top_border(b_box, n, depth)) {
	  push(b_box, depth, c);
	  b_box = b;
	}
	if (right_border(b_box, n, depth)) {
	  push(b_box, depth, c);
	  b_box = b;
	}
	if (bottom_border(b_box, n, depth)) {
	  push(b_box, depth, c);
	}
      }
#endif
      remaining_box(b, n, depth);
    }
  }
  void remove_border(Box& b, 
		   node_descriptor n,
		   unsigned depth) {
    if (begin_border(b, n, depth)) {
      remaining_box(b, n, depth);
    }
  }
  void end_box(const Box& b, 
	       node_descriptor n,
	       unsigned depth) { }

  void finish();
  void set_max_depth(unsigned m) { max_depth_ = m; }
  void flush();
  void set_mode(gl::begin_mode mode);
  void set_dryrun(bool d) { dryrun_ = d; }
  float * get_data() const { return data_; }
  int get_size() const { return size_; }
  void set_current(int c) { current_ = c; }
  
protected:
  void allocate_ressources();

  unsigned size_;
  const FloatColumn * color_prop_;
  unsigned max_depth_;		// only display treemap up to that depth
  const FilterColumn * filter_;

  float * data_;
  int current_;
  gl::begin_mode mode_;

  unsigned int color_texture_;
  bool color_smooth_;
  std::vector<Color> color_ramp_;
  float color_range_;
  float color_min_;
  float color_scale_;
  float color_delta_;		// experimental
  bool dryrun_;

  // for NVidia extension GL_NV_vertex_array_range and GL_NV_fence
  unsigned total_size_;		// total size of allocated AGP memory
  int current_buffer_;
  void next_buffer();
  void wait_fence();
  struct var_buffer {
    float* pointer;
    unsigned int fence;
  };
  var_buffer buffer[4];
};

} // namespace infovis

#endif // TREEMAP2_FASTDRAWER_HPP

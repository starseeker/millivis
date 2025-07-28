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
#ifndef INFOVIS_DRAWING_COLOR_HPP
#define INFOVIS_DRAWING_COLOR_HPP

#include <infovis/alloc.hpp>
// TODO: Removed boost/config.hpp and boost/detail/select_type.hpp - modernized to C++17
#include <infovis/drawing/colors/detail.hpp>
#include <limits>
#include <utility>
#include <cmath>

namespace infovis {

/**
 * Color space traits
 */
template <class Color>
struct color_space_traits {
  typedef typename Color::channel_type channel_type;
  typedef typename Color::channel_iterator channel_iterator;
  typedef typename Color::color_space color_space;

};

/**
 * RGB color space
 */
struct color_space_rgb {
  enum channel_name { red, green, blue, last_channel };
  static const char * name() { return "rgb"; }
};
/**
 * RGBA color space
 */
struct color_space_rgba {
  enum channel_name { red, green, blue, alpha, last_channel };
  static const char * name() { return "rgba"; }
};

/**
 * Template for a color type
 */
template <class T, class S>
struct color_type : public S
{
  typedef T channel_type;
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef const T* channel_iterator;
  typedef S color_space;
  T channel[S::last_channel];
    
  color_type() { }
  explicit color_type(const T& val) {
    for (int chan = 0; chan < S::last_channel; chan++) {
      channel[chan] = val;
    }
  }
  template <class ChannelIt>
  color_type(ChannelIt start, ChannelIt end) {
    for (int chan = 0; chan < S::last_channel; chan++) {
      if (start != end)
	channel[chan] = *start++;
    }
  }
  friend std::pair<const_iterator,const_iterator>
  channels(const color_type& c) {
    return std::make_pair(c.channel, c.channel+S::last_channel);
  }
  const channel_type& operator[](int i) const { return channel[i]; }
  channel_type& operator[](int i) { return channel[i]; }
  static T one() {
    T t;
    return infovis::detail::one(t);
  }

  iterator begin() { return channel; }
  const_iterator begin() const { return channel; }
  iterator end() { return channel + S::last_channel; }
  const_iterator end() const { return channel + S::last_channel; }

  bool is_valid() const { return true; }
};

template <class T> struct color_rgba;
template <class T> struct color_rgb;
  
/**
 * Template for a color_rgb type
 */
template <class T>
struct color_rgb : public color_type<T, color_space_rgb>
{
  typedef color_type<T, color_space_rgb> super;

  color_rgb() {
    this->channel[color_space_rgb::red] = super::one();
    this->channel[color_space_rgb::green] = super::one();
    this->channel[color_space_rgb::blue] = super::one();
  }
  explicit color_rgb(const T& val) : super(val) {}
  template <class S>
  color_rgb(const color_rgb<S>& r) {
    for (int chan = 0; chan < color_space_rgb::last_channel; chan++) {
      infovis::detail::convert(this->channel[chan],r.channel[chan]);
    }
  }

  template <class ChannelIt>
  color_rgb(ChannelIt start, ChannelIt end) {
    int chan;
    for (chan = 0; chan < color_space_rgb::last_channel; chan++) {
      if (start != end)
	this->channel[chan] = *start++;
    }
    while(chan < color_space_rgb::last_channel)
      this->channel[chan++] = T(0);
  }
  color_rgb(unsigned int r, unsigned int g, unsigned int b) {
    this->channel[color_space_rgb::red]   = T(r * super::one() / 255);
    this->channel[color_space_rgb::green] = T(g * super::one() / 255);
    this->channel[color_space_rgb::blue]  = T(b * super::one() / 255);
  }
  color_rgb(double r, double g, double b) {
    detail::round(this->channel[color_space_rgb::red], r * super::one());
    detail::round(this->channel[color_space_rgb::green], g * super::one());
    detail::round(this->channel[color_space_rgb::blue], b * super::one());
  }
};

/**
 * Template for a color_rgba type
 */
template <class T>
struct color_rgba : public color_type<T, color_space_rgba>
{
  typedef color_type<T, color_space_rgba> super;
  color_rgba() {
    this->channel[color_space_rgba::red] = super::one();
    this->channel[color_space_rgba::green] = super::one();
    this->channel[color_space_rgba::blue] = super::one();
    this->channel[color_space_rgba::alpha] = super::one();
  }
  color_rgba(const color_rgb<T>& c);
  explicit color_rgba(const T& val) : super(val)  { }
  template <class S>
  color_rgba(const color_rgba<S>& r) {
    for (int chan = 0; chan < color_space_rgba::last_channel; chan++) {
      infovis::detail::convert(this->channel[chan], r.channel[chan]);
    }
  }

  template <class ChannelIt>
  color_rgba(ChannelIt start, ChannelIt end) {
    int chan;
    for (chan = 0; chan < color_space_rgba::last_channel; chan++) {
      if (start != end)
	this->channel[chan] = *start++;
    }
    while(chan < color_space_rgba::last_channel)
      this->channel[chan++] = super::one();
  }
  color_rgba(unsigned int r, unsigned int g, unsigned int b,
	     unsigned int a = 255) {
    this->channel[color_space_rgba::red]   = T(r * super::one() / 255);
    this->channel[color_space_rgba::green] = T(g * super::one() / 255);
    this->channel[color_space_rgba::blue]  = T(b * super::one() / 255);
    this->channel[color_space_rgba::alpha] = T(a * super::one() / 255);
  }
  color_rgba(double r, double g, double b, double a = 1.0) {
    detail::round(this->channel[color_space_rgba::red], r * super::one());
    detail::round(this->channel[color_space_rgba::green], g * super::one());
    detail::round(this->channel[color_space_rgba::blue], b * super::one());
    detail::round(this->channel[color_space_rgba::alpha], a * super::one());
  }
};

template <class T>
inline
color_rgba<T>::color_rgba(const color_rgb<T>& r)
{
  for (int chan = 0; chan < color_space_rgb::last_channel; chan++) {
    this->channel[chan] = r.channel[chan];
  }
  this->channel[color_space_rgba::alpha] = super::one();
}

template <class S, class T>
inline void
convert(color_rgba<S>& rgba, const color_rgb<T>& c)
{
  for (int chan = 0; chan < color_space_rgb::last_channel; chan++)
    infovis::detail::convert(rgba[chan], c[chan]);
  rgba[color_space_rgba::alpha] = color_rgba<S>::one();
}

template <class S, class T>
inline void
convert(color_rgb<S>& rgba, const color_rgba<T>& c)
{
  for (int chan = 0; chan < color_space_rgb::last_channel; chan++)
    infovis::detail::convert(rgba[chan], c[chan]);
}


}; // namespace infovis

#endif // INFOVIS_DRAWING_COLOR_HPP

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
#include <Recorder.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

namespace infovis {

Recorder::Recorder()
  : current_(0)
{ }

void
Recorder::set_current(unsigned c)
{
  if (c > size())
    c = 0;
  current_ = c;
}

unsigned
Recorder::get_current() const
{
  if (current_ < size())
    return current_;
  return 0;
}

void
Recorder::advance(int i)
{
  if (size() == 0) {
    current_ = 0;
    return;
  }

  int c = current_ + i;
  if (c < 0)
    while (c < 0)
      c += size();
  else
    c %= size();
  set_current(unsigned(c));
}

void
Recorder::add(const RecordItem& r)
{
  if (size() == 0)
    super_vector::push_back(r);
  else if (r == current())
    return;			// don't duplicate
  else {
    super_vector::insert(super_vector::begin()+get_current()+1, r);
    advance(1);
  }
}

const RecordItem&
Recorder::current() const
{
  return *(super_vector::begin()+get_current());
}

static Ramp
str_to_ramp(const string& str)
{
  if (str == "categorical" ||
      str == "categorical1")
    return ramp_categorical1;
  else if (str == "categorical2")
    return ramp_sequential2;
  else if (str == "sequential1")
    return ramp_sequential1;
  else
    return ramp_sequential2;
}

static const char *
ramp_to_str(Ramp r)
{
  switch(r) {
  case ramp_categorical1:
    return "categorical1";
  case ramp_categorical2:
    return "categorical2";
  case ramp_sequential1:
    return "sequential1";
  case ramp_sequential2:
    return "sequential2";
  }
  return "sequential1";		// don't know??
}

static bool
str_to_bool(const string& str)
{
  return (str == "true");
}

static const char *
bool_to_str(bool b)
{
  return b ? "true" : "false";
}


static float
str_to_float(const string& str)
{
  return float(atof(str.c_str()));
}

static const char *
float_to_str(float f)
{
  static char buffer[100];

  sprintf(buffer, "%f", f);
  return buffer;
}

static LiteTreemap::Layout
str_to_layout(const string& str)
{
  if (str == "squarified")
    return LiteTreemap::layout_squarified;
  else if (str == "strip")
    return LiteTreemap::layout_strip;
  else if (str == "slice_and_dice")
    return LiteTreemap::layout_slice_and_dice;
  else if (str == "scatter_plot")
    return LiteTreemap::layout_scatter_plot;
  return LiteTreemap::layout_slice_and_dice;
}

static const char *
layout_to_str(LiteTreemap::Layout l)
{
  switch(l) {
  case LiteTreemap::layout_squarified:
    return "squarified";
  case LiteTreemap::layout_strip:
    return "strip";
  case LiteTreemap::layout_slice_and_dice:
    return "slice_and_dice";
  case LiteTreemap::layout_scatter_plot:
    return "scatter_plot";
  }
  return "slice_and_dice";	// don't know
}

void
Recorder::load(std::istream& in)
{
  char buffer[2048];
  RecordItem item;
  while (in) {
    in.getline(buffer, 2048);
    string line(buffer);
    if (line.length() == 0)
      continue;
    if (line[0] == '#' || line[0] == '!')
      continue;
    size_t prop_start = 0;
    size_t prop_end = 0;
    size_t val_start = 0;
    size_t val_end = 0;
    size_t i = 0;
    for (; i < line.length(); i++) {
      if (! isspace(line[i])) {
	break;
      }
    }
    prop_start = i;
    for (; i < line.length(); i++) {
      if (line[i] == '\\') {
	if (i == (line.length()-1)) {
	  line.erase(i);
	  break;
	}
	else {
	  line.erase(i, 1);	// remove \\ and don't test next char
	}
      }
      else if (isspace(line[i])) {
	break;
      }
    }
    prop_end = i;
    if (prop_start == prop_end)	// end of line reached
      continue;
    for (; i < line.length(); i++) {
      if (! isspace(line[i])) {
	break;
      }
    }
    val_start = i;
    for (; i < line.length(); i++) {
      if (line[i] == '\\') {
	if (i == (line.length()-1)) {
	  line.erase(i);
	  break;
	}
	else {
	  line.erase(i, 1);	// remove \\ and don't test next char
	}
      }
      else if (isspace(line[i])) {
	break;
      }
    }
    val_end = i;
    string prop(line.substr(prop_start, prop_end));
    string val(line.substr(val_start, val_end));
    if (prop == "begin") {
      // do nothing
    }
    else if (prop == "end") {
      add(item);
      item = RecordItem();
    }
    else if (prop == "weight") {
      item.weight = val;
    }
    else if (prop == "color") {
      item.color = val;
    }
    else if (prop == "name") {
      item.name = val;
    }
    else if (prop == "ramp") {
      item.ramp = str_to_ramp(val);
    }
    else if (prop == "color_smooth") {
      item.color_smooth = str_to_bool(val);
    }
    else if (prop == "color_min") {
      item.color_min = str_to_float(val);
    }
    else if (prop == "color_range") {
      item.color_range = str_to_float(val);
    }
    else if (prop == "order") {
      item.order = val;
    }
    else if (prop == "layout") {
      item.layout = str_to_layout(val);
    }
    else {
      std::cerr << "Unrecognized prop " << prop
		<< " with value " << val
		<< " in record file\n";
    }
  }
}

void
Recorder::load(const string& fname)
{
  std::ifstream in(fname.c_str());
  load(in);
}

void
Recorder::save(std::ostream& out)
{
  for (int i = 0; i < size(); i++) {
    const RecordItem& item = super_vector::at(i);
    out << "begin: " << i << std::endl;
    out << "weight: " << item.weight << std::endl;
    out << "color: " << item.color << std::endl;
    out << "name: " << item.name << std::endl;
    out << "ramp: " << ramp_to_str(item.ramp) << std::endl;
    out << "color_smooth: " << bool_to_str(item.color_smooth) << std::endl;
    out << "color_min: " << float_to_str(item.color_min) << std::endl;
    out << "color_range: " << float_to_str(item.color_range) << std::endl;
    out << "order: " << item.order << std::endl;
    out << "layout: " << layout_to_str(item.layout) << std::endl;
    out << "end: " << i << std::endl;
  }
}

void
Recorder::save(const string& fname)
{
  std::ofstream out(fname.c_str());
  save(out);
}

  // BoundedRange
float
Recorder::min() const { return 0; }

float
Recorder::max() const { return size(); }

float
Recorder::value() const { return current_; }

float
Recorder::range() const { return 1; }

  // AbstractBoundedRangeObservable
const BoundedRange *
Recorder::getBoundedRange() const
{
  return this;
}

void
Recorder::notifyValueDragged(float v)
{
  if (v < 0 || v > size())
    return;
  current_ = int(v);
}


} // namespace infovis

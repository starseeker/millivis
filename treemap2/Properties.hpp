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
#ifndef TREEMAP2_PROPERTIES_HPP
#define TREEMAP2_PROPERTIES_HPP

#include <infovis/drawing/drawing.hpp>

#include <map>
#include <string>
#include <iostream>

using std::string;

namespace infovis {

class Font;

class Properties : public std::map<string,string>
{
public:
  typedef std::map<string,string> super;
  Properties() : defaults(0) { }
  Properties(const Properties * defs) : defaults(defs) { }

  string set(const string& key, const string& value) {
    string ret = get(key);
    (*this)[key] = value;
    return ret;
  }
  const_iterator find_value(const string& key) const {
    for (const Properties * prop = this; prop != 0; prop = prop->defaults) {
      const_iterator i = prop->find(key);
      if (i != prop->end())
	return i;
    }
    return end();
  }
  string get(const string& key) const {
    const_iterator i = find_value(key);
    if (i != end())
      return i->second;
    return "";
  }
  string get(const string& key, const string& def) const {
    const_iterator i = find_value(key);
    if (i != end())
      return i->second;
    return def;
  }
  int get_int(const string& key, int def = 0) const;
  double get_double(const string& key, double def = 0) const;
  Font * get_font(const string& key, Font * def = 0) const;
  Color get_color(const string& key, const Color& def) const;
  void load(std::istream& in);
  void load(const string& fname);
  void save(std::ostream& out);
  void save(const string& fname);
  static Properties * instance();
  static void load();
  static void save();

protected:
  const Properties * defaults;
  static Properties * instance_;
};

} // namespace infovis

#endif // TREEMAP2_PROPERTIES_HPP

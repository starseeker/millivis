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
#ifndef FileType_hpp
#define FileType_hpp

#include <infovis/alloc.hpp>
#include <map>
#include <string>
#include <cstring>
#include <strings.h> // TODO: Added for strncasecmp - C++17 modernization
using std::string;

struct string_nocase_less {
  bool operator () (const std::string& s1, const std::string& s2) const {
    int min_sz = s1.size() < s2.size() ? s1.size() : s2.size();
    // TODO: Replaced strnicmp with strncasecmp for Unix compatibility - C++17 modernization
    int ret = strncasecmp(s1.c_str(), s2.c_str(), min_sz);
    if (ret < 0) return true;
    if (ret > 0) return false;
    return s1.size() < s2.size();
  }
};

struct string_nocase_eq {
  bool operator() (const std::string& s1, const std::string& s2) const {
    if (s1.size() != s2.size())
      return false;
    // TODO: Replaced strnicmp with strncasecmp for Unix compatibility - C++17 modernization
    return strncasecmp(s1.c_str(), s2.c_str(), s1.size()) == 0;
  }
};

class FileType {
public:
  FileType();
  ~FileType();

  enum {
    type_unknown,
    type_compressed,
    type_directory,
  };
  struct Type {
    bool is_compressed:1;
    bool is_directory:1;
    int code:30;

    Type(bool comp, bool dir, int c)
      : is_compressed(comp), is_directory(dir), code(c) {}
    int getCode() const { return code; }
    bool isCompressed() const { return is_compressed; }
    bool isDirectory() const { return is_directory; }
  };
  void setCode(const string& ext, int code);
  int getCode(const string& ext) const;

  int extensionCount() const;
  const string& getExtension(int index) const;

  Type fileType(const string& filename) const;

  void load(const string& filename);
  void save(const string& filename) const;
protected:
  using Map = std::map<string, int, string_nocase_less>;
  Map extmap;
};

#endif

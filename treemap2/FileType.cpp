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
#include "FileType.hpp"
#include <algorithm>
#include <fstream>
#include <cstring>

FileType::FileType() {}
FileType::~FileType() { }

void
FileType::setCode(const string& ext, int code)
{
  extmap[ext] = code;
}

int
FileType::getCode(const string& ext) const
{
  Map::const_iterator i = extmap.find(ext);
  if (i == extmap.end())
    return type_unknown;
  return i->second;
}

int
FileType::extensionCount() const
{
  return extmap.size();
}

const string&
FileType::getExtension(int index) const
{
  Map::const_iterator i = extmap.begin();
  std::advance(i, index);
  return i->first;

}

FileType::Type
FileType::fileType(const string& filename) const
{
  if (filename.empty())
    return Type(false, false,type_unknown);
  if (*filename.rbegin() == '/')
    return Type(false, true, type_directory);
  string::size_type p = filename.find_last_of(".");
  if (p == string::npos)
    p = string::size_type(-1);
  Map::const_iterator i = extmap.find(filename.substr(p+1));
  if (i == extmap.end())
    return Type(false, false, type_unknown);
  int code = i->second;
  if (code == type_directory)
    return Type(false, true, code);
  else if (code != type_compressed)
    return Type(false, false, code);
  string::size_type p2 = filename.find_last_of(".", p-1);
  if (p2 == string::npos)
    return Type(false, false, code);
  i = extmap.find(filename.substr(p2+1, p - p2 - 1));
  if (i == extmap.end())
    return Type(false, false, code);
  return Type(false, true, i->second);
}

void
FileType::save(const string& filename) const
{
  std::ofstream out(filename.c_str());
  
  for (Map::const_iterator i = extmap.begin(); i != extmap.end(); i++)
    out << i->first << '\t' << i->second << std::endl;
}

void
FileType::load(const string& filename)
{
  std::ifstream in(filename.c_str());

  while (in) {
    string ext;
    int num;
    in >> ext >> num;
    setCode(ext, num);
  }
}

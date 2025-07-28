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
#include <infovis/table/csv_loader.hpp>
#include <iostream>
#include <fstream>
#include <string>

namespace infovis {

using std::string;
using std::istream;

static void
skip_to_eol(istream& in)
{
  while (in) {
    char c = in.get();
    if (c == '\n')
      break;
  }
}

static void
ignore_line(istream& in)
{
  while (in.peek() == '#')
    skip_to_eol(in);
  skip_to_eol(in);
}

static string
read_to_eol(istream& in)
{
  char line[1024];

  while (in.peek() == '#')
    skip_to_eol(in);
  in.getline(line, 1024, '\n');
  return string(line, in.gcount());
}

static int
read_int(istream& in)
{
  int res;
  while (in.peek() == '#')
    skip_to_eol(in);
  in >> res;
  skip_to_eol(in);
  return res;
}

static bool
read_next_field(istream& in, char * line, int size)
{
  while (in.peek() == '#')
    skip_to_eol(in);
  while (in && size-- != 0) {
    char c = in.get();
    if (c == '\n' || c == ',') {
      *line++ = 0;
      return true;
    }
    else
      *line++ = c;
  }
  return false;
}

static column *
read_float_column(istream& in, int n)
{
  char line[1024];
  if (! read_next_field(in, line, 1024))
    return 0;
  
  FloatColumn * col = new FloatColumn(line);
  
  for (int i = 0; i < n; i++) {
    read_next_field(in, line, 1024);
    col->add_value(line);
  }
  return col;
}

bool tqd_table(const std::string& filename, table& t)
{
  std::ifstream in(filename.c_str());

  if (! in)
    return false;

  string title = read_to_eol(in);
  string static_attr = read_to_eol(in);
  string dynamic_attr = read_to_eol(in);
  int time_points = read_int(in);
  int records = read_int(in);

  ignore_line(in);		// ignore labels

  for (int i = 0; i < records; i++) {
    column * flt = read_float_column(in, time_points);
    if (flt != 0)
      t.add_column(flt);
  }

  return true;
}

} // namespace infovis

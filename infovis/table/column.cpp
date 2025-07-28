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
#include <infovis/table/column.hpp>

namespace infovis {

column::column(const string& name)
  : name_(name),
    defined_()
{ }

column::column(const column& other)
  : name_(other.name_),
    defined_(other.defined_)
{ }


void
column::print(std::ostream& out) const
{
  out << name_ <<  "={" << std::endl;
  for (unsigned int i = 0; i < size(); i++) {
    out << get_value(i) << std::endl;
  }
  out << '}' << std::endl;
}

bool
column::read(std::istream& in)
{
  char buffer[1024];
  int i;

  in.getline(buffer, 1024);
  for (i = 0; buffer[i] != 0; i++) {
    if (buffer[i] == '=' &&
	buffer[i+1] == '{') {
      name_ = string(buffer, i);
      break;
    }
  }
  if (buffer[i] == 0)
    return false;

  while (in) {
    in.getline(buffer, 1024);
    if (buffer[0] == '}')
      break;
    add_value(buffer);
  }
  return true;
}


} // namespace infovis

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
#include <infovis/table/table.hpp>
#include <infovis/table/column.hpp>
#include <boost/config.hpp>
#include <stdexcept>
#include <algorithm>
#undef min
#undef max

namespace infovis {

table::table()
{ }

table::table(const table& other)
{
  for (int i = 0; i < other.column_count(); i++) {
    add_column(other.get_column(i)->clone());
  }
}

table::~table()
{ }

column *
table::get_column(unsigned int index) const
{
  if (index < column_.size())
    return column_[index];
  else
    return 0;
}

void
table::set_column(unsigned int index, column * c)
{
  if (index > column_.size())
    throw std::out_of_range("column out of range");
  column * old = column_[index];
  column_[index] = c;
  //delete old;
}

void
table::add_column(column * c)
{
  column_.push_back(c);
  c->resize(row_count());
}

void
table::remove_column(unsigned int index)
{
  column * c = get_column(index);
  column_.erase(column_.begin()+index);
  //delete c;
}

int
table::index_of(const string& name) const
{
  for (unsigned int i = 0; i < column_count(); i++) {
    if (get_column(i)->get_name() == name)
      return i;
  }
  return -1;
}

column *
table::find_column(const string& name) const
{
  for (unsigned int i = 0; i < column_count(); i++) {
    if (get_column(i)->get_name() == name)
      return get_column(i);
  }
  return 0;
}

unsigned int
table::column_count() const
{
  return column_.size();
}

unsigned int
table::row_count() const
{
  int c = 0;
  for (unsigned i = 0; i < column_count(); i++) {
    c = std::max(c, int(get_column(i)->size()));
  }
  return c;
}

struct name_cmp {
  bool operator () (const column * c1, const column * c2) const {
    return c1->get_name() < c2->get_name();
  }
};

void
table::sort_columns()
{
  std::sort(column_.begin(), column_.end(), name_cmp());
}

void
table::resize(unsigned sz)
{
  for (unsigned i = 0; i < column_count(); i++) {
    get_column(i)->resize(sz);
  }
}

void
table::reserve(unsigned sz)
{
  for (unsigned i = 0; i < column_count(); i++) {
    get_column(i)->reserve(sz);
  }
}

bool
table::defined(unsigned int index) const
{
  for (unsigned i = 0; i < column_count(); i++) {
    if (! get_column(i)->defined(index))
      return false;
  }
  return true;
}

void
table::clear()
{
  column_.clear();
}

void
table::print(std::ostream& out) const
{
  for (unsigned int i = 0; i < column_count(); i++) {
    column * c = get_column(i);
    if (i != 0)
      out << "; ";
    out << c->get_name();
  }
  out << std::endl;
  bool finished = false;
  for (unsigned int j = 0; ! finished; j++) {
    finished = true;
    for (unsigned int i = 0; i < column_count(); i++) {
      column * c = get_column(i);
      if (i != 0)
	out << "; ";
      if (j < c->size()) {
	if (c->defined(j))
	  out << c->get_value(j);
	if ((j+1) != c->size())
	  finished = false;

      }
    }
    out << std::endl;
  }
}

void
table::read(std::istream& )
{
}

} // namespace infovis

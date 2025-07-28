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
#include <iostream>

using namespace infovis;

int main()
{
  IntColumn c("int");
  FloatColumn f("float");

  c.add_value("10");
  c.add_value("20");
  c.add_value("15");

  f.add_value("10.2");
  f.add_value("20.3");
  f.add_value("14.3");

  std::cout << "c=" << c;
  std::cout << "min(c) = " << c.get_min() << std::endl;
  std::cout << "max(c) = " << c.get_max() << std::endl;
  std::cout << "f=" << f;
  std::cout << "min(f) = " << f.get_min() << std::endl;
  std::cout << "max(f) = " << f.get_max() << std::endl;

  std::cout << "resizing c to 10\n";
  c.resize(10);
  for (int i = 0; i < c.size(); i++) {
    std::cout << "c.defined(" << i << ") = " <<
      (c.defined(i) ? "yes" : "no" );
    if (c.defined(i))
      std::cout << " = " << c[i];
    std::cout << std::endl;
  }
  
  return 0;
}

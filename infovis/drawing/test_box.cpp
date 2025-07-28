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
#include <infovis/drawing/box_concepts.hpp>
#include <infovis/drawing/box_io.hpp>
#include <iostream>

using namespace infovis;


#define check(expr) std::cout << "\"" << #expr << "\"\n\t=" << (expr) << std::endl

int main(int argc, char * argv[])
{
  box_min_extent<int,unsigned> b3(1, 2, 1, 2);
  boost::function_requires<BoxConcept<box_min_extent<int, int> > >();
  check(b3);
  
  box_min_max<float> b1(b3), b2(0, 0, 100, 100);
  boost::function_requires<BoxConcept<box_min_max<float> > >();

  check(b1);
  check(b2);
  check(b1 = box_min_max<float>(20, 20, 110, 110));
  check(intersects(b1,b2));

  return 0;
}

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
#include <infovis/tree/xml_tree.hpp>
#include <infovis/tree/export_tree_xml.hpp>
#include <infovis/tree/sum_weight_visitor.hpp>
#include <iostream>

using namespace infovis;

int main(int argc, char * argv[])
{
  tree t;

  if (argc < 2) {
    std::cerr << "syntax: " << argv[0] << " <xml-file>\n";
    return 1;
  }
  std::cout << "Loading " << argv[1] << std::endl;
  xml_tree(argv[1], t);

  std::cout << "loaded\n";
  column * c = t.find_column("length");
  if (c != 0 && t.find_column("size") == 0) {
    c->set_name("size");
    FloatColumn * size = FloatColumn::cast(c);
    if (size != 0)
      sum_weights(t, *size);
  }

  std::cout << "exporting into dir.xml\n";
  export_tree_xml("dir.xml", t);
  return 0;
}

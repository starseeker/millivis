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
#include <infovis/tree/tree.hpp>
#include <expat.h>
#include <zlib.h>
#include <vector>
#include <cstdlib>

namespace infovis {
typedef tree Tree;
typedef tree_traits<Tree>::node_descriptor node_descriptor;

struct xml_tree_builder
{
  Tree& tree_;
  StringColumn * tag_;
  std::vector<node_descriptor> node_stack;

  static void startElement(void *userData,
			   const char *name, const char **atts) {
    xml_tree_builder * builder = static_cast<xml_tree_builder*>(userData);
    builder->start(name, atts);
  }
  static void endElement(void *userData, const char *name) {
    xml_tree_builder * builder = static_cast<xml_tree_builder*>(userData);
    builder->end(name);
  }

  node_descriptor current() const {
    return node_stack.back();
  }
  node_descriptor push() {
    node_descriptor n = add_node(current(), tree_);
    node_stack.push_back(n);
    return n;
  }
  void pop() {
    node_stack.pop_back();
  }
  void start(const char *name, const char **atts) {
    StringColumn * str;
    FloatColumn  * flt;
    node_descriptor n = push();
    (*tag_)[n] = name;
    for (const char ** a = atts; *a != 0; a += 2) {
      column * c = tree_.find_column(*a);
      if (c == 0) {
	char * end = const_cast<char*>(a[1]);
	float v = strtod(end, &end);
	if (end == a[1] || *end != 0) { // not a float
	  str = new StringColumn(a[0]);
	  tree_.add_column(str);
	  (*str)[n] = a[1];
	}
	else {
	  flt = new FloatColumn(a[0]);
	  tree_.add_column(flt);
	  (*flt)[n] = v;
	}
      }
      else if ((flt = FloatColumn::cast(c)) != 0) {
	float v = strtod(a[1], 0);
	(*flt)[n] = v;
      }
      else if ((str = StringColumn::cast(c)) != 0) {
	(*str)[n] = a[1];
      }
    }
  }
  void end(const char *name) {
    pop();
  }

  void build(const std::string& filename) {
    char buf[1024];
    gzFile input;
    const char * fname = filename.c_str();

    input = gzopen(fname, "rb");
    if (input == NULL) {
      return;
    }
    node_stack.push_back(root(tree_));
    XML_Parser parser = XML_ParserCreate(NULL);
    int done;
    XML_SetUserData(parser, this);
    XML_SetElementHandler(parser, startElement, endElement);
    do {
      size_t len = gzread(input, buf, sizeof(buf));
      done = len < sizeof(buf);
      if (!XML_Parse(parser, buf, len, done)) {
	fprintf(stderr,
		"%s at line %d\n",
		XML_ErrorString(XML_GetErrorCode(parser)),
		XML_GetCurrentLineNumber(parser));
	done = 1;
      }
    } while (!done);
    gzclose(input);
    XML_ParserFree(parser);
    pop();
  }
  xml_tree_builder(Tree& t, StringColumn * n)
    : tree_(t), tag_(n) { }
};

unsigned xml_tree(const std::string& filename, tree& t)
{
  StringColumn * tag = StringColumn::find("tag", t);

  (*tag)[root(t)] = filename;
  xml_tree_builder builder(t, tag);
  builder.build(filename);
  return num_nodes(t);
}

} // namespace infovis 

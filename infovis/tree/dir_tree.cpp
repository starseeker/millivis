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
#include <infovis/table/metadata.hpp>
#include <infovis/tree/sum_weight_visitor.hpp>
#include <boost/directory.h>
#include <sys/stat.h>
#include <iostream>
#include <list>

namespace infovis {

typedef tree Tree;
typedef tree_traits<Tree>::node_descriptor node_descriptor;

struct dir_tree_builder
{
  Tree& tree_;
  StringColumn * name_;
  FloatColumn * size_;
  FloatColumn * type_;
  FloatColumn * mtime_;
  FloatColumn * atime_;
  FloatColumn * ctime_;
    

  unsigned build(node_descriptor parent, const std::string& dirname) {
    using namespace boost::filesystem;
    unsigned ret = 0;
    typedef std::list<std::string>  DirQueue;
    DirQueue dir_queue;
    typedef std::list<node_descriptor> ParentQueue;
    ParentQueue parent_queue;
    

    dir_queue.push_front(dirname);
    parent_queue.push_front(parent);

    while (! dir_queue.empty()) {
      std::string dirname = dir_queue.front();
      dir_queue.pop_front();
      parent = parent_queue.front();
      parent_queue.pop_front();
      std::cerr << "Reading dir " << dirname << std::endl;
      for (dir_it it(dirname); it != dir_it(); ++it) {
	if ((*it)[0] == '.')
	  continue;
	ret++;
	node_descriptor n = add_node(parent, tree_);
	size_->set(n, boost::filesystem::get<size>(it));
	type_->set(n, boost::filesystem::get<is_directory>(it) ? 1 : 0);
	struct stat s;
	stat((dirname+(*it)).c_str(), &s);

	mtime_->set(n, s.st_mtime);
	ctime_->set(n, s.st_ctime);
	atime_->set(n, s.st_atime);

	if (boost::filesystem::get<is_directory>(it)) {
	  std::string name = *it + "/";
	  name_->set(n, name);
	  //ret += build(n, dirname+name);
	  dir_queue.push_front(dirname+name);
	  parent_queue.push_front(n);
	}
	else
	  name_->set(n, *it);
      }
    }
    return ret;
  }
  dir_tree_builder(Tree& t,
		   StringColumn * n,
		   FloatColumn * s,
		   FloatColumn * tp,
		   FloatColumn * m,
		   FloatColumn * c,
		   FloatColumn * a)
    : tree_(t), name_(n), size_(s), type_(tp),
      mtime_(m), ctime_(c), atime_(a) { }
};

unsigned dir_tree(const std::string& dirname, Tree& t)
{
  StringColumn * name = StringColumn::find("name", t);
  name->put_metadata(metadata::type, metadata::type_nominal);
  FloatColumn * size = FloatColumn::find("size", t);
  size->put_metadata(metadata::type, metadata::type_ordinal);
  size->put_metadata("aggregate", "sum");
  FloatColumn * type = FloatColumn::find("type", t);
  type->put_metadata(metadata::type, metadata::type_categorical);
  FloatColumn * mtime = FloatColumn::find("mtime", t);
  mtime->put_metadata(metadata::type, metadata::type_ordinal);
  mtime->put_metadata(metadata::user_type, metadata::user_type_unix_time);
  FloatColumn * atime = FloatColumn::find("atime", t);
  atime->put_metadata(metadata::type, metadata::type_ordinal);
  atime->put_metadata(metadata::user_type, metadata::user_type_unix_time);
  FloatColumn * ctime = FloatColumn::find("ctime", t);
  ctime->put_metadata(metadata::type, metadata::type_ordinal);
  ctime->put_metadata(metadata::user_type, metadata::user_type_unix_time);

  std::string dname(dirname);
  if (*dname.rbegin() != '/')
    dname.append("/");
  struct stat s;
  stat(dirname.c_str(), &s);
  name->set(root(t),dname);
  size->set(root(t),s.st_size);
  type->set(root(t),((s.st_mode&S_IFDIR) != 0));
  mtime->set(root(t),s.st_mtime);
  ctime->set(root(t),s.st_ctime);
  atime->set(root(t),s.st_atime);
  dir_tree_builder builder(t, name, size, type, mtime, atime, ctime);
  unsigned n= builder.build(root(t), dname);
  sum_weights(t, *size);
  return n;
}


} // namespace infovis

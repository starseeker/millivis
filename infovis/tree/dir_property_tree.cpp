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
#include <infovis/tree/property_tree.hpp>
// TODO: Replaced boost/directory.h with std::filesystem - C++17 modernization
#include <filesystem>
#include <sys/stat.h>

namespace infovis {

typedef property_tree Tree;
typedef tree_traits<Tree>::node_descriptor node_descriptor;
typedef Tree::prop_id prop_id;

struct dir_property_tree_builder {
  Tree& tree_;
  Tree::prop_id name_;
  Tree::prop_id size_;
  Tree::prop_id type_;
  Tree::prop_id mtime_;
  Tree::prop_id atime_;
  Tree::prop_id ctime_;
    
  unsigned build(node_descriptor parent, const std::string& dirname) {
    // TODO: Replaced boost::filesystem with std::filesystem - C++17 modernization
    unsigned ret = 0;

    if (!std::filesystem::exists(dirname)) return ret;
    
    for (const auto& entry : std::filesystem::directory_iterator(dirname)) {
      std::string filename = entry.path().filename().string();
      if (filename[0] == '.')
	continue;
      ret++;
      node_descriptor n = add_node(parent, tree_);
      
      std::error_code ec;
      auto file_size = std::filesystem::file_size(entry.path(), ec);
      tree_.get_prop_numeric(size_)[n] = ec ? 0 : static_cast<long>(file_size);
      
      tree_.get_prop_numeric(type_)[n] = entry.is_directory() ? 1 : 0;
      struct stat s;
      stat(entry.path().c_str(), &s);

      tree_.get_prop_numeric(mtime_)[n] = s.st_mtime;
      tree_.get_prop_numeric(ctime_)[n] = s.st_ctime;
      tree_.get_prop_numeric(atime_)[n] = s.st_atime;

      if (entry.is_directory()) {
	std::string name = filename + "/";
	tree_.get_prop_string(name_)[n] = name;
	ret += build(n, entry.path().string() + "/");
      }
      else
	tree_.get_prop_string(name_)[n] = filename;
    }
    return ret;
  }
  dir_property_tree_builder(Tree& t,
			    Tree::prop_id n,
			    Tree::prop_id s,
			    Tree::prop_id tp,
			    Tree::prop_id m,
			    Tree::prop_id c,
			    Tree::prop_id a)
    : tree_(t), name_(n), size_(s), type_(tp),
      mtime_(m), ctime_(c), atime_(a) { }
};

unsigned dir_property_tree(const std::string& dirname,
			   property_tree& t)
{
  Tree::prop_id name = t.add_property("name", Tree::type_string);
  Tree::prop_id size = t.add_property("size", Tree::type_numeric);
  Tree::prop_id type = t.add_property("type", Tree::type_numeric);
  Tree::prop_id mtime = t.add_property("mtime", Tree::type_numeric);
  Tree::prop_id atime = t.add_property("atime", Tree::type_numeric);
  Tree::prop_id ctime = t.add_property("ctime", Tree::type_numeric);
    
  if (num_nodes(t) == 0)
    add_node(root(t), t);
  std::string dname(dirname);
  if (*dname.rbegin() != '/')
    dname.append("/");
  struct stat s;
  stat(dirname.c_str(), &s);
  t.get_prop_string(name)[root(t)] = dname;
  t.get_prop_numeric(size)[root(t)] = s.st_size;
  t.get_prop_numeric(type)[root(t)] = ((s.st_mode&S_IFDIR) != 0);
  t.get_prop_numeric(mtime)[root(t)] = s.st_mtime;
  t.get_prop_numeric(ctime)[root(t)] = s.st_ctime;
  t.get_prop_numeric(atime)[root(t)] = s.st_atime;
  dir_property_tree_builder builder(t, name, size, type, mtime, atime, ctime);
  return builder.build(root(t), dname);
}


} // namespace infovis

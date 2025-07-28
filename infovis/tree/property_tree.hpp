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
#ifndef INFOVIS_TREE_PROPERTY_TREE_HPP
#define INFOVIS_TREE_PROPERTY_TREE_HPP

#include <infovis/alloc.hpp>
#include <infovis/tree/vector_as_tree.hpp>
//#include <infovis/tree/properties.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <cassert>

namespace infovis {

class property_tree : public vector_as_tree
{
public:
  typedef vector_as_tree super;
  enum prop_type {
    type_invalid,
    type_numeric,
    type_string
  };
  struct prop_id {
    unsigned type_ : 2;
    int index_ : 30;

    prop_id() : type_(type_invalid) { }
    explicit prop_id(prop_type t, int i = -1) : type_(t), index_(i) { }
    bool is_numeric() const { return type_ == type_numeric; }
    bool is_string() const {  return type_ == type_string; }
    bool is_invalid() const { return type_ == type_invalid; }
    prop_type type() const { return prop_type(type_); }
    unsigned index() const { return index_; }
    bool operator == (const prop_id& other) const {
      return type_ == other.type_ && index_ == other.index_;
    }
  };
  typedef std::vector<float,gc_alloc<float,true> > numeric_prop;
  typedef std::vector<std::string,gc_alloc<std::string> > string_prop;
  typedef std::vector<prop_id,gc_alloc<prop_id, true> > id_prop;
  typedef string_prop::const_iterator names_iterator;
  typedef id_prop::const_iterator ids_iterator;

  property_tree() : vector_as_tree() {}
  ~property_tree() { clear(); }

  prop_id add_property(const std::string& name, prop_type t) {
    string_prop::iterator i = std::find(prop_names.begin(), prop_names.end(),
					name);
    if (i != prop_names.end()) {
      prop_id id = prop_ids[i - prop_names.begin()];
      if (id.type() != t)
	throw std::runtime_error("name exists with another type");
      return id;
    }
    prop_id id(t);
    if (t == type_numeric) {
      id.index_ = numeric_props.size();
      numeric_props.push_back(new numeric_prop(size()));
    }
    else if (t == type_string) {
      id.index_ = string_props.size();
      string_props.push_back(new string_prop(size()));
    }
    prop_names.push_back(name);
    prop_ids.push_back(id);
    return id;
  }
  prop_id get_prop_id(const std::string& name) const {
    string_prop::const_iterator i =
      std::find(prop_names.begin(), prop_names.end(),
		name);
    if (i == prop_names.end())
      return prop_id(type_invalid);
    return prop_ids[i - prop_names.begin()];
  }

  const string_prop& get_prop_string(unsigned index) const {
    string_prop& sp = *string_props[index];
    sp.resize(size());	// const doesn't stick to pointers
    return sp;
  }
  string_prop& get_prop_string(unsigned index) {
    string_prop& sp = *string_props[index];
    sp.resize(size());	// const doesn't stick to pointers
    return sp;
  }
  const string_prop& get_prop_string(prop_id id) const {
    assert(id.type() == type_string);
    return get_prop_string(id.index());
  }
  string_prop& get_prop_string(prop_id id) {
    assert(id.type() == type_string);
    return get_prop_string(id.index());
  }
  const string_prop& get_prop_string(const std::string& name) const {
    return get_prop_string(get_prop_id(name));
  }      
  string_prop& get_prop_string(const std::string& name) {
    return get_prop_string(get_prop_id(name));
  }      
  const numeric_prop& get_prop_numeric(unsigned index) const {
    numeric_prop& np = *numeric_props[index];
    np.resize(size());
    return np;
  }
  numeric_prop& get_prop_numeric(unsigned index) {
    numeric_prop& np = *numeric_props[index];
    np.resize(size());
    return np;
  }
  const numeric_prop& get_prop_numeric(prop_id id) const {
    assert(id.type() == type_numeric);
    return get_prop_numeric(id.index());
  }
  numeric_prop& get_prop_numeric(prop_id id) {
    assert(id.type() == type_numeric);
    return get_prop_numeric(id.index());
  }
  numeric_prop& get_prop_numeric(const std::string& name) {
    return get_prop_numeric(get_prop_id(name));
  }
  const numeric_prop& get_prop_numeric(const std::string& name) const {
    return get_prop_numeric(get_prop_id(name));
  }
  int prop_count() const { return prop_names.size(); }
  names_iterator begin_names() const { return prop_names.begin(); }
  names_iterator end_names() const { return prop_names.end(); }

  ids_iterator begin_ids() const { return prop_ids.begin(); }
  ids_iterator end_ids() const { return prop_ids.end(); }
    
  void clear() {
    erase(begin(), end());
    prop_names.erase(prop_names.begin(), prop_names.end());
    prop_ids.erase(prop_ids.begin(), prop_ids.end());
    int i;
    for (i = 0; i < numeric_props.size(); i++) {
      delete numeric_props[i];
    }
    numeric_props.erase(numeric_props.begin(), numeric_props.end());
    for (i = 0; i < string_props.size(); i++) {
      delete string_props[i];
    }
    string_props.erase(string_props.begin(), string_props.end());
  }
protected:
  string_prop prop_names;
  id_prop prop_ids;
  std::vector<numeric_prop*, gc_alloc<numeric_prop*> > numeric_props;
  std::vector<string_prop*, gc_alloc<string_prop*> > string_props;
};

template <>
struct tree_traits< property_tree > : public tree_traits< vector_as_tree >
{};

inline void clear(property_tree& prop)
{
  prop.clear();
}

inline void copy_prop(const property_tree& in,
		      property_tree::prop_id iid,
		      tree_traits<property_tree>::node_descriptor nin,
		      property_tree& out,
		      property_tree::prop_id oid,
		      tree_traits<property_tree>::node_descriptor nout)
{
  if (iid.is_numeric()) {
    out.get_prop_numeric(oid)[nout] = in.get_prop_numeric(iid)[nin];
  }
  else if (iid.is_string()) {
    out.get_prop_string(oid)[nout] = in.get_prop_string(iid)[nin];
  }
}
			

struct props_copier {
  const property_tree& in_;
  property_tree& out_;
  typedef std::vector<
    std::pair<property_tree::prop_id,property_tree::prop_id>,
    gc_alloc<std::pair<property_tree::prop_id,property_tree::prop_id>,true>
  > IdMap;
  IdMap ids_;

  props_copier(const property_tree& in, property_tree& out)
    : in_(in), out_(out) {
    for (property_tree::names_iterator n = in_.begin_names();
	 n != in_.end_names(); n++) {
      property_tree::prop_id iid = in_.get_prop_id(*n);
      ids_.push_back(std::make_pair(iid, out_.add_property(*n, iid.type())));
    }
  }
  void operator()(tree_traits<property_tree>::node_descriptor nin,
		  tree_traits<property_tree>::node_descriptor nout) {
    for (IdMap::const_iterator i = ids_.begin(); i != ids_.end(); i++) {
      copy_prop(in_, i->first, nin, out_, i->second, nout);
    }
  }
};

template <class ToProp, class UnaryFn>
void props_fill(ToProp& to, UnaryFn fn)
{
  for (int i = 0; i < to.size(); i++) {
    to[i] = fn(i);
  }
}

} // namespace infovis 

#endif // INFOVIS_TREE_PROPERTY_TREE_HPP

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
#ifndef INFOVIS_TABLE_COLUMN_HPP
#define INFOVIS_TABLE_COLUMN_HPP

#include <infovis/alloc.hpp>
#include <infovis/table/table.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <map>

namespace infovis {

using std::string;

/**
 * Base class for each column types.
 * A column is a 1-dimensional table of value, in principle stored in
 * a table for fast indexing.  Values can be undefine, i.e. because on
 * value is not provided or the data file has an unparable value at
 * that index.
 *
 * The base class manages the undefined table whereas actual data
 * storage is managed by derived classes.
 *
 * Each column can have associated metadata, in the form of names
 * associated with values.  Each name is a string and each value is
 * also a string.  Metadata are meant to specify extra information on
 * the column.  See <b>metadata</b> for examples.
 */
class column : public gc
{
public:
  /**
   * Type of the map holding the matadata.
   */
  typedef std::map<string,string> Metadata;
protected:
  string name_;			/// The name of the column
  std::vector<bool> defined_;	/// The vector of defined value
  Metadata metadata_;		/// The metadata map
protected:

  /**
   * Create a column with a name.
   * @param name the name
   */
  column(const string& name);

  /**
   * Create a column copy of another column.
   * @param other the other column to copy from.
   */
  column(const column& other);
public:

  /**
   * The copy method.
   * @param other the other column to copy from.
   */
  virtual column& operator = (const column& other) {
    name_ = other.name_;
    defined_ = other.defined_;
    return *this;
  }

  /**
   * Get the name of the column.
   * @return the name of the column
   */
  const string& get_name() const { return name_; }

  /**
   * Set the name of the column.
   * @name the name.
   */
  void set_name(const string& name) {
    name_ = name;
  }


  /**
   * Return a copy of the column.
   * @return a copy of the column
   */
  virtual column * clone() const = 0;

  /**
   * Return the size of the column.
   * @return the size of the column.
   */
  virtual unsigned size() const = 0;

  /**
   * Change the size of the column.
   * @param sz the new size.
   */
  virtual void resize(int sz) = 0;

  /**
   * Check whether the value at index if valid (defined).
   * @param index the row index
   * @return true if the value is defined
   */
  bool defined(unsigned int index) const {
    return index < defined_.size() && defined_[index];
  }

  /**
   * Undefine the value at index.
   * @param index the row index to undefine.
   */
  void undefine(unsigned int index) {
    if (index < defined_.size())
      defined_[index] = false;
  }

  /**
   * Return the metadata map.
   * @return the metadata map.
   */
  Metadata& metadata() { return metadata_; }

  /**
   * Return the metadata map.
   * @return the metadata map.
   */
  const Metadata& metadata() const { return metadata_; }


  /**
   * Check whether a metadata key is defined.
   * @param key the key
   * return whether the metadata key is defined
   */
  bool has_metadata(const string& key) const {
    return metadata_.find(key) != metadata_.end();
  }


  /**
   * Return the value associated with a metadata key.
   * @param key the key.
   * @return the value associated with a metadata key.
   */
  const string& get_metadata(const string& key) const {
    static string empty;

    Metadata::const_iterator i = metadata_.find(key);
    if (i == metadata_.end())
      return empty;
    return i->second;
  }


  /**
   * Associate a metadata value to a key.
   * @param key the key
   * @param value the value
   * @return true if the value already existed.
   */
  bool put_metadata(const string& key, const string& value) {
    std::pair<Metadata::iterator,bool> res =
      metadata_.insert(std::make_pair(key, value));
    return res.second;
  }

  /**
   * Reserve room for adding new values.
   * @param sz the room reserved.
   */
  virtual void reserve(unsigned int sz) = 0;

  /**
   * Return the capacity allocated for this column.
   * @return the capacity allocated for this column.
   */
  virtual unsigned capacity() const = 0;

  /**
   * Return the string representation of the value at index.
   * @param index the row index.
   * @return the string representation of the value at index.
   */
  virtual string get_value(unsigned int index) const = 0;

  /**
   * Set the value at index from its string representation.
   * @param index the row index
   * @param val the string representation of the value.
   */
  virtual void set_value(unsigned int index, const string& val) = 0;

  /**
   * Add a value to the column.
   * @param val the the string representation of the value.
   */
  virtual void add_value(const string& val) = 0;

  /**
   * Clear the column, emptying it.
   */
  virtual void clear() = 0;

  /**
   * Return the string representation of the minimum value contained
   * in the column.
   * @return the string representation of the minimum value contained
   * in the column.
   */
  virtual string get_min() const = 0;

  /**
   * Return the string representation of the maximum value contained
   * in the column.
   * @return the string representation of the maximum value contained
   * in the column.
   */
  virtual string get_max() const = 0;

  /**
   * Print a readable representation of the column.
   * @param out the output stream.
   */
  virtual void print(std::ostream& out) const;

  /**
   * Read a the column from a stream. 
   * @param in the input stream.
   */
  virtual bool read(std::istream& in);
};

/**
 * Shortcut for printing a column.
 */
inline std::ostream&
operator << (std::ostream& out, const column& c) {
  c.print(out);
  return out;
}

/**
 * Shortcut for reading a column.
 */
inline std::istream&
operator >> (std::istream& in, column& c) {
  c.read(in);
  return in;
}

/**
 * Template class for specializing column types.
 */
template <class T>
class column_of : public column
{
protected:
  typedef std::vector<T, gc_alloc<T,true> > List;
  List value_;
  T default_;			/// The default value.
  mutable T min_;		/// The computed minumum value.
  mutable T max_;		/// The computed maximum value.
  mutable bool min_max_valid_;

  /**
   * Lazy computation of min and max values.
   */
  void compute_min_max() const { // tricky, use mutable values
    int i;

    if (min_max_valid_)
      return;
    for (i = 0; i < value_.size(); i++) {
      if (defined(i)) {
	min_ = max_ = value_[i];
	break;
      }
    }
    if (i == value_.size())
      return;

    for (; i < value_.size(); i++) {
      if (! defined(i))
	continue;
      const T& v = value_[i];
      if (v < min_)
	min_ = v;
      else if (v > max_)
	max_ = v;
    }
    min_max_valid_ = true;
  }
public:
  typedef column_of<T> self;	///
  typedef T value_type;		///
  typedef typename List::const_iterator const_iterator; /// The iterator type.

  /**
   * Constructor of a column specialized for a data type.
   */
  explicit column_of(const string& name, int capacity = 10, const T& def = T())
    : column(name),
      default_(def),
      min_max_valid_(false) {
    defined_.reserve(capacity);
    value_.reserve(capacity);
  }

  /**
   * Copy constructor of a column specialized for a data type.
   */
  column_of(const column_of<T>& other)
    : column(other),
      value_(other.value_),
      default_(other.default_),
      min_max_valid_(false)
  { }

  virtual column * clone() const {
    return new column_of(*this);
  }

  /**
   * Copy operator.
   */
  column_of& operator = (const column_of& other) {
    column::operator = (other);
    value_ = other.value_;
    default_ = other.default_;
    return *this;
  }
  virtual unsigned size() const { return value_.size(); }
  virtual void resize(int sz) {
    defined_.resize(sz, false);
    value_.resize(sz, default_);
  }

  virtual void reserve(unsigned int sz) {
    defined_.reserve(sz);
    value_.reserve(sz);
  }
  virtual unsigned capacity() const { return value_.capacity(); }
  virtual string get_value(unsigned int index) const {
    if (! defined(index))
      return "";
    std::basic_ostringstream<char> out;
    out << value_[index];
    return out.str();
  }
  virtual void set_value(unsigned int index, const string& val) {
    if (index > size())
      return;

    std::basic_istringstream<char> sstream;
    T v;
    sstream >> v;
    set(index, v);
  }

  virtual void add_value(const string& val) {
    std::basic_istringstream<char> in(val);
    T v;
    in >> v;
    add(v);
  }

  void clear() { value_.clear(); min_max_valid_ = false; }

  virtual string get_min() const {
    compute_min_max();
    std::basic_ostringstream<char> sstream;
    sstream << min_;
    return sstream.str();
  }

  virtual string get_max() const {
    compute_min_max();
    std::basic_ostringstream<char> sstream;
    sstream << max_;
    return sstream.str();
  }

  /**
   * Fast type-specific operator for getting and setting a value.
   * @param index the index of the value
   * @return a reference to the indexed value
   */
  T& operator[] (unsigned int index) {
    if (index >= size()) {
      resize(index+1);
    }
    defined_[index] = true;
    return value_[index];
  }

  /**
   * Fast type-specific operator for getting a value.
   * @param index the index of the value
   * @return a reference to the indexed value
   */
  const T& operator[] (unsigned int index) const { return value_[index]; }

  /**
   * Get method for a value.
   * @param index the index of the value
   * @return the value if the index is valid, the default value otherwise.
   */
  const T& get(unsigned int index) const {
    if (defined(index))
      return value_[index];
    return default_;
  }

  /**
   * Fast inline method for reading the table.
   * @param index the index
   * @return the value
   */
  const T& fast_get(unsigned int index) const {
    return value_[index];
  }

  /**
   * Set the value at index.
   * @param index the index
   * @param v the value
   */
  void set(unsigned int index, const T& v) {
    if (index >= size()) {
      resize(index+1);
    }
    value_[index] = v;
    defined_[index] = true;
    min_max_valid_ = false;
  }

  /**
   * Fast inline methode for seting the value.
   * @param index the index
   * @param v the value
   */
  void fast_set(unsigned int index, const T& v) {
    value_[index] = v;
  }

  /**
   * Add a value to the column
   * @param v the value
   */
  void add(const T& v) {
    value_.push_back(v);
    defined_.push_back(true);
    if (min_max_valid_) {
      if (v < min_)
	min_ = v;
      else if (v > max_)
	max_ = v;
    }
  }

  /**
   * Return the minimum value of the column.
   * @return the minimum value of the column
   */
  const T& min() const {
    compute_min_max();
    return min_;
  }

  /**
   * Return the maximum value of the column.
   * @return the maximum value of the column
   */
  const T& max() const {
    compute_min_max();
    return max_;
  }

  /**
   * Utility static method to cast a generic column to a specific type
   * column.
   * @param c the generic column
   * @return the type specific column of null
   */
  static self * cast(column * c) {
    return dynamic_cast<self*>(c);
  }

  /**
   * Utility static method to cast a generic const column to a specific type
   * const column.
   * @param c the generic const column
   * @return the type specific const column of null
   */
  static const self * cast(const column * c) {
    return dynamic_cast<const self*>(c);
  }

  /**
   * Utility static method to find a typed column from a table,
   * creating it if it doesn't already exist.
   * @param name the name of the desired column
   * @param tab the table to search into
   * @return a specific type column or null if a column of the
   * required name exists with a different type
   */
  static self * find(const string& name, table& tab) {
    self * ret;
    column * col = tab.find_column(name);

    if (col == 0) {
      ret = new self(name);
      tab.add_column(ret);
    }
    else {
      ret = cast(col);
    }
    return ret;
  }


  /**
   * Const iterator to the begining of the values
   */
  const_iterator begin() const { return value_.begin(); }

  /**
   * Const iterator to the end of the values
   */
  const_iterator end() const { return value_.end(); }
};
typedef column_of<bool> BoolColumn; /// column type for bool values
typedef column_of<char> CharColumn; /// column type for char value
typedef column_of<unsigned> UnsignedColumn; /// column type for unsigned value
typedef column_of<int> IntColumn; /// column type for int value
typedef column_of<long> LongColumn; /// column type for long value
typedef column_of<float> FloatColumn; /// column type for float value
typedef column_of<double> DoubleColumn; /// column type for double value
typedef column_of<string> StringColumn; /// column type for string value

template <class T, class V>
inline void put(column_of<T>& pa, int k, const V& val) { pa[k] = val;  }

template <class T>
inline const T& get(const column_of<T>& pa, int k) { return pa[k]; }

/**
 * Fills a column from the result of a unary function.
 * @param to te column
 * @param fn the unary function
 */
template <class ToProp, class UnaryFn>
void fill_column(ToProp& to, UnaryFn fn)
{
  for (int i = 0; i < to.size(); i++) {
    to[i] = fn(i);
  }
}

} // namespace infovis

#endif // INFOVIS_TABLE_COLUMN_HPP

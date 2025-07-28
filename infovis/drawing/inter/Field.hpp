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
#ifndef INFOVIS_DRAWING_FIELD_HPP
#define INFOVIS_DRAWING_FIELD_HPP

#include <infovis/drawing/drawing.hpp>
#include <map>
#include <vector>

namespace infovis {

class Field : public gc
{
public:
  enum Type {
    type_void,
    type_bool,
    type_int,
    type_float,
    type_double,
    type_string,
    type_composite
  };
  Field(const string& name);
  Field(const Field& other);

  virtual Field * clone() const;
  virtual Type getType() const;

  const string& getName() const { return name_; }
  Field * getParent() { return parent_; }
  const Field * getParent() const { return parent_; }

  long getTimestamp() const { return timestamp_; }

  virtual void addField(Field * val);
  
  virtual bool getBoolValue() const;
  virtual void setBoolValue(bool v);
  virtual int getIntValue() const;
  virtual void setIntValue(int v);
  virtual float getFloatValue() const;
  virtual void setFloatValue(float v);
  virtual double getDoubleValue() const;
  virtual void setDoubleValue(double v);
  virtual const string& getStringValue() const;
  virtual void setStringValue(const string& v);

  virtual int fieldCount() const;

  virtual bool hasField(const string& field) const;
  virtual bool hasField(int ) const;

  virtual Field& getField(const string& field);
  virtual const Field& getField(const string& field) const;

  virtual Field& getField(int field);
  virtual const Field& getField(int field) const;

  const Field& operator[] (const string& field) const {
    return getField(field);
  }
  Field& operator[] (const string& field) {
    return getField(field);
  }
  const Field& operator[] (int field) const {
    return getField(field);
  }
  Field& operator[] (int field) {
    return getField(field);
  }

  virtual void clear();

  static const Field nullField;
  bool isModified() const { return timestamp_ == global_timestamp_; }

  static void nextGlobalTimeStamp() { global_timestamp_++; }
  static long getGlobalTimeStamp() { return global_timestamp_; }
protected:
  void fieldAdded(Field * field) {
    field->parent_ = this;
  }
  void updateTimeStamp() {
    timestamp_ = global_timestamp_;
    if (parent_ != 0)
      parent_->updateTimeStamp();
  }

  string name_;
  Field * parent_;
  long timestamp_;

  static long global_timestamp_;
};

class FieldComposite : public Field
{
public:
  FieldComposite(const string& name);
  FieldComposite(const FieldComposite& other);

  virtual Field * clone() const;
  virtual Type getType() const;

  virtual void addField(Field * val);

  virtual int fieldCount() const;
  virtual bool hasField(const string& field) const;
  virtual bool hasField(int ) const;

  virtual Field& getField(const string& field);
  virtual const Field& getField(const string& field) const;
  virtual Field& getField(int field);
  virtual const Field& getField(int field) const;

  virtual void clear();
protected:
  typedef std::map<string,Field*> Map;
  typedef std::vector<Field*> Vector;

  Map map_;
  Vector array_;
};

class FieldBool : public Field
{
public:
  FieldBool(const string& name, bool value = false);
  FieldBool(const FieldBool& other);

  virtual Field * clone() const;
  virtual Type getType() const;
  virtual bool getBoolValue() const;
  virtual void setBoolValue(bool v);
protected:
  bool value_;
};

class FieldInt : public Field
{
public:
  FieldInt(const string& name, int value = 0);
  FieldInt(const FieldInt& other);

  virtual Field * clone() const;
  virtual Type getType() const;
  virtual int getIntValue() const;
  virtual void setIntValue(int v);
protected:
  int value_;
};

class FieldFloat : public Field
{
public:
  FieldFloat(const string& name, float value = 0.0f);
  FieldFloat(const FieldFloat& other);

  virtual Field * clone() const;
  virtual Type getType() const;
  virtual float getFloatValue() const;
  virtual void setFloatValue(float v);
protected:
  float value_;
};

class FieldDouble : public Field
{
public:
  FieldDouble(const string& name, double value = 0.0);
  FieldDouble(const FieldDouble& other);

  virtual Field * clone() const;
  virtual Type getType() const;
  virtual double getDoubleValue() const;
  virtual void setDoubleValue(double v);
protected:
  double value_;
};

class FieldString : public Field
{
public:
  FieldString(const string& name, const string& value = string());
  FieldString(const FieldString& other);

  virtual Field * clone() const;
  virtual Type getType() const;
  virtual const string& getStringValue() const;
  virtual void setStringValue(const string& v);
protected:
  string value_;
};

} // namespace infovis

#endif // INFOVIS_DRAWING_FIELD_HPP

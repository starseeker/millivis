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
#include <infovis/drawing/inter/Field.hpp>
#include <stdexcept>

namespace infovis {

const Field Field::nullField("null");
long Field::global_timestamp_ = 0;

Field::Field(const string& name)
  : name_(name),
    parent_(0),
    timestamp_(0)
{ }

Field::Field(const Field& other)
  : name_(other.name_),
    parent_(0),
    timestamp_(other.timestamp_)
{ }

Field *
Field::clone() const
{
  return new Field(*this);
}

Field::Type
Field::getType() const
{
  return type_void;
}

void
Field::addField(Field * val)
{
  throw std::runtime_error("Field is not composite");
}

bool
Field::getBoolValue() const
{
  throw std::runtime_error("Field is not a bool");
}

void
Field::setBoolValue(bool)
{
  throw std::runtime_error("Field is not a bool");
}

int
Field::getIntValue() const
{
  throw std::runtime_error("Field is not an int");
}

void
Field::setIntValue(int v)
{
  setFloatValue(v);
}

float
Field::getFloatValue() const
{
  return getIntValue();
}

void
Field::setFloatValue(float v)
{
  setDoubleValue(v);
}

double
Field::getDoubleValue() const
{
  return getFloatValue();
}

void
Field::setDoubleValue(double v)
{
  throw std::runtime_error("Field is not a double");  
}

const string&
Field::getStringValue() const
{
  throw std::runtime_error("Field is not a string");  
}

void
Field::setStringValue(const string& v)
{
  throw std::runtime_error("Field is not a string");  
}

int
Field::fieldCount() const
{
  throw std::runtime_error("Field is not composite");
}

bool
Field::hasField(const string& field) const
{
  return false;
}

bool
Field::hasField(int) const
{
  return false;
}

Field&
Field::getField(const string& field)
{
  throw std::runtime_error("Field is not composite");
}

const Field&
Field::getField(const string& field) const
{
  throw std::runtime_error("Field is not composite");
}

Field&
Field::getField(int field)
{
  throw std::runtime_error("Field is not composite");
}

const Field&
Field::getField(int field) const
{
  throw std::runtime_error("Field is not composite");
}

void
Field::clear()
{ }

FieldComposite::FieldComposite(const string& name)
  : Field(name)
{ }

FieldComposite::FieldComposite(const FieldComposite& other)
  : Field(other)
{
  for (int i = 0; i < other.fieldCount(); i++) {
    addField(other.getField(i).clone());
  }
}

Field *
FieldComposite::clone() const
{
  return new FieldComposite(*this);
}
Field::Type
FieldComposite::getType() const
{
  return type_composite;
}

void
FieldComposite::addField(Field * field)
{
  Field*& old = map_[field->getName()];
  if (old == 0) {
    old = field;
    array_.push_back(field);
  } else if (field->getType() != old->getType())
    throw std::runtime_error("Add field already present with incompatible type");
}


int
FieldComposite::fieldCount() const
{
  return map_.size();
}

bool
FieldComposite::hasField(const string& field) const
{
  return map_.find(field) != map_.end();
}

bool
FieldComposite::hasField(int field) const
{
  return field >= 0 && field < array_.size();
}

Field&
FieldComposite::getField(int field)
{
  if (field >= 0 && field < array_.size())
    return *array_[field];
  throw std::out_of_range("Field in FieldComposite");
}

const Field&
FieldComposite::getField(int field) const
{
  if (field >= 0 && field < array_.size())
    return *array_[field];
  throw std::out_of_range("Field in FieldComposite");
}

Field&
FieldComposite::getField(const string& field)
{
  Field * f = map_[field];
  if (f != 0)
    return *f;
  throw std::runtime_error("Unexisting field");
}

const Field&
FieldComposite::getField(const string& field) const
{
  Map::const_iterator i = map_.find(field);
  if (i == map_.end())
    return nullField;
  return *i->second;
}

void
FieldComposite::clear()
{
  map_.clear();
  array_.clear();
}

FieldBool::FieldBool(const string& name, bool value)
  : Field(name),
    value_(value)
{ }

FieldBool::FieldBool(const FieldBool& other)
  : Field(other),
    value_(other.value_)
{ }

Field *
FieldBool::clone() const
{
  return new FieldBool(*this);
}
Field::Type
FieldBool::getType() const
{
  return type_bool;
}

bool
FieldBool::getBoolValue() const
{
  return value_;
}

void
FieldBool::setBoolValue(bool v)
{
  value_ = v;
  updateTimeStamp();
}

FieldInt::FieldInt(const string& name, int value)
  : Field(name),
    value_(value)
{ }

FieldInt::FieldInt(const FieldInt& other)
  : Field(other),
    value_(other.value_)
{ }

Field *
FieldInt::clone() const
{
  return new FieldInt(*this);
}
Field::Type
FieldInt::getType() const
{
  return type_int;
}

int
FieldInt::getIntValue() const
{
  return value_;
}

void
FieldInt::setIntValue(int v)
{
  value_ = v;
  updateTimeStamp();
}

FieldFloat::FieldFloat(const string& name, float value)
  : Field(name),
    value_(value)
{ }

FieldFloat::FieldFloat(const FieldFloat& other)
  : Field(other),
    value_(other.value_)
{ }

Field *
FieldFloat::clone() const
{
  return new FieldFloat(*this);
}
Field::Type
FieldFloat::getType() const
{
  return type_float;
}

float
FieldFloat::getFloatValue() const
{
  return value_;
}

void
FieldFloat::setFloatValue(float v)
{
  value_ = v;
  updateTimeStamp();
}

FieldDouble::FieldDouble(const string& name, double value)
  : Field(name),
    value_(value)
{ }

FieldDouble::FieldDouble(const FieldDouble& other)
  : Field(other),
    value_(other.value_)
{ }

Field *
FieldDouble::clone() const
{
  return new FieldDouble(*this);
}
Field::Type
FieldDouble::getType() const
{
  return type_double;
}

double
FieldDouble::getDoubleValue() const
{
  return value_;
}

void
FieldDouble::setDoubleValue(double v)
{
  value_ = v;
  updateTimeStamp();
}

FieldString::FieldString(const string& name, const string& value)
  : Field(name),
    value_(value)
{ }

FieldString::FieldString(const FieldString& other)
  : Field(other),
    value_(other.value_)
{ }

Field *
FieldString::clone() const
{
  return new FieldString(*this);
}
Field::Type
FieldString::getType() const
{
  return type_string;
}

const string&
FieldString::getStringValue() const
{
  return value_;
}

void
FieldString::setStringValue(const string& v)
{
  value_ = v;
  updateTimeStamp();
}

} // namespace infovis

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
#ifndef INFOVIS_TABLE_METADATA_HPP
#define INFOVIS_TABLE_METADATA_HPP

#include <infovis/alloc.hpp>
#include <infovis/table/column.hpp>

namespace infovis {

/**
 * Metadata properties are associated with columns and describe their
 * semantic.
 */
namespace metadata {

extern const string type;/// key for the visualization type
extern const string type_nominal; /// nominal data type
extern const string type_categorical; /// categorical data type
extern const string type_ordinal; /// ordinal data type 

extern const string aggregate;/// key for aggregation type
extern const string aggregate_sum; /// aggregate by sum hierarchically
extern const string aggregate_average; /// aggregate by average hierarchically
extern const string aggregate_min; /// aggregate by min value hierarchically
extern const string aggregate_max; /// aggregate by max value hierarchically

extern const string user_type; /// key for the user-visible type
extern const string user_type_unix_time; /// user-visible type is unix date

} // namespace metadata

} // namespace infovis

#endif // INFOVIS_TABLE_METADATA_HPP

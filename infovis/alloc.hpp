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
#ifndef INFOVIS_ALLOC_HPP
#define INFOVIS_ALLOC_HPP


# if defined (_MSC_VER)
# if (_MSC_VER > 1000)
// #pragma warning ( disable : 4251 )	// ignore template classes being exported in .dll's
/* 
 * "this used in base member initializer list"
 * arrow operator warning
 * copy constr & assignment cannot be generated
 * "forcing value to bool 'true' or 'false'
 * typedef used instaead of full type
 * 4018 : signed/unsigned mismatch, 4146 - result still unsigned 
 */
#  pragma warning ( disable : 4355 4284  4231 4511 4512 4097 4786 4800 4018 4146 4244 4514 4127)
#  pragma warning ( disable : 4245 4514 4660) // conversion from enum to unsigned int signed/unsigned mismatch
#  if (_MSC_VER > 1200)
// multiple copy constructors/assignment operators specified,
// with member templates are bogus...
#   pragma warning ( disable : 4521 4522)
#  endif  
# endif
#endif

#include <infovis/gc_alloc.hpp>
#endif // INFOVIS_ALLOC_HPP

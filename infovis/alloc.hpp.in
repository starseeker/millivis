/* -*- C++ -*-
 *
 * Copyright (C) 2002 Jean-Daniel Fekete
 * 
 * This file is part of MillionVis.
 * 
 * MillionVis is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * MillionVis is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MillionVis; see the file COPYING.  If not, write to the
 * Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA. 
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

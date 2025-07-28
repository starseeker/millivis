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
#ifndef _gc_alloc_hpp
#define _gc_alloc_hpp

#ifndef USE_GC
#include <memory>
/**
 * Base class for garbage collecting.
 */
struct gc {};

/**
 * Base class for garbage collecting and still calling the destructor.
 */
struct gc_cleanup : virtual public gc {};

template <class T, bool Atomic = false>
class gc_alloc : public std::allocator<T>
{
public:
  // TODO: Added C++17 allocator requirements for modernization
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  
  template<class U>
  struct rebind {
    using other = gc_alloc<U, Atomic>;
  };
  
  gc_alloc() = default;
  
  template<class U>
  gc_alloc(const gc_alloc<U, Atomic>&) {}
};

enum GCPlacement {UseGC,
#ifndef GC_NAME_CONFLICT
		  GC=UseGC,
#endif
                  NoGC, PointerFreeGC};

typedef void (* GCCleanUpFunc)();

inline void*
operator new(std::size_t size,
	     GCPlacement gcp,
	     GCCleanUpFunc cleanup = 0,
	     void* clientData = 0)
{
  return ::operator new(size);
}

inline void* operator new[](std::size_t size, 
			    GCPlacement gcp,
			    GCCleanUpFunc cleanup = 0,
			    void* clientData = 0)
{
  return ::operator new(size);
}

#else
#include <gc_cpp.h>

struct GC_true_type {};
struct GC_false_type {};

template <class GC_tp>
struct GC_type_traits {
  GC_false_type GC_is_ptr_free;
};

# define GC_DECLARE_PTRFREE(T) \
template<> struct GC_type_traits<T> { GC_true_type GC_is_ptr_free; }

GC_DECLARE_PTRFREE(signed char);
GC_DECLARE_PTRFREE(unsigned char);
GC_DECLARE_PTRFREE(signed short);
GC_DECLARE_PTRFREE(unsigned short);
GC_DECLARE_PTRFREE(signed int);
GC_DECLARE_PTRFREE(unsigned int);
GC_DECLARE_PTRFREE(signed long);
GC_DECLARE_PTRFREE(unsigned long);
GC_DECLARE_PTRFREE(float);
GC_DECLARE_PTRFREE(double);
/* The client may want to add others.	*/

// In the following GC_Tp is GC_true_type iff we are allocating a
// pointerfree object.
template <class GC_Tp>
inline void * GC_selective_alloc(std::size_t n, GC_Tp) {
    return GC_MALLOC(n);
}

template <>
inline void * GC_selective_alloc<GC_true_type>(std::size_t n, GC_true_type) {
    return GC_MALLOC_ATOMIC(n);
}

template <class T, bool Atomic = false>
class gc_alloc {
public:
  typedef std::size_t	size_type;
  typedef ptrdiff_t	difference_type;
  typedef T		value_type;
  typedef T *		pointer;
  typedef const T*	const_pointer;
  typedef T&		reference;
  typedef const T&	const_reference;

  template <class U> struct rebind { typedef gc_alloc<U> other; };
  
  gc_alloc() throw() {}
  gc_alloc(const gc_alloc&) throw() {}
#ifndef _MSC_VER
  template <class U> gc_alloc(const gc_alloc<U>&) throw() {}
#endif
  ~gc_alloc() throw() {}

  pointer address(reference x) const { return &x; }
  const_pointer address(const_reference x) const { return &x; }

  pointer allocate(std::size_type n, const void * = 0) {
    if (n ==0)
      return 0;
#if 1
    GC_type_traits<T> traits;
    return static_cast<T *>
	    (GC_selective_alloc(n * sizeof(T),
			        traits.GC_is_ptr_free));
#else
    return (pointer)operator new ((size_type)(n * sizeof(value_type)));
#endif
  }
  char *_Charalloc(size_type n) {
      return n == 0 ? 0 :
	(char*)GC_MALLOC_ATOMIC (n);
  }
  void deallocate(pointer p, size_type) {
    deallocate(p);
  }
  void deallocate(pointer p) const {
    if (p != 0)
      operator delete (p);
  }
  void deallocate(void * p, size_type) {
    if (p != 0)
      operator delete (p);
  }

  size_type max_size() const throw()
    { return std::size_t(-1) / sizeof(value_type); }

  void construct(pointer p, const T& val) const {
    new (p) T(val);
  }
  void destroy(pointer p) const {
    p->~T();
  }
  template <class U, bool A>
  bool operator==(const gc_alloc<U, A>& ) const { return false; }

  bool operator==(const gc_alloc<T, Atomic>& ) const { return true; }
};

#endif // USE_GC
#endif // _gc_alloc_hpp

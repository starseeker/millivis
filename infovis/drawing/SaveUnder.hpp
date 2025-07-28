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
#ifndef INFOVIS_DRAWING_SAVEUNDER_HPP
#define INFOVIS_DRAWING_SAVEUNDER_HPP

#include <infovis/alloc.hpp>

namespace infovis {

/**
 * Class for saving the frame buffer in memory of texture memory
 */
class SaveUnder : public gc_cleanup
{
public:
  SaveUnder(bool use_texture = false);
  SaveUnder(unsigned int max_width, unsigned int max_height,
	    bool use_texture = false);
  ~SaveUnder();

  void save(int x, int y, unsigned int width, unsigned int height);
  void restore();
  void restore(int x, int y);
  void resize(unsigned int max_width, unsigned int max_height);

  unsigned int get_texture() const { return use_texture ? data[0] : 0; }
  unsigned int get_tex_width() const { return use_texture ? tile_width : 0; }
  unsigned int get_tex_height() const { return use_texture ? tile_height : 0; }
  // Utility function useful for others
  static unsigned int next_power_of_2(unsigned int n);
protected:
  bool use_texture;
  unsigned int *data;		// pixels or texture objecte
  unsigned int data_size;	// allocated pixels of texture objects
  unsigned int tile_width;
  unsigned int tile_height;
  unsigned int max_width;
  unsigned int max_height;
  int x;
  int y;
  unsigned int width;
  unsigned int height;

  void allocate_ressources();
  void free_ressources();
};

} // namespace infovis

#endif // INFOVIS_DRAWING_SAVEUNDER_HPP

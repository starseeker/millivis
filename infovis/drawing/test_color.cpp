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
#include <infovis/drawing/color.hpp>
#include <infovis/drawing/color_io.hpp>
#include <infovis/drawing/colors/hsb.hpp>
#include <infovis/drawing/colors/xyz.hpp>
#include <infovis/drawing/drawing.hpp>
#include <iostream>
#include <typeinfo>

using namespace infovis;


typedef unsigned char uchar;
int main()
{
  color_rgb<float> rgb(1.0f, 0.0f, 0.0f);
  color_rgba<unsigned char> rgba(0U, 255U, 0U);
  color_hsb hsb(0.0f, 1.0f, 1.0f);
  color_xyz xyz;

  std::cout << rgb << std::endl;
  std::cout << rgba << std::endl;
  std::cout << hsb << std::endl;

  infovis::color_rgba<float> rgba2(rgb);
  std::cout << rgba2 << std::endl;

  infovis::convert(rgb, hsb);
  std::cout << rgb << std::endl;

  infovis::convert(hsb, rgb);
  std::cout << hsb << std::endl;

#define PRINT_CONVERT(color) \
  infovis::convert(rgb, color_##color); \
  infovis::convert(hsb, rgb); \
  std::cout << #color " rgba=" << color_##color << " hsb=" << hsb << std::endl

  PRINT_CONVERT(none);
  PRINT_CONVERT(white);
  PRINT_CONVERT(black);
  PRINT_CONVERT(red);
  PRINT_CONVERT(green);
  PRINT_CONVERT(blue);
  PRINT_CONVERT(cyan);
  PRINT_CONVERT(magenta);
  PRINT_CONVERT(yellow);
  PRINT_CONVERT(half_grey);
  
#undef PRINT_CONVERT
#define PRINT_CONVERT(color) \
  infovis::convert(rgb, color_##color); \
  infovis::convert<float>(xyz, rgb); \
  std::cout << #color " rgb=" << color_##color << " xyz=" << xyz << std::endl

  PRINT_CONVERT(none);
  PRINT_CONVERT(white);
  PRINT_CONVERT(black);
  PRINT_CONVERT(red);
  PRINT_CONVERT(green);
  PRINT_CONVERT(blue);
  PRINT_CONVERT(cyan);
  PRINT_CONVERT(magenta);
  PRINT_CONVERT(yellow);
  PRINT_CONVERT(half_grey);

  return 0;
}

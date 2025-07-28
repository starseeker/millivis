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
#include <infovis\drawing\colors\hvc.hpp>
#include <stdexcept>
#include <cassert>
#include <cmath>

namespace infovis {

#ifdef DBL_EPSILON
#  define MY_DBL_EPSILON DBL_EPSILON
#else
#  define MY_DBL_EPSILON 0.00001
#endif
static const float u_best_red = 0.7127f;
static const float v_best_red = 0.4931f;
static const float epsilon    = 0.001f;
static const float chroma_scale = 7.50725f;

#ifndef PI
#  ifdef M_PI
#    define PI	M_PI
#  else
#    define PI       3.14159265358979323846264338327950
#  endif
#endif

inline double degrees(double r)
{
  return (r * 180.0 / PI);
}

inline double radians(double d)
{
  return (d * PI / 180.0);
}

color_hvc::color_hvc()
{
  channel[hue] = float(0);
  channel[value] = float(1);
  channel[chroma] = float(0);
}

bool
color_hvc::is_valid() const
{
  return (channel[value] < (-MY_DBL_EPSILON) ||
	  channel[value] > (100 + MY_DBL_EPSILON) ||
	  channel[chroma] < (-MY_DBL_EPSILON))
    ? false : true;
}

bool
color_hvc::check_modify()
{
  bool ret = true;

  if (channel[value] < 0) {
    ret = false;
    channel[value] = 0;
  }
  else if (channel[value] > 100) {
    ret = false;
    channel[value] = 100;
  }

  if (channel[chroma] < 0) {
    ret = false;
    channel[chroma] = 0;
  }

  if (channel[hue] < 0) {
    int n = int(-channel[hue]/360);
    channel[hue] += (n + 1) * 360;
    if (channel[hue] >= 360)
      channel[hue] -= 360;
    ret = false;
  }
  else if (channel[hue] > 360) {
    int n = int(channel[hue] / 360);
    channel[hue] -= n * 360;
    ret = false;
  }
  return ret;
}

// FIXME, should be implemented
bool
color_hvc::clip_chroma() { return true; }
bool
color_hvc::clip_value() { return true; }
bool
color_hvc::clip_value_chroma() { return true; }
float
color_hvc::max_chroma() { return true; }
float
color_hvc::max_value() { return true; }
float
color_hvc::min_value() { return true; }
float
color_hvc::max_value_chroma() { return true; }


static float theta_offset(const color_uvY& uvY)
{
  return degrees(atan2(v_best_red - uvY[color_uvY::v_prime],
		       u_best_red - uvY[color_uvY::u_prime]));
}

void convert(color_uvY& uvY, const color_hvc& hvc)
{
  color_uvY white;
  float thetaOffset = theta_offset(white);

  if (hvc[color_hvc::value] == 0) {
    uvY[color_uvY::Y] = 0;
  } else if (hvc[color_hvc::value] == 100) {
    uvY[color_uvY::Y] = 1;
  } else {
    float tempHue = hvc[color_hvc::hue]  + thetaOffset;

    while (tempHue < 0) {
      tempHue += 360;
    }
    while (tempHue >= 360) {
      tempHue -= 360;
    }

    tempHue = radians(tempHue);
    float u = cos(tempHue) * hvc[color_hvc::chroma] /
      hvc[color_hvc::value] * chroma_scale;
    float v = sin(tempHue) * hvc[color_hvc::chroma] /
      hvc[color_hvc::value] * chroma_scale;
    uvY[color_uvY::u_prime] = u + white[color_uvY::u_prime];
    uvY[color_uvY::v_prime] = v + white[color_uvY::v_prime];

    if (hvc[color_hvc::value] < 7.99953624f) {
      uvY[color_uvY::Y] = hvc[color_hvc::value] / 903.29f;
    } else {
      float tmpVal = (hvc[color_hvc::value] + 16) / 116;
      uvY[color_uvY::Y] = tmpVal * tmpVal * tmpVal;
    }
  }
}

void convert(color_hvc& hvc, const color_uvY& uvY)
{
  color_uvY white;
  float thetaOffset = theta_offset(white);

  float u = uvY[color_uvY::u_prime] - white[color_uvY::u_prime];
  float v = uvY[color_uvY::v_prime] - white[color_uvY::v_prime];

  float theta = (u == 0) ? 0 : degrees(atan2(u, v));
  float nThetaLow = 0;
  float nThetaHigh = 360;
  if (u > 0 && v > 0) {
    nThetaLow = 0;
    nThetaHigh = 90;
  }
  else if (u < 0 && v > 0) {
    nThetaLow = 90;
    nThetaHigh = 180;
  }
  else if (u < 0 && v < 0) {
    nThetaLow = 180;
    nThetaHigh = 270;
  }
  else if (u > 0 && v < 0) {
    nThetaLow = 270;
    nThetaHigh = 360;
  }
  while (theta < nThetaLow) {
    theta += 90;
  }
  while (theta >= nThetaHigh) {
    theta -= 90;
  }
  float L2 = (uvY[color_uvY::Y] < 0.008856f) ?
    uvY[color_uvY::Y] * 903.29f :
    float(pow(uvY[color_uvY::Y] * 116, 1/3) - 16);
  hvc[color_hvc::chroma] = L2 * chroma_scale * float(hypot(u, v));
  if (hvc[color_hvc::chroma] < 0) {
    theta = 0;
  }
  hvc[color_hvc::value] = L2;
  hvc[color_hvc::hue] = theta - thetaOffset;

  while (hvc[color_hvc::hue] < -epsilon) {
    hvc[color_hvc::hue] += 360.0;
  }
  while (hvc[color_hvc::hue] >= 360.0 + epsilon) {
    hvc[color_hvc::hue] -= 360.0;
  }
}

void convert(color_xyz& xyz, const color_hvc& hvc)
{
  color_uvY uvY;

  convert(uvY, hvc);
  convert(xyz, uvY);
}

void convert(color_hvc& hvc, const color_xyz& xyz)
{
  color_uvY uvY;

  convert(uvY, xyz);
  convert(hvc, uvY);
}

void convert(color_rgb<float>& rgb, const color_hvc& hvc)
{
  color_xyz xyz;

  convert(xyz, hvc);
  convert(rgb, xyz);
}

void convert(color_hvc& hvc, const color_rgb<float>& rgb)
{
  color_xyz xyz;

  convert(xyz, rgb);
  convert(hvc, xyz);
}

} // namespace infovis

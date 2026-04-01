#pragma once

#include "interval.hpp"
#include "vec3.hpp"

#include <ostream>

using Colour = Vec3;

inline double linear_to_gamma(double linear_comp) {
  if (linear_comp > 0)
    return std::sqrt(linear_comp);

  return 0.0;
}

inline void write_colour(std::ostream &out, const Colour &c) {
  auto r = linear_to_gamma(c.x);
  auto g = linear_to_gamma(c.y);
  auto b = linear_to_gamma(c.z);

  static const Interval intensity(0.000, 0.999);
  int ir = static_cast<int>(255.999 * intensity.clamp(r));
  int ig = static_cast<int>(255.999 * intensity.clamp(g));
  int ib = static_cast<int>(255.999 * intensity.clamp(b));

  out << ir << ' ' << ig << ' ' << ib << ' ' << '\n';
}

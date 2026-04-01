#pragma once

#include "interval.hpp"
#include "vec3.hpp"

#include <ostream>

using colour = vec3;

inline void write_colour(std::ostream &out, const colour &c) {
  auto r = c.x;
  auto g = c.y;
  auto b = c.z;

  static const interval intensity(0.000, 0.999);
  int ir = static_cast<int>(255.999 * intensity.clamp(r));
  int ig = static_cast<int>(255.999 * intensity.clamp(g));
  int ib = static_cast<int>(255.999 * intensity.clamp(b));

  out << ir << ' ' << ig << ' ' << ib << ' ' << '\n';
}

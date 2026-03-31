#pragma once

#include "vec3.hpp"

#include <ostream>

using colour = vec3;

inline void write_colour(std::ostream &out, const colour &c) {
  auto r = c.x;
  auto g = c.y;
  auto b = c.z;

  int ir = static_cast<int>(255.999 * r);
  int ig = static_cast<int>(255.999 * g);
  int ib = static_cast<int>(255.999 * b);

  out << ir << ' ' << ig << ' ' << ib << ' ' << '\n';
}

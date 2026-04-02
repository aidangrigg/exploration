#pragma once

#include "interval.hpp"
#include "vec3.hpp"

#include <cstdint>
#include <ostream>

using Colour = Vec3;

inline double linear_to_gamma(double linear_comp) {
  if (linear_comp > 0)
    return std::sqrt(linear_comp);

  return 0.0;
}

inline uint32_t to_argb8888(const Colour &c) {
  auto r = linear_to_gamma(c.x);
  auto g = linear_to_gamma(c.y);
  auto b = linear_to_gamma(c.z);

  uint8_t ir = static_cast<uint8_t>(255.999 * std::clamp(r, 0.0, 0.9));
  uint8_t ig = static_cast<uint8_t>(255.999 * std::clamp(g, 0.0, 0.9));
  uint8_t ib = static_cast<uint8_t>(255.999 * std::clamp(b, 0.0, 0.9));

  return 0xFF << 24 | ir << 16 | ig << 8 | ib;
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

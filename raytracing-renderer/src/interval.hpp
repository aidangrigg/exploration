#pragma once

#include "utils/constants.hpp"

#include <algorithm>

class Interval {
public:
  const double min, max;

  constexpr Interval() : min(-infinity), max(infinity) {}
  constexpr Interval(double min, double max) : min(min), max(max) {}

  constexpr double size() const { return max - min; }

  constexpr bool contains(double x) const { return x >= min && x <= max; }
  constexpr bool surrounds(double x) const { return x > min && x < max; }
  constexpr double clamp(double x) const { return std::clamp(x, min, max); }

  static const Interval empty, universe;
};

constexpr Interval Interval::empty = Interval(+infinity, -infinity);
constexpr Interval Interval::universe = Interval(-infinity, -infinity);

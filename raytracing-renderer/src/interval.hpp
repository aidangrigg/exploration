#pragma once

#include "utils.hpp"
#include <algorithm>
class interval {
public:
  const double min, max;

  constexpr interval() : min(-infinity), max(infinity) {}
  constexpr interval(double min, double max) : min(min), max(max) {}

  constexpr double size() const { return max - min; }

  constexpr bool contains(double x) const { return x >= min && x <= max; }
  constexpr bool surrounds(double x) const { return x > min && x < max; }
  constexpr double clamp(double x) const { return std::clamp(x, min, max); }

  static const interval empty, universe;
};

constexpr interval interval::empty = interval(+infinity, -infinity);
constexpr interval interval::universe = interval(-infinity, -infinity);

#pragma once

#include <cstdlib>
#include <limits>
#include <random>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

inline double random_double(double min, double max) {
  static std::uniform_real_distribution<double> distribution(min, max);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double random_double() { return random_double(0.0, 1.0); }

constexpr double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }

#pragma once

#include <random>

inline double random_double(double min, double max) {
  static std::uniform_real_distribution<double> distribution(min, max);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double random_double() { return random_double(0.0, 1.0); }

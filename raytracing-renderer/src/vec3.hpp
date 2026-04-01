#pragma once

#include "utils.hpp"
#include <cmath>
#include <ostream>

class vec3 {
public:
  union {
    struct {
      double x;
      double y;
      double z;
    };
    double e[3];
  };

  constexpr vec3() : x(0), y(0), z(0) {}
  constexpr vec3(double x, double y, double z) : x(x), y(y), z(z) {}

  constexpr vec3 operator-() const { return vec3(-x, -y, -z); }

  constexpr vec3 &operator+=(const vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  constexpr vec3 &operator*=(double t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }

  constexpr vec3 &operator/=(double t) { return *this *= 1 / t; }
  constexpr double length() const { return std::sqrt(length_squared()); }
  constexpr double length_squared() const { return x * x + y * y + z * z; }

  static vec3 random() { return vec3(random_double(), random_double(), random_double()); }

  static vec3 random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
  }

  constexpr bool near_zero() const {
    constexpr auto s = 1e-8;
    return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
  }
};

using point3 = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}

constexpr inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

constexpr inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

constexpr inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

constexpr inline vec3 operator*(double t, const vec3 &v) { return vec3(t * v.x, t * v.y, t * v.z); }

constexpr inline vec3 operator*(const vec3 &v, double t) { return t * v; }

constexpr inline vec3 operator/(const vec3 &v, double t) { return (1 / t) * v; }

constexpr inline double dot(const vec3 &u, const vec3 &v) {
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

constexpr inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

constexpr inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }

inline vec3 random_unit_vector() {
  for (int lim = 0; lim < 100; lim++) {
    auto p = vec3::random(-1, 1);
    auto lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1)
      return p / std::sqrt(lensq);
  }

  return vec3();
}

inline vec3 random_on_hemisphere(const vec3 &normal) {
  vec3 on_unit_spehere = random_unit_vector();
  if (dot(on_unit_spehere, normal) > 0.0) {
    return on_unit_spehere;
  } else {
    return -on_unit_spehere;
  }
}

constexpr inline vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2 * dot(v, n) * n; }

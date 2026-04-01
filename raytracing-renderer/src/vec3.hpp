#pragma once

#include "utils/random.hpp"
#include <cmath>
#include <ostream>

class Vec3 {
public:
  union {
    struct {
      double x;
      double y;
      double z;
    };
    double e[3];
  };

  constexpr Vec3() : x(0), y(0), z(0) {}
  constexpr Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

  constexpr Vec3 operator-() const { return Vec3(-x, -y, -z); }

  constexpr Vec3 &operator+=(const Vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  constexpr Vec3 &operator*=(double t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }

  constexpr Vec3 &operator/=(double t) { return *this *= 1 / t; }
  constexpr double length() const { return std::sqrt(length_squared()); }
  constexpr double length_squared() const { return x * x + y * y + z * z; }

  static Vec3 random() { return Vec3(random_double(), random_double(), random_double()); }

  static Vec3 random(double min, double max) {
    return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
  }

  constexpr bool near_zero() const {
    constexpr auto s = 1e-8;
    return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
  }
};

using Point3 = Vec3;

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}

constexpr inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

constexpr inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

constexpr inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

constexpr inline Vec3 operator*(double t, const Vec3 &v) { return Vec3(t * v.x, t * v.y, t * v.z); }

constexpr inline Vec3 operator*(const Vec3 &v, double t) { return t * v; }

constexpr inline Vec3 operator/(const Vec3 &v, double t) { return (1 / t) * v; }

constexpr inline double dot(const Vec3 &u, const Vec3 &v) {
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

constexpr inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

constexpr inline Vec3 unit_vector(const Vec3 &v) { return v / v.length(); }

inline Vec3 random_unit_vector() {
  for (int lim = 0; lim < 100; lim++) {
    auto p = Vec3::random(-1, 1);
    auto lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1)
      return p / std::sqrt(lensq);
  }

  return Vec3();
}

inline Vec3 random_on_hemisphere(const Vec3 &normal) {
  Vec3 on_unit_spehere = random_unit_vector();
  if (dot(on_unit_spehere, normal) > 0.0) {
    return on_unit_spehere;
  } else {
    return -on_unit_spehere;
  }
}

constexpr inline Vec3 reflect(const Vec3 &v, const Vec3 &n) { return v - 2 * dot(v, n) * n; }

constexpr inline Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat) {
  auto cos_theta = std::fmin(dot(-uv, n), 1.0);
  Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

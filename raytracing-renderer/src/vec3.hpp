#pragma once

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

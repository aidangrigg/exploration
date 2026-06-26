#pragma once

#include "ray.hpp"
#include "math/interval.hpp"
#include <memory>

class Material;

class HitRecord {
public:
  Point3 p;
  Vec3 normal;
  std::shared_ptr<Material> mat;
  double t;
  bool front_face;

  void set_face_normal(const Ray &r, const Vec3 &outward_normal);
};

class Hittable {
public:
  virtual ~Hittable() = default;
  virtual bool hit(const Ray& r, const Interval& ray_t, HitRecord& rec) const = 0;
};

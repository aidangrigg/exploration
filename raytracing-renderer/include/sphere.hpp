#pragma once

#include "hittable.hpp"
#include "math/interval.hpp"
#include "math/vec3.hpp"
#include <cmath>
#include <memory>

class Sphere : public Hittable {
public:
  Sphere(const Point3 &center, double radius, std::shared_ptr<Material> mat);
  bool hit(const Ray &r, const Interval &ray_t, HitRecord &rec) const override;

private:
  const Point3 center;
  const double radius;
  std::shared_ptr<Material> mat;
};

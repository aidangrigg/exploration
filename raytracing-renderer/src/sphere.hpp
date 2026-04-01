#pragma once

#include "hittable.hpp"
#include "interval.hpp"
#include "vec3.hpp"
#include <cmath>
#include <memory>

class Sphere : public Hittable {
public:
  Sphere(const Point3 &center, double radius, std::shared_ptr<Material> mat)
      : center(center), radius(std::fmax(0, radius)), mat(mat) {}

  bool hit(const Ray &r, const Interval &ray_t, HitRecord &rec) const override {
    // D2 t2 + 2 O D t + O2 - R2 = 0

    // a = D^2
    // b = 2OD
    // c = O^2 - R^2
    // where D = ray direction
    //       O = ray origin

    const Vec3 oc = center - r.origin();
    const double a = r.direction().length_squared();
    const double h = dot(r.direction(), oc);
    const double c = oc.length_squared() - radius * radius;

    const double discriminant = h * h - a * c;
    if (discriminant < 0)
      return false;

    const auto sqrtd = std::sqrt(discriminant);

    const auto a_div = 1.0 / a;
    auto root = (h - sqrtd) * a_div;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) * a_div;
      if (!ray_t.surrounds(root))
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    // normal is the unit vector from intersection point to center of circle
    auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
  }

private:
  const Point3 center;
  const double radius;
  std::shared_ptr<Material> mat;
};

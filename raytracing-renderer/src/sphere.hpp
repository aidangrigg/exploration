#pragma once

#include "hittable.hpp"
#include "interval.hpp"
#include "vec3.hpp"
#include <cmath>

class sphere : public hittable {
public:
  sphere(const point3 &center, double radius) : center(center), radius(std::fmax(0, radius)) {}

  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    // D2 t2 + 2 O D t + O2 - R2 = 0

    // a = D^2
    // b = 2OD
    // c = O^2 - R^2
    // where D = ray direction
    //       O = ray origin

    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
      return false;

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root))
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    // normal is the unit vector from intersection point to center of circle
    auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
  }

private:
  const point3 center;
  const double radius;
};

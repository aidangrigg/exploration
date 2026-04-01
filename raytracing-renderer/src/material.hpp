#pragma once

#include "colour.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class material {
public:
  virtual ~material() = default;

  virtual bool scatter(const ray &, const hit_record &, colour &, ray &) const { return false; }
};

class lambertian : public material {
public:
  lambertian(const colour &albedo) : albedo(albedo) {}

  bool scatter(const ray &, const hit_record &rec, colour &attenuation,
               ray &scattered) const override {
    auto direction = rec.normal + random_on_hemisphere(rec.normal);

    if (direction.near_zero())
      direction = rec.normal;

    scattered = ray(rec.p, direction);

    attenuation = albedo;
    return true;
  }

private:
  colour albedo;
};

class metal : public material {
public:
  metal(const colour &albedo) : albedo(albedo) {}

  bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation,
               ray &scattered) const override {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return true;
  }

private:
  colour albedo;
};

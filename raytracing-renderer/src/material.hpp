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
  metal(const colour &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

  bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation,
               ray &scattered) const override {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return true;
  }

private:
  colour albedo;
  double fuzz;
};

class dielectric : public material {
public:
  dielectric(double refractive_index) : refractive_index(refractive_index) {};

  bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation,
               ray &scattered) const override {
    attenuation = colour(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0 / refractive_index) : refractive_index;

    vec3 unit_direction = unit_vector(r_in.direction());

    double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double())
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, ri);

    scattered = ray(rec.p, direction);

    // vec3 refracted = refract(unit_direction, rec.normal, ri);
    // scattered = ray(rec.p, refracted);
    return true;
  }

private:
  double refractive_index;

  static double reflectance(double cosine, double refractive_index) {
    auto r0 = (1 - refractive_index) / (1 + refractive_index);
    r0 = r0*r0;
    return r0 + (1-r0)*std::pow((1 - cosine),5);
  }
};

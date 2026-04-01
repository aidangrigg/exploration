#pragma once

#include "colour.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class Material {
public:
  virtual ~Material() = default;

  virtual bool scatter(const Ray &, const HitRecord &, Colour &, Ray &) const { return false; }
};

class Lambertian : public Material {
public:
  Lambertian(const Colour &albedo) : albedo(albedo) {}

  bool scatter(const Ray &, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override {
    auto direction = rec.normal + random_on_hemisphere(rec.normal);

    if (direction.near_zero())
      direction = rec.normal;

    scattered = Ray(rec.p, direction);

    attenuation = albedo;
    return true;
  }

private:
  Colour albedo;
};

class Metal : public Material {
public:
  Metal(const Colour &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

  bool scatter(const Ray &r_in, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override {
    Vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return true;
  }

private:
  Colour albedo;
  double fuzz;
};

class Dielectric : public Material {
public:
  Dielectric(double refractive_index) : refractive_index(refractive_index) {};

  bool scatter(const Ray &r_in, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override {
    attenuation = Colour(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0 / refractive_index) : refractive_index;

    Vec3 unit_direction = unit_vector(r_in.direction());

    double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    Vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double())
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, ri);

    scattered = Ray(rec.p, direction);

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

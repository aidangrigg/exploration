#pragma once

#include "colour.hpp"
#include "hittable.hpp"
#include "ray.hpp"

class Material {
public:
  virtual ~Material() = default;
  virtual bool scatter(const Ray &, const HitRecord &, Colour &, Ray &) const { return false; }
};

class Lambertian : public Material {
public:
  Lambertian(const Colour &albedo);

  bool scatter(const Ray &, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override;

private:
  Colour albedo;
};

class Metal : public Material {
public:
  Metal(const Colour &albedo, double fuzz);

  bool scatter(const Ray &r_in, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override;

private:
  Colour albedo;
  double fuzz;
};

class Dielectric : public Material {
public:
  Dielectric(double refractive_index);

  bool scatter(const Ray &r_in, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override;

private:
  double refractive_index;
  static double reflectance(double cosine, double refractive_index);
};

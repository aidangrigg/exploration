#include "camera.hpp"
#include "hittable-list.hpp"
#include "material.hpp"
#include "sphere.hpp"

#include <cassert>
#include <cmath>
#include <memory>

int main() {
  Camera cam({
      .max_depth = 10,
      .samples_per_pixel = 50,
      .image_width = 1200,
      .lookfrom = {13, 2, 3},
      .lookat = {0, 0, 0},
      .vfov = 20
  });

  HittableList world;

  auto ground_material = std::make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
  world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      Point3 center(a + 0.9, 0.2, b + 0.9);

      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<Material> sphere_material;

        if ((b * a) % 3 == 0) {
          // diffuse
          auto albedo = Colour(0.5, 0.2, 1.0);
          sphere_material = std::make_shared<Lambertian>(albedo);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else if ((b * a) % 3 == 1) {
          // metal
          auto albedo = Colour(0.1, 0.5, 0.6);
          auto fuzz = 0.3;
          sphere_material = std::make_shared<Metal>(albedo, fuzz);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = std::make_shared<Dielectric>(1.5);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
  world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  cam.render(world);
  return 0;
}

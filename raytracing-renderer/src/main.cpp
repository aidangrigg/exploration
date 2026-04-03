#include "camera.hpp"
#include "hittable-list.hpp"
#include "material.hpp"
#include "renderer/renderer.hpp"
#include "sphere.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <memory>

int main() {

  constexpr double aspect_ratio = 16.0 / 9.0;
  constexpr int width = 400;
  constexpr int height = width / aspect_ratio;

  camera::ViewportSettings vs{.width = width, .height = height};

  std::shared_ptr<camera::Camera> camera = std::make_shared<camera::Camera>(vs);
  camera->set_camera_pos({5, 2, 5});

  renderer::Renderer r(camera, {.raycast_depth = 3, .samples_per_pixel = 6});

  HittableList world;

  auto ground_material = std::make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
  world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  // for (int a = -11; a < 11; a++) {
  //   for (int b = -11; b < 11; b++) {
  //     Point3 center(a + 0.9, 0.2, b + 0.9);

  //     if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
  //       std::shared_ptr<Material> sphere_material;

  //       if ((b * a) % 3 == 0) {
  //         // diffuse
  //         auto albedo = Colour(0.5, 0.2, 1.0);
  //         sphere_material = std::make_shared<Lambertian>(albedo);
  //         world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
  //       } else if ((b * a) % 3 == 1) {
  //         // metal
  //         auto albedo = Colour(0.1, 0.5, 0.6);
  //         auto fuzz = 0.3;
  //         sphere_material = std::make_shared<Metal>(albedo, fuzz);
  //         world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
  //       } else {
  //         // glass
  //         sphere_material = std::make_shared<Dielectric>(1.5);
  //         world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
  //       }
  //     }
  //   }
  // }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
  world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  while (r.running()) {
    r.handle_inputs();
    r.render(world);
  }

  return 0;
}

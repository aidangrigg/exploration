#pragma once

#include "colour.hpp"
#include "hittable.hpp"
#include "material.hpp"

#include "renderer/backends/sdl.hpp"
#include "utils/math.hpp"
#include "utils/random.hpp"

#include "utils/thread_pool.hpp"
#include "vec3.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

struct camera_settings {
  const int max_depth = 10;
  const int samples_per_pixel = 100;
  const double aspect_ratio = 16.0 / 9.0;
  const double image_width = 400;
  const Point3 lookfrom = {0, 0, 0};
  const Point3 lookat = {0, 0, -1};
  const Vec3 vup = {0, 1, 0};
  const double vfov = 90;
};

class Camera {
public:
  Camera(const camera_settings &settings)
      : max_depth(settings.max_depth), samples_per_pixel(settings.samples_per_pixel),
        pixel_samples_scale(1.0 / settings.samples_per_pixel), image_width(settings.image_width),
        image_height(static_cast<int>(settings.image_width / settings.aspect_ratio)),
        lookfrom(settings.lookfrom) {

    const double focal_length = (lookfrom - lookat).length();
    auto theta = degrees_to_radians(settings.vfov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2 * h * focal_length;
    const double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    auto w = unit_vector(lookfrom - lookat);
    auto u = unit_vector(cross(vup, w));
    auto v = cross(w, u);

    Vec3 viewport_u = viewport_width * u;   // Vector across viewport horizontal edge
    Vec3 viewport_v = viewport_height * -v; // Vector down viewport vertical edge

    auto viewport_upper_left = lookfrom - (focal_length * w) - viewport_u / 2 - viewport_v / 2;

    const_cast<Vec3 &>(this->pixel_delta_u) = viewport_u / image_width;
    const_cast<Vec3 &>(this->pixel_delta_v) = viewport_v / image_height;
    const_cast<Point3 &>(this->pixel00_loc) =
        viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  void render(const Hittable &world) {
    ThreadPool pool;
    std::vector<uint32_t> pixels(image_height * image_width);

    for (int r = 0; r < image_height; r++) {
      pool.push_task([this, r, &pixels, &world] {
        for (int c = 0; c < image_width; c++) {
          Colour pixel_colour(0, 0, 0);

          for (int sample = 0; sample < samples_per_pixel; sample++) {
            Ray ray = get_ray(c, r);
            pixel_colour += ray_colour(ray, max_depth, world);
          }

          pixels[c + image_width * r] = to_argb8888(pixel_colour * pixel_samples_scale);
        }
      });
    }

    renderer::SDLBackend r(image_width, image_height);

    while (pool.is_running() && r.is_running()) {
      r.render_framebuffer(pixels);
    }

    while (r.is_running()) {
    }
  }

private:
  Ray get_ray(int w, int h) const {
    auto offset = Vec3(random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0.0);
    auto pixel_sample =
        pixel00_loc + ((w + offset.x) * pixel_delta_u) + ((h + offset.y) * pixel_delta_v);
    auto direction = pixel_sample - lookfrom;

    return Ray(lookfrom, direction);
  }

  Colour ray_colour(const Ray &r, int depth, const Hittable &world) const {
    if (depth <= 0)
      return Colour(1, 1, 1);

    HitRecord rec;

    if (world.hit(r, Interval(0.001, infinity), rec)) {
      Ray scattered;
      Colour attenuation;

      rec.mat->scatter(r, rec, attenuation, scattered);
      return attenuation * ray_colour(scattered, depth - 1, world);
    }

    Vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
  }

  const int max_depth;
  const int samples_per_pixel;
  const double pixel_samples_scale;
  const double image_width;
  const double image_height;
  const Vec3 pixel_delta_u;
  const Vec3 pixel_delta_v;
  const Point3 pixel00_loc;
  const Point3 lookfrom = Point3(13, 2, 3);
  const Point3 lookat = Point3(0, 0, 0);
  const Vec3 vup = Vec3(0, 1, 0);
};

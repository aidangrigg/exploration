#pragma once

#include "colour.hpp"
#include "material.hpp"
#include "hittable.hpp"
#include "utils.hpp"
#include "vec3.hpp"

#include <iostream>

class camera {
public:
  camera(double aspect_ratio = 16.0 / 9.0, double image_width = 400,
         point3 camera_center = {0, 0, 0})
      : max_depth(10), samples_per_pixel(100), pixel_samples_scale(1.0 / samples_per_pixel),
        aspect_ratio(aspect_ratio), image_width(image_width),
        image_height(static_cast<int>(image_width / aspect_ratio)), camera_center(camera_center) {

    const double focal_length = 1.0;
    const double viewport_height = 2.0;
    const double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);

    const vec3 viewport_u = vec3(viewport_width, 0, 0);
    const vec3 viewport_v = vec3(0, -viewport_height, 0);

    const_cast<vec3 &>(this->pixel_delta_u) = viewport_u / image_width;
    const_cast<vec3 &>(this->pixel_delta_v) = viewport_v / image_height;
    const_cast<point3 &>(this->viewport_upper_left) =
        camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

    const_cast<point3 &>(this->pixel00_loc) =
        viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  void render(const hittable &world) {
    constexpr int max_colour = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << '\n' << max_colour << '\n';

    for (int h = 0; h < image_height; h++) {
      std::clog << "Scanlines reminaing: " << (image_height - h) << '\r' << std::flush;
      for (int w = 0; w < image_width; w++) {
        colour pixel_colour(0, 0, 0);

        for (int sample = 0; sample < samples_per_pixel; sample++) {
          ray r = get_ray(w, h);
          pixel_colour += ray_colour(r, max_depth, world);
        }

        write_colour(std::cout, pixel_samples_scale * pixel_colour);
      }
    }
  }

private:
  ray get_ray(int w, int h) const {
    auto offset = vec3(random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0.0);
    auto pixel_sample =
        pixel00_loc + ((w + offset.x) * pixel_delta_u) + ((h + offset.y) * pixel_delta_v);
    auto direction = pixel_sample - camera_center;

    return ray(camera_center, direction);
  }

  colour ray_colour(const ray &r, int depth, const hittable &world) const {
    if (depth <= 0)
      return colour(0, 0, 0);

    hit_record rec;

    if (world.hit(r, interval(0.001, infinity), rec)) {
      ray scattered;
      colour attenuation;

      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_colour(scattered, depth - 1, world);
      }
      return colour(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
  }

  const int max_depth;
  const int samples_per_pixel;
  const double pixel_samples_scale;
  const double aspect_ratio;
  const double image_width;
  const double image_height;
  const vec3 pixel_delta_u;
  const vec3 pixel_delta_v;
  const point3 camera_center;

  const point3 viewport_upper_left;
  const point3 pixel00_loc;
};

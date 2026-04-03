#pragma once

#include "ray.hpp"

#include "utils/math.hpp"
#include "utils/random.hpp"

#include "vec3.hpp"

struct CameraSettings {
  const int image_width = 400;
  const int image_height = 225;
  const double vfov = 90;
  const Point3 lookfrom = {0, 0, 0};
  const Point3 lookat = {0, 0, -1};
  const Vec3 vup = {0, 1, 0};
};

class Camera {
private:
  const int image_width;
  const int image_height;
  Vec3 pixel_delta_u;
  Vec3 pixel_delta_v;
  Point3 pixel00_loc;
  Point3 lookfrom;
  Point3 lookat;
  Vec3 vup;

public:
  Camera(const CameraSettings &settings)
      : image_width(settings.image_width), image_height(settings.image_height),
        lookfrom(settings.lookfrom), vup(settings.vup) {

    const double focal_length = (lookfrom - lookat).length();
    auto theta = degrees_to_radians(settings.vfov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    auto w = unit_vector(lookfrom - lookat);
    auto u = unit_vector(cross(vup, w));
    auto v = cross(w, u);

    Vec3 viewport_u = viewport_width * u;   // Vector across viewport horizontal edge
    Vec3 viewport_v = viewport_height * -v; // Vector down viewport vertical edge

    auto viewport_upper_left = lookfrom - (focal_length * w) - viewport_u / 2 - viewport_v / 2;

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  int width() const { return image_width; }
  int height() const { return image_height; }

  Ray get_ray(int w, int h) const {
    auto offset = Vec3(random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0.0);
    auto pixel_sample =
        pixel00_loc + ((w + offset.x) * pixel_delta_u) + ((h + offset.y) * pixel_delta_v);
    auto direction = pixel_sample - lookfrom;

    return Ray(lookfrom, direction);
  }
};

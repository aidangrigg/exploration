#pragma once

#include "ray.hpp"

#include "utils/math.hpp"
#include "utils/random.hpp"

#include "vec3.hpp"
#include <cmath>
#include <iostream>

namespace camera {

enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

struct ViewportSettings {
  int width = 800;
  int height = 450;
  double vfov = 90;
};

class Camera {
private:
  // Viewport stuff
  Vec3 pixel_du, pixel_dv;
  Point3 viewport_pixel_start;
  int image_width;
  int image_height;

  // Camera stuff
  double vfov;
  Vec3 position, front, up, right, world_up;
  double pitch, yaw;

  void recompute_viewport() {
    const double pitch_rad = degrees_to_radians(pitch);
    const double yaw_rad = degrees_to_radians(yaw);

    front.x = std::cos(yaw_rad) * std::cos(pitch_rad);
    front.y = std::sin(pitch_rad);
    front.z = std::sin(yaw_rad) * std::cos(pitch_rad);
    front.normalize();

    right = unit_vector(cross(front, world_up));
    up = cross(right, front);

    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2 * h;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    const auto viewport_u = viewport_width * right; // Vector across viewport horizontal edge
    const auto viewport_v = viewport_height * -up;  // Vector down viewport vertical edge

    const auto viewport_upper_left = position - front - viewport_u / 2 - viewport_v / 2;

    pixel_du = viewport_u / image_width;
    pixel_dv = viewport_v / image_height;
    viewport_pixel_start = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);
  }

public:
  Camera(const ViewportSettings &vs)
      : image_width(vs.width), image_height(vs.height), vfov(vs.vfov), position({0, 0, 0}),
        world_up({0, 1, 0}), pitch(0.0), yaw(0.0) {
    recompute_viewport();
  }

  Camera(const ViewportSettings &vs, Point3 position, double pitch = 0.0, double yaw = 0.0)
      : image_width(vs.width), image_height(vs.height), vfov(vs.vfov), position(position),
        world_up({0, 1, 0}), pitch(pitch), yaw(yaw) {
    recompute_viewport();
  }

  int width() const { return image_width; }
  int height() const { return image_height; }

  constexpr static double velocity = 0.2;

  void move(Movement m) {
    switch (m) {
    case FORWARD:
      position -= (front * velocity);
      break;
    case BACKWARD:
      position += (front * velocity);
      break;
    case LEFT:
      position -= (right * velocity);
      break;
    case RIGHT:
      position += (right * velocity);
      break;
    }

    recompute_viewport();
  }

  // PERF: would be more efficient to only recompute once per frame, rather than
  //       3 times max (pos change, pitch change, yaw change)
  void set_camera_pos(Point3 pos) {
    position = std::move(pos);
    recompute_viewport();
  }

  void set_pitch(double pitch) {
    this->pitch = pitch;
    recompute_viewport();
  }

  void set_yaw(double yaw) {
    this->yaw = yaw;
    recompute_viewport();
  }

  Ray get_ray(int w, int h) const {
    auto offset = Vec3(random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0.0);
    auto pixel_sample =
        viewport_pixel_start + ((w + offset.x) * pixel_du) + ((h + offset.y) * pixel_dv);
    auto direction = pixel_sample - position;

    return Ray(position, direction);
  }
};
}; // namespace camera

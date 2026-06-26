#pragma once

#include "ray.hpp"
#include "math/vec3.hpp"

#include <cmath>

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

  void recompute_viewport();

public:
  Camera(const ViewportSettings &vs);
  Camera(const ViewportSettings &vs, Point3 position, double pitch = 0.0, double yaw = 0.0);

  int get_width() const;
  int get_height() const;

  float get_yaw();
  float get_pitch();

  constexpr static double velocity = 0.2;

  void move(Movement m);

  void set_camera_pos(Point3 pos);

  void set_pitch(double pitch);

  void set_yaw(double yaw);

  Ray get_ray(int w, int h) const;
};
}; // namespace camera

#include "camera.hpp"
#include "hittable-list.hpp"
#include "sphere.hpp"

#include <cassert>
#include <cmath>
#include <memory>

int main() {
  camera cam(16.0 / 9.0, 400);

  // world
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

  cam.render(world);
  return 0;
}

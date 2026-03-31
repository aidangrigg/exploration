#include "camera.hpp"
#include "colour.hpp"
#include "hittable-list.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "utils.hpp"
#include "vec3.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>


int main() {
  camera cam(16.0 / 9.0, 400);

  // world
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

  cam.render(world);
  return 0;
}

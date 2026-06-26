#include "camera.hpp"
#include "colour.hpp"
#include "hittable.hpp"
#include "utils/thread_pool.hpp"

#include "backends/sdl.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <cstdint>
#include <memory>

namespace renderer {

//  Should contain:
// - Viewport stuff (probably?)
// - Pixel locations for sure (can probably generate these within the camera)
// - Thread pool & SDLBackend
// - Have a function that takes in a camera & a scene, and draws that scene to the screen
// - Should maybe also have a pointer to the camera, so that it can update the camera on keypress
//   events

struct Settings {
  const int raycast_depth = 5;
  const int samples_per_pixel = 10;
  const int window_width = 1920;
  const int window_height = 1080;
};

// TODO: could handle a ratio of image_width/height & screen_width/height,
//       Although, most games probably just handle the image_width/height and either set the screen
//       to the same size, or stretch it to the window size. Think about this...

class Renderer {
private:
  const float MOUSE_SENSITIVITY = 0.1;

  ThreadPool pool;

  std::shared_ptr<camera::Camera> camera;
  std::shared_ptr<SDLBackend> backend;

  std::vector<uint32_t> fb;

  Settings settings;

  Colour ray_colour(const Ray &r, int depth, const Hittable &world) const;

  void keyboard_handler(SDL_Scancode code);
  void handle_mouse_motion(float dx, float dy);

public:
  Renderer(std::shared_ptr<camera::Camera> camera, Settings settings);
  void handle_inputs();
  void draw(const Hittable &world);
  bool running();
};

}; // namespace renderer

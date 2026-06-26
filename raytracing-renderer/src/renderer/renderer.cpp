#include "renderer/renderer.hpp"

#include "material.hpp"

namespace renderer {

//  Should contain:
// - Viewport stuff (probably?)
// - Pixel locations for sure (can probably generate these within the camera)
// - Thread pool & SDLBackend
// - Have a function that takes in a camera & a scene, and draws that scene to the screen
// - Should maybe also have a pointer to the camera, so that it can update the camera on keypress
//   events

// TODO: could handle a ratio of image_width/height & screen_width/height,
//       Although, most games probably just handle the image_width/height and either set the screen
//       to the same size, or stretch it to the window size. Think about this...

Renderer::Renderer(std::shared_ptr<camera::Camera> camera, Settings settings)
    : camera(camera), settings(settings) {
  backend = std::make_shared<SDLBackend>(settings.window_width, settings.window_height,
                                         camera->get_width(), camera->get_height());
  fb.resize(camera->get_width() * camera->get_height(), 0);
}

Colour Renderer::ray_colour(const Ray &r, int depth, const Hittable &world) const {
  if (depth <= 0)
    return Colour(1, 1, 1);

  HitRecord rec;

  if (world.hit(r, Interval(0.001, infinity), rec)) {
    Ray scattered;
    Colour attenuation;

    if (rec.mat->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_colour(scattered, depth - 1, world);
    } else {
      return Colour(0, 0, 0);
    }
  }

  Vec3 unit_direction = unit_vector(r.direction());
  auto a = 0.5 * (unit_direction.y + 1.0);
  return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
}

void Renderer::keyboard_handler(SDL_Scancode code) {
  switch (code) {
  case SDL_SCANCODE_W:
    camera->move(camera::FORWARD);
    break;
  case SDL_SCANCODE_S:
    camera->move(camera::BACKWARD);
    break;
  case SDL_SCANCODE_A:
    camera->move(camera::LEFT);
    break;
  case SDL_SCANCODE_D:
    camera->move(camera::RIGHT);
    break;
  default:
    break;
  }
}

void Renderer::handle_mouse_motion(float dx, float dy) {
  if (dx != 0) {
    auto yaw = camera->get_yaw();
    yaw -= dx * MOUSE_SENSITIVITY;
    camera->set_yaw(yaw);
  }

  if (dy != 0) {
    auto pitch = camera->get_pitch();
    pitch += dy * MOUSE_SENSITIVITY;
    camera->set_pitch(pitch);
  }
}

void Renderer::handle_inputs() {
  SDL_Event event;

  while (backend->poll_event(event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      backend->set_running(false);
      break;
    case SDL_EVENT_KEY_DOWN:
      keyboard_handler(event.key.scancode);
      break;
    case SDL_EVENT_MOUSE_MOTION:
      handle_mouse_motion(event.motion.xrel, event.motion.yrel);
      break;
    }
  }
}

void Renderer::draw(const Hittable &world) {
  const auto start_time = std::chrono::steady_clock::now();

  const int image_width = camera->get_width();
  const int image_height = camera->get_height();
  const double pixel_samples_scale = 1.0 / settings.samples_per_pixel;

  for (int r = 0; r < image_height; r++) {
    pool.push_task([&, this, r] {
      for (int c = 0; c < image_width; c++) {
        Colour pixel_colour(0, 0, 0);

        for (int sample = 0; sample < settings.samples_per_pixel; sample++) {
          Ray ray = camera->get_ray(c, r);
          pixel_colour += ray_colour(ray, settings.raycast_depth, world);
        }

        fb[c + image_width * r] = to_argb8888(pixel_colour * pixel_samples_scale);
      }
    });
  }

  while (pool.is_busy()) {
  }

  const auto end_time = std::chrono::steady_clock::now();

  std::chrono::duration<double, std::milli> ms_double = end_time - start_time;
  backend->render_framebuffer(fb, image_width, ms_double.count());
}

bool Renderer::running() { return backend->is_running(); }

}; // namespace renderer

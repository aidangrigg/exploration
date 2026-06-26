#pragma once

#include <SDL3/SDL.h>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace renderer {

class SDLBackend {
private:
  template <class T> using sdl_unique_ptr = std::unique_ptr<T, std::function<void(T *)>>;

  // uint32_t window_width, window_height, image_width, image_height;
  bool running;
  sdl_unique_ptr<SDL_Window> window;
  sdl_unique_ptr<SDL_Renderer> renderer;
  sdl_unique_ptr<SDL_Texture> texture;

public:
  SDLBackend(int window_width, int window_height, int image_width, int image_height);

  ~SDLBackend();

  bool is_running() const;
  void set_running(bool r);

  // TODO: dont't expose SDL event
  bool poll_event(SDL_Event &e);

  void render_framebuffer(std::vector<uint32_t> &fb, size_t width,
                          std::optional<double> frame_time);
};

}; // namespace renderer

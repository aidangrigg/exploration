#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace renderer {

class SDLBackend {
private:
  template <class T> using sdl_unique_ptr = std::unique_ptr<T, std::function<void(T *)>>;

  int window_width, window_height;
  bool running;
  sdl_unique_ptr<SDL_Window> window;
  sdl_unique_ptr<SDL_Renderer> renderer;
  sdl_unique_ptr<SDL_Texture> texture;

public:
  SDLBackend(int width, int height) : window_width(width), window_height(height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
      return; // TODO: throw exceptions for these errors
    }

    window = sdl_unique_ptr<SDL_Window>(
        SDL_CreateWindow("really cool window title...", window_width, window_height, 0),
        SDL_DestroyWindow);

    if (!window.get()) {
      std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
      SDL_Quit();
      return;
    }

    renderer = sdl_unique_ptr<SDL_Renderer>(SDL_CreateRenderer(window.get(), nullptr),
                                            SDL_DestroyRenderer);

    if (!renderer.get()) {
      std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
      SDL_Quit();
      return;
    }

    texture = sdl_unique_ptr<SDL_Texture>(
        SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                          window_width, window_height),
        SDL_DestroyTexture);

    if (!texture.get()) {
      std::cerr << "Failed to create texture: " << SDL_GetError() << "\n";
      SDL_Quit();
      return;
    }

    running = true;
  }

  ~SDLBackend() { SDL_Quit(); }

  bool is_running() const { return running; }

  void render_framebuffer(std::vector<uint32_t> &fb) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_UpdateTexture(texture.get(), nullptr, fb.data(),
                      window_width * static_cast<int>(sizeof(uint32_t)));

    SDL_RenderClear(renderer.get());
    SDL_RenderTexture(renderer.get(), texture.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer.get());
  }
};

}; // namespace renderer

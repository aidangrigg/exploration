add_library(raytrace-engine-lib
  camera.cpp
  hittable.cpp
  hittable-list.cpp
  material.cpp
  ray.cpp
  sphere.cpp
  utils/thread_pool.cpp
  renderer/renderer.cpp
  renderer/backends/sdl.cpp
)

target_link_libraries(raytrace-engine-lib PRIVATE SDL3::SDL3)
target_include_directories(raytrace-engine-lib PRIVATE ../include/)

add_executable(raytrace-engine main.cpp)
target_include_directories(raytrace-engine PRIVATE ../include/)
target_link_libraries(raytrace-engine PRIVATE raytrace-engine-lib)

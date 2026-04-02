cmake_minimum_required(VERSION 3.10)
project(raycast-renderer CXX)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

find_package(SDL3 REQUIRED)

add_executable(raycast-renderer
        src/main.cpp)

target_link_libraries(raycast-renderer PRIVATE SDL3::SDL3)

# target_link_libraries(unicorn_datastream
#     PRIVATE
#     ${CMAKE_SOURCE_DIR}/lib/libunicorn.so
#     stdc++ # needed by libunicorn
#     bluetooth
# )

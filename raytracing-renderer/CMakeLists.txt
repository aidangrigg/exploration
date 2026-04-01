cmake_minimum_required(VERSION 3.10)
project(raycast-renderer CXX)

add_compile_options(-Wall -Wextra)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

add_executable(raycast-renderer
        src/main.cpp)

# target_link_libraries(unicorn_datastream
#     PRIVATE
#     ${CMAKE_SOURCE_DIR}/lib/libunicorn.so
#     stdc++ # needed by libunicorn
#     bluetooth
# )

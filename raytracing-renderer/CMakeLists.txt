cmake_minimum_required(VERSION 3.10)
project(raycast-renderer CXX)

add_compile_options(-Wall -Wextra)

add_executable(raycast-renderer
        src/main.cpp)

# target_link_libraries(unicorn_datastream
#     PRIVATE
#     ${CMAKE_SOURCE_DIR}/lib/libunicorn.so
#     stdc++ # needed by libunicorn
#     bluetooth
# )

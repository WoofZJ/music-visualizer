cmake_minimum_required(VERSION 3.20)

project(music-visualizer)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_C_STANDARD 11)

add_subdirectory("raylib")

file(GLOB SRC "src/*.c")
add_executable(music-visualizer ${SRC})

target_link_libraries(music-visualizer PUBLIC raylib m)

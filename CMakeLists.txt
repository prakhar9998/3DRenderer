cmake_minimum_required(VERSION 3.10)

project(Renderer)

# import library target for SFML
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)
include_directories(include)
add_executable(Renderer src/main.cpp src/display.cpp src/mesh.cpp include/display.h include/vector3d.h include/mesh.h include/matrix.h include/camera.h)
target_link_libraries(Renderer sfml-graphics sfml-window)

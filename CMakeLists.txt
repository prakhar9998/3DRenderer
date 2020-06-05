cmake_minimum_required(VERSION 3.10)

project(Renderer)

# import library target for SFML
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)

add_executable(Renderer main.cpp)
target_link_libraries(Renderer sfml-graphics sfml-window)

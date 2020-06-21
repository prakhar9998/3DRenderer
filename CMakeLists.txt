cmake_minimum_required(VERSION 3.10)

project(Renderer)

# import library target for SFML
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(include lib)
file(GLOB SOURCES
    include/*.h
    src/*.cpp
    lib/stb_image.h)
add_executable(Renderer ${SOURCES})
target_link_libraries(Renderer PRIVATE OpenMP::OpenMP_CXX sfml-graphics sfml-window)

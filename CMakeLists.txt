cmake_minimum_required(VERSION 3.11)
project(Renderer CXX)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE sources RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(Renderer WIN32 ${sources})

target_link_libraries(Renderer PRIVATE OpenGL::GL)
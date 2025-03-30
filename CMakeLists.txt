cmake_minimum_required(VERSION 3.11)
project(Renderer CXX)

set(CMAKE_CXX_STANDARD 14)
file(GLOB_RECURSE sources RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
IF (WIN32)
find_package(OpenGL REQUIRED)

add_executable(Renderer WIN32 ${sources})

target_link_libraries(Renderer PRIVATE OpenGL::GL)
ELSE()
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

add_executable(Renderer ${sources})

target_link_libraries(Renderer PRIVATE OpenGL::GL ${X11_LIBRARIES})
ENDIF()

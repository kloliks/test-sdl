cmake_minimum_required (VERSION 3.1.0)
project( test-sdl )

set(CMAKE_CXX_FLAGS "-O2 -Wall")
set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

include_directories(
  ${SDL2_INCLUDE_DIRS}
  include/AnalyticGeometry
  include/Graphics
  )

add_subdirectory( src )
add_subdirectory( test )

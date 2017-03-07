cmake_minimum_required (VERSION 3.1.0)
project( test.a )

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(
  test.a
  main.cpp
)
#target_compile_features( test.a PRIVATE cxx_range_for)
  

target_link_libraries(
  test.a
  boost_system
  ${OPENGL_LIBRARIES}
  ${SDL2_LIBRARIES}
)

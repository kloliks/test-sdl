add_executable(
  test-sdl
  AnalyticGeometry/Matrix.cpp
  Graphics/Point.cpp
  main.cpp
)
#target_compile_features( test.a PRIVATE cxx_range_for)

target_link_libraries(
  test-sdl
  boost_system
  ${OPENGL_LIBRARIES}
  ${SDL2_LIBRARIES}
)

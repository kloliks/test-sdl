add_executable(
  test.a
  ../src/AnalyticGeometry/Matrix.cpp
  ../src/Graphics/Point.cpp
  main.cpp
)
#target_compile_features( test.a PRIVATE cxx_range_for)

target_link_libraries(
  test.a
  boost_system
  ${OPENGL_LIBRARIES}
  ${SDL2_LIBRARIES}
)


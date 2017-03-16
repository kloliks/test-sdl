#pragma once

struct Color {
  enum { COLOR_VECTOR_SIZE = 4 };
  float color[COLOR_VECTOR_SIZE] = { .0f, .0f, .0f, .0f };

  void setColor(float r, float g, float b, float a) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
  }
  void setColor(float r, float g, float b) {
    setColor(r, g, b, 1.0f);
  }
};

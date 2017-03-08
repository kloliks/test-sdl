#pragma once

struct Vector {
  enum { SIZE = 3 };
  float vector[SIZE] = { 0, 0, 0 };

  operator float* () { return vector; }
};

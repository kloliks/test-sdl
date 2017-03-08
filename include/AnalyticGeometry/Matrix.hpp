#pragma once

struct Matrix {
  enum { SIZE = 9 };
  float matrix[SIZE] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  Matrix& loadIdentity();

  float& operator []( int index ) {
    return matrix[index];
  }

  const float& operator[]( int index ) const {
    return matrix[index];
  }
};

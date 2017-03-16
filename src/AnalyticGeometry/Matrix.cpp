#include "Matrix.hpp"

#include <string.h>

struct Identity : Matrix {
  float matrix[SIZE] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
};

Matrix& Matrix::loadIdentity() {
  *this = Identity();
  return *this;
}

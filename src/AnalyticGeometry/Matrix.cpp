#include "Matrix.hpp"

#include <string.h>

struct Indentity : Matrix {
  float matrix[SIZE] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
};

Matrix& Matrix::loadIdentity() {
  *this = Indentity();
  return *this;
}

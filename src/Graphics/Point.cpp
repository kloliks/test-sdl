#include "Graphics/Point.hpp"
#include "AnalyticGeometry/Matrix.hpp"

#ifdef _DEBUG
#include "math.h"
#endif

Point& Point::multMatrix( const Matrix& matrix ){
  const Matrix& m = matrix;

#ifdef _DEBUG
  double r = m[6]*x + m[7]*y + m[8];
  assert(fabs(r - 1) <= 0.003);
#endif

  x = m[0]*x + m[1]*y + m[2];
  y = m[3]*x + m[4]*y + m[5];

  return *this;
}

// vim: path+=include

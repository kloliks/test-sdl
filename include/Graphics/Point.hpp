#pragma once

struct Matrix;
struct Point {
  double x, y;
  Point(double x, double y) : x( x ), y( y ) {}
  Point& set(double x, double y) {
    this->x = x;
    this->y = y;
    return *this;
  }
  Point& multMatrix( const Matrix& matrix );
};

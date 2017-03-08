#include "Matrix.hpp"
#include "Point.hpp"

#include <iostream>
#include <chrono>

#include <string.h>

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

using std::cout;
using std::endl;

void printMatrix( const Matrix& m ){
  for( auto f : m.matrix ){
    cout << f << " ";
  }
  cout << endl;
}
void printPoint( const Point& p ){
  cout
    << "x: " << p.x << ", "
    << "y: " << p.y
    << endl;
}
int main() {
  // TODO: write Unit tests
  Point p(5, 6);
  printPoint( p );

  return 0;
}

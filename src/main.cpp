#include <math.h>
#include <string.h>

#include <list>
#include <vector>
#include <iostream>

#include <boost/shared_ptr.hpp>

#include <SDL.h>
#include <GL/gl.h>

#include "AnalyticGeometry/Matrix.hpp"
#include "Graphics/Point.hpp"

using std::cout;
using std::endl;
using std::list;
using std::vector;

using boost::shared_ptr;

const int screen_width  = 1024;
const int screen_height = 1024;


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
struct Shape {
  Color fill;
  Color line;

  list<Point> points;

  void copyColors(const Shape& s) {
    fill = s.fill;
    line = s.line;
  }

  void setFillColor(float r, float g, float b, float a = 1.0f) {
    fill.setColor(r, g, b, a);
  }
  void setLineColor(float r, float g, float b, float a = 1.0f) {
    line.setColor(r, g, b, a);
  }

  void draw() const {
    if( points.size() == 0 )
      return;

    if( points.size() > 2 && fill.color[3] > 0.00003f ){
      glColor4fv(fill.color);
      glBegin(GL_POLYGON);
      for( const auto& p : points ){
        glVertex2d(p.x, p.y);
      }
      glEnd();
    }

    if( line.color[3] > 0.00003f ){
      glColor4fv(line.color);
      points.size() == 1
        ? glBegin(GL_POINTS)
        : glBegin(GL_LINE_LOOP);
      for( const auto& p : points ){
        glVertex2d(p.x, p.y);
      }
      glEnd();
    }
  }
  Shape& addPoint( double x, double y ){
    points.push_back(Point(x, y));
    return *this;
  }
  Shape& multMatrix( const Matrix& matrix ){
    for( auto& p : points ){
      p.multMatrix(matrix);
    }
    return *this;
  }
  void clear() {
    list<Point> points;
    points.swap(this->points);
  }
};
struct Scene {
  int x     , y;
  int width , height;

  Matrix projection;

  Color web_fill_color;
  Color web_line_color;

  vector<Shape> web;

  vector<Shape> shapes;

  Scene() {}
  Scene( int x, int y, int width, int height )
    : x( x ), y( y ), width( width ), height( height )
  {
    Scene s;
    s.set_projection( x, width, y, height );
    for( int i = 0; i < width; ++i ){
      for( int j = 0; j < height; ++j ){
        Shape shape;
          shape.addPoint(i, j);
          shape.addPoint(i, j+1);
          shape.addPoint(i+1, j+1);
          shape.addPoint(i+1, j);
        s.addShape(shape);
      }
    }
    web.swap(s.shapes);
  }

  void set_projection( float left, float right, float top, float bottom ){
    projection.loadIdentity();

    const float w = right - left;
    const float l = top - bottom;

    projection[0] = 2 / w;
    projection[2] = - (right + left) / w;
    projection[4] = 2 / l;
    projection[5] = - (top + bottom) / l;
    projection[8] = 1;
  }
  void set_projection( const Matrix& projection ){
    this->projection = projection;
  }

  void setWebFillColor( float r, float g, float b ){
    web_fill_color.setColor( r, g, b );
  }
  void setWebLineColor( float r, float g, float b ){
    web_line_color.setColor( r, g, b );
  }

  Scene& addShape( const Shape& shape ){
    Shape s = shape;
    shapes.push_back(s.multMatrix(projection));
    return *this;
  }
  Scene& removeShape( int index ){
    if( shapes.size() ){
      auto it = shapes.begin();
      if( index >= 0 ){
        for( int i = 0; i < index; ++i ){
          if( ++it != shapes.end() ){
            continue;
          }
          else {
            it = shapes.begin();
          }
        }
      }
      else {
        for( int i = index; i < 0; ++i, --it ){
          if( it != shapes.begin() ){
            continue;
          }
          else {
            it = shapes.end();
          }
        }
      }
      shapes.erase(it);
    }
    return *this;
  }

  Scene& rasterization() {
    for( auto& shape : shapes ){
      for( auto& point : shape.points ){
        double xs = (point.x + 1) * (width/2) + x;
        double ys = (-point.y + 1) * (height/2) + y;

        web[int(xs)*width + int(ys)].fill = web_fill_color;
      }
    }
    return *this;
  }

  void drawWeb() const {
    if( web.size() == 0 )
      return;

    for( auto& cell : web ) {
      if( cell.fill.color[3] > 0.00003f ){
        glColor4fv(cell.fill.color);
        glBegin(GL_POLYGON);
        for( const auto& p : cell.points ){
          glVertex2d(p.x, p.y);
        }
        glEnd();
      }
    }

    if( web_line_color.color[3] > 0.00003f ){
      for( auto& cell : web ) {
        glColor4fv(web_line_color.color);
        glBegin(GL_LINE_LOOP);
        for( const auto& p : cell.points ){
          glVertex2d(p.x, p.y);
        }
        glEnd();
      }
    }
  }
  void draw() const {
    for( auto& shape : shapes ){
      if( shape.points.size() == 0 )
        continue;

      if( shape.points.size() > 2 && shape.fill.color[3] > 0.00003f ){
        glColor4fv(shape.fill.color);
        glBegin(GL_POLYGON);
        for( const auto& p : shape.points ){
          glVertex2d(p.x, p.y);
        }
        glEnd();
      }

      if( shape.line.color[3] > 0.00003f ){
        glColor4fv(shape.line.color);
        shape.points.size() == 1
          ? glBegin(GL_POINTS)
          : glBegin(GL_LINE_LOOP);
        for( const auto& p : shape.points ){
          glVertex2d(p.x, p.y);
        }
        glEnd();
      }
    }
  }
};
struct DynamicScene {
  int x     , y;
  int width , height;

  enum { PROJECTION_SIZE = 9 };
  Matrix projection;

  Shape shape;

  DynamicScene( int x, int y, int width, int height )
    : x( x ), y( y ), width( width ), height( height )
  {
    shape.setLineColor(float(0xAF)/256, float(0x87)/256, .0f);
  }

  void set_projection( float left, float right, float top, float bottom ){
    projection.loadIdentity();

    const float w = right - left;
    const float l = top - bottom;

    projection[0] = 2 / w;
    projection[2] = - (right + left) / w;
    projection[4] = 2 / l;
    projection[5] = - (top + bottom) / l;
    projection[8] = 1;
  }
  void set_projection( const Matrix& projection ){
    this->projection = projection;
  }

  Point* addPoint(double x, double y){
    shape.addPoint(x, y);
    return &shape.points.back();
  }
  DynamicScene& clear() {
    shape.clear();
    return *this;
  }

  void draw() const {
    Shape s = shape;
    s.multMatrix(projection).draw();
  }
};

void glOrthoAspectRatio() {
  if( screen_width <= screen_height ){
    const double ar = (double)screen_height / screen_width;
    glOrtho(-1, 1, -ar, ar, .0f, 1);
  }
  else {
    const double ar = (double)screen_width / screen_height;
    glOrtho(-ar, ar, -1, 1, .0f, 1);
  }
}

void drawRectangle( int x, int y, int width, int height ){
  glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x + width, y);
    glVertex2d(x + width, y + height);
    glVertex2d(x, y + height);
  glEnd();
}
void QRect( int x, int y, int w, int h ){
  drawRectangle(x, y, w, h);
}

//void drawVLine(const Scene& scene, double x) {
//  glBegin(GL_LINES);
//    glVertex2d(x, 0);
//    glVertex2d(x, scene.height);
//  glEnd();
//}
//void drawHLine(const Scene& scene, double y) {
//  glBegin(GL_LINES);
//    glVertex2d(0, y);
//    glVertex2d(scene.width, y);
//  glEnd();
//}
void drawgl(const Scene& scene, const DynamicScene& dynamic_scene) {
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, 0, 1);
//  glOrthoAspectRatio();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glColor3f(.2, .8, .4);
//  for( double x = 0; x <= scene.width; x += scene.x_norm ){
//    drawVLine( scene, x );
//  }
//  for( double y = 0; y <= scene.height; y += scene.y_norm ){
//    drawHLine( scene, y );
//  }

  glColor3f(float(0x2A)/256, float(0xA1)/256, float(0x98)/256);
  scene.drawWeb();

  glColor3f(float(0xAF)/256, float(0x87)/256, .0f);
  scene.draw();
  dynamic_scene.draw();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, 0, 1);
  glOrthoAspectRatio();
}

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow(
      "",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      screen_width, screen_height,
      SDL_WINDOW_OPENGL
      );

  SDL_GLContext context = SDL_GL_CreateContext(window);

  glClearColor(.2, .2, .3, 1);
  glViewport(0, 0, screen_width, screen_height);

  Scene static_scene(0, 0, 8, 8);
  static_scene.setWebFillColor(.2, .3, .8);
  static_scene.setWebLineColor(float(0x2A)/256, float(0xA1)/256, float(0x98)/256);
  static_scene.set_projection(0, screen_width, 0, screen_height);

  DynamicScene dynamic_scene(0, 0, screen_width, screen_height);
  dynamic_scene.set_projection(0, screen_width, 0, screen_height);

  Point* point;

  bool mouse_button_down = false;
  bool mouse_motion = false;


  bool running = true;
  while( running ){
    SDL_Event event;
    while( SDL_PollEvent(&event) ){
      switch( event.type ){
        case SDL_QUIT:
          running = false;
          break;

        case SDL_KEYDOWN:
          if( event.key.keysym.sym == SDLK_ESCAPE ){
            running = false;
          }
          break;

        case SDL_MOUSEBUTTONDOWN:
          mouse_button_down = true;
          dynamic_scene.addPoint(event.button.x, event.button.y);
          break;

        case SDL_MOUSEMOTION:
          if( mouse_motion ){
            point->set(event.button.x, event.button.y);
          }
          else if( mouse_button_down ){
            point = dynamic_scene.addPoint(event.button.x, event.button.y);
            mouse_motion = true;
          }
          break;

        case SDL_MOUSEBUTTONUP:
          if( mouse_button_down ){
            static_scene.addShape( dynamic_scene.shape );
            dynamic_scene.clear();
            point = NULL;
            mouse_motion = false;
            mouse_button_down = false;
          }
          break;
      }
    }
    static_scene.rasterization();
    drawgl(static_scene, dynamic_scene);
    SDL_GL_SwapWindow(window);
    SDL_Delay(1);
  }

  SDL_GL_DeleteContext( context );
  SDL_DestroyWindow( window );
  SDL_Quit();
  std::cout << "Done" << std::endl;
  return 0;
}

// vim: path+=include

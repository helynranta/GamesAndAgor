#ifndef _VECTOR2D
#define _VECTOR2D

#include <math.h>

class Vector2d {
public:
    float x = 0.0f;
    float y = 0.0f;
    Vector2d() {;}
    Vector2d(float x_, float y_) : x(x_), y(y_) {;}
    void setPosition (float x_, float y_) { x = x_; y = y_; }
    inline Vector2d normalize() {
      Vector2d vec;
      vec.x=x/sqrt(x*x+y*y);
      vec.y=y/sqrt(x*x+y*y);
      return vec;
    }
};
#endif

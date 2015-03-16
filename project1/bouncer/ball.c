
#include "ball.h"
#include <cmath>
#include <algorithm>

ball::ball(const int _width, const int _height)
:height(_height), width(_width)
{
  this->radius = std::min(width, height)/10;
  this->x_pos = width/2;
  this->y_pos = height/2;
  //TODO Make this a color array
  this-> color = 0;
  this->period = 1;//How long it takes to do once bounce
  this->dy = (height-2*radius)/(period*30);//The amount the image will change per frame
}

ball::~ball()
{
}

void ball::get_pos(int frame)
{
  //Need to change direction
  int direction = sin(frame/(period*30.0)) > 0 ? -1 : 1;
  
  y_pos += direction*dy;

}

bool ball::in_ball(int _x, int _y)
{
  return sqrt((_x - x_pos)*(_x - x_pos) + (_y - y_pos)*(_y - y_pos)) <= radius;
  
}





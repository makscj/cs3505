
#include "ball.h"
#include <cmath>
#include <algorithm>

ball::ball(const int _width, const int _height)
:height(_height), width(_width)
{ 
  this->radius = std::min(width, height)/10;
  this->x_pos = width/2;
  this->y_pos = height/2;
  this->y_0 = height/2;
  //TODO Make this a color array
  this-> color = 0;
  this->period = 1;//How long it takes to do once bounce
  this->dy = (height-2*radius)/(period*30.0);//The amount the image will change per frame
  this->y_direction = 1;
  this->x_direction = 1;
  this->dx =(width-2*radius)/(period*30.0);


}

ball::~ball()
{
}

void ball::update(int frame)
{
  //Need to change direction
  //int direction = sin(frame/(period*30.0)) >= 0 ? 1 : -1;

  if(y_pos >= height-radius)
    {
      y_direction = -1;
    }

  if(y_pos <= y_0)
    {
      y_direction = 1;
    }

  if(x_pos-radius < 0)
    x_direction = 1;
  
  if(x_pos+radius > width)
    x_direction = -1;

  if(true)
    {

    }
  
  x_pos += x_direction*dx;

  y_pos += y_direction*dy;

}

bool ball::in_ball(int _x, int _y)
{
  return sqrt((_x - x_pos)*(_x - x_pos) + (_y - y_pos)*(_y - y_pos)) <= radius;
  
}

int ball::distance_from_center(int _x, int _y)
{
  return sqrt(pow(x_pos-_x, 2) + pow(y_pos - _y, 2));
}

double ball::shading_coeff(int _x, int _y)
{
  // Calculate the offset to get to the edge of the ball at an angle of 45 degrees from the center
  double offset = radius * sqrt(2)/2.0;
  // Calculate the x pos and y pos of a point 2/3 of the way to the edge of the ball
  double shading_center_x = x_pos + offset * 2/3;
  double shading_center_y = y_pos - offset * 2/3;
  // Calculate the maximum distance a point can be from the shading center of the ball
  double max_dist = sqrt(pow(shading_center_x - (x_pos - offset), 2) + pow(shading_center_y - (y_pos + offset), 2));

  // Return a value between 0 and 1 (1 at shading center, 0 at max distance from the shading center)
  return 1 - sqrt(pow(_x - shading_center_x, 2) + pow(_y - shading_center_y, 2))/max_dist;
}





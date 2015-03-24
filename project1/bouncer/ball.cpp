/*
Authors:

Maks Cegielski-Johnson
John Ballard
CS 3505 - Project 1 Checkpoint 3

The ball object used for drawing the ball on the image.
 */


#include "ball.h"
#include <cmath>
#include <algorithm>

ball::ball(const int _width, const int _height)
:height(_height), width(_width)
{ 
  //The radius of the ball
  this->radius = std::min(width, height)/10;
  //The initial x position of the ball
  this->x_pos = width/2;
  //The initial y position of the ball
  this->y_pos = height/2;
  //The intial height of the ball
  this->y_0 = height/2;
  //The period of the oscillation
  this->period = 1;
  //The initial vertical velocity 
  this->dy = (height-2*radius)/(period*45.0);//The amount the image will change per frame
  //The ball is initally moving down
  this->y_direction = 1;
  //The ball is initially moving to the right;
  this->x_direction = 1;
  //The initial horizontal velocity.
  this->dx =(width-2*radius)/(period*30.0);
}

ball::~ball()
{
}

//Change the position of the ball
void ball::update()
{
  //Reverse the ball when the bottom of the ball makes contact with the bottom of the image
  if(y_pos >= height-radius)
    {
      y_direction = -1;
    }

  //Reverse the ball when the top of the ball makes contact with the top of the image
  if(y_pos <= radius)
    {
      y_direction = 1;
    }
  
  //Don't let the ball bounce off screen.
  if(x_pos-radius < 0)
    {
      x_direction = 1;
    }
  
  if(x_pos+radius > width)
    {
      x_direction = -1;
    }
  //Update the positions
  x_pos += x_direction*dx;
  y_pos += y_direction*dy;

}

//Check if the coordinates are located in the ball
bool ball::in_ball(int _x, int _y)
{
  return sqrt((_x - x_pos)*(_x - x_pos) + (_y - y_pos)*(_y - y_pos)) <= radius;
  
}

//The shading function for the ball
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





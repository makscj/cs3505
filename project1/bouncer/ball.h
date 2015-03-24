/*
Authors:
Maks Cegielski-Johnson
John Ballard

CS3505 - Project 1 - Checkpoint 3

Header file for the Ball object. 
*/

#ifndef BALL_H
#define BALL_H

//This class represents the bouncing ball object.
class ball
{
 public:

  //Constructor for the ball object, takes the width and height of the image 
  ball(const int width, const int height);

  //Destructor for the ball object
  ~ball();

  //Move the ball according to its velocity and where it is relative to the borders. 
  void update();

  //Check whether the coordinates are located in the ball
  bool in_ball(int _x, int _y);

  //Function for shading the ball
  double shading_coeff(int _x, int _y);

 private:
  //Radius of the ball
  int radius;
  //Current x position of the ball
  int x_pos;
  //Current y position of the ball
  int y_pos;
  //The width of the image 
  const int width;
  //The height of the image
  const int height;
  //The period of the oscillation of the ball
  int period;
  //The vertical velocity of the ball
  int dy;
  //The horizontal velocity of the ball
  int dx;
  //The initial y position of the ball
  int y_0;
  //The horizontal direction the ball is moving
  int x_direction;
  //The vertical direction the ball is moving
  int y_direction;
};
#endif

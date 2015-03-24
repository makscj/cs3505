
#ifndef BALL_H
#define BALL_H

//dranger.com/ffmpeg/tutorial01.html
class ball
{
 public:

  ball(const int width, const int height);

  ~ball();

  void update(int frame);

  bool in_ball(int _x, int _y);

  int distance_from_center(int _x, int _y);

  double shading_coeff(int _x, int _y);

 private:
  int radius;
  int x_pos;
  int y_pos;
  const int width;
  const int height;
  int color;
  int period;
  int dy;
  int dx;
  int y_0;
  int x_direction;
  int y_direction;
};
#endif


#ifndef BALL_H
#define BALL_H

//dranger.com/ffmpeg/tutorial01.html
class ball
{
 public:

  ball(const int width, const int height);

  ~ball();

  void get_pos(int frame);

  bool in_ball(int _x, int _y);

 private:
  int radius;
  int x_pos;
  int y_pos;
  const int width;
  const int height;
  int color;
  int period;
  int dy;
};
#endif

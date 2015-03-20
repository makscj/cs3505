// g++ test.cpp `pkg-config --cflags --libs libavutil libavformat libavcodec`
//
// The `backticks` above indicate that we execute the pkg-config command, then
// use its results as inputs to the g++ command.  You can execute the
// pkg-config command separately to see what command line arguments it
// generates, but smart CS types will keep using it as shown above so that
// if libraries change, so does your build command.

#include <iostream>

// A few definitions to help this code compile.

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

// All ffmpeg headers refer to C functions and declarations.

extern "C"
{

// You can now include any .h files from ffmpeg, and call and use
// functions and declarations as needed.

#include "libavutil/mathematics.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

}

// Our code is C++, but it will work fine with ffmpeg's C code so
// long as the above strategy is used.

using namespace std;

int main ()
{
  int64_t result;

  result = av_gcd(39, 91);
  
  cout << "Hello: " << result << endl;
  
  return 0;
}

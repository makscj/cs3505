#include "ball.h"
#include <iostream>
#include <../ffmpeg/libavcodec/avcodec.h>
#include <../ffmpeg/libavformat/avformat.h>
#include <../ffmpeg/libswscale/swscale.h>



int main(int argc, char *argv [])
{
  av_register_all();

  AVFormatContext *pFormatCtx = NULL;


  if(avformat_open_input(&pFormatCtx, argv[1], NULL, 0, NULL) != 0)
    return -1;//Couldn't open file


  dump_format(pFormatCtx, 0, argv[1], false);

  AVCodecContext *pCodecCtx;

  pCodecCtx = pFormatCtx->streams[0]->codec;
  pCodecCtx->width = 0;

  return 1;
}

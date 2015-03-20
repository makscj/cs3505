#include "ball.h"
#include <iostream>
#include <../ffmpeg/libavcodec/avcodec.h>
#include <../ffmpeg/libavformat/avformat.h>
#include <../ffmpeg/libswscale/swscale.h>

void drawFrame();

void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);

int main(int argc, char *argv [])
{
  AVCodec *pCodec = NULL;
  AVFrame *pFrame = NULL;
  AVFrame *pFrameRGB = NULL;
  AVPacket packet;
  uint8_t *buffer = NULL;
  AVFormatContext *pFormatCtx = NULL;
  AVCodecContext *pCodecCtx;
  int numBytes;
  int i, videoStream;
  struct SwsContext *sws_ctx = NULL;

  av_register_all();


  //Get pFormatCtx
  if(avformat_open_input(&pFormatCtx, argv[1], NULL, 0, NULL) != 0)
    return -1;//Couldn't open file

  if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
    return -1;//Couldn't find stream info

  av_dump_format(pFormatCtx, 0, argv[1], 0);
  
  // Find the first video stream
  videoStream = -1;
  for(i = 0; i < pFormatCtx -> nb_streams; i++) {
    if(pFormatCtx -> streams[i] -> codec -> codec_type == AVMEDIA_TYPE_VIDEO) {
      videoStream = i;
      break;
    }
  }
  if(videoStream == -1) {
    return -1; // Didn't find a video stream
  }

  // Get a po

  pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);

  pCodecCtx = avcodec_alloc_context3(pCodec);


  if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    return -1;

  pFrame = av_frame_alloc();

  pFrameRGB = av_frame_alloc();

  numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

  buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

  avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, pCodecCtx -> width, pCodecCtx -> height);

  sws_ctx = sws_getContext(pCodecCtx -> width,
			   pCodecCtx -> height,
			   pCodecCtx -> pix_fmt,
			   pCodecCtx -> width,
			   pCodecCtx -> height,
			   PIX_FMT_RGB24,
			   SWS_BILINEAR,
			   NULL,
			   NULL,
			   NULL
			   );

  i = 0;
  if(av_read_frame(pFormatCtx, &packet) < 0){
    // Shit's broke yo
    return -1;
  }
  if(packet.stream_index == videoStream){
    // Decode video frame
    avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
    
    // Did we get a video frame?
    if(frameFinished) {
      // Convert the image from its native format to RGB
    }
  }

  //

  //saveframe shit

  av_free(buffer);
  av_frame_free(&pFrameRGB);

  av_frame_free(&pFrame);

  avcodec_close(pCodecCtx);
  

  avformat_close_input(&pFormatCtx);
  

  return 0;
}


void drawFrame()
{
  
}


void saveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
  FILE *pFile;

  char szFilename[32];

  int y;

  sprintf(szFilename, "frame%d.ppm", iFrame);

  pFile = fopen(szFilename, "wb");

  if(pFile == NULL)
    return;

  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

  for(y = 0; y < height; y++)
    {
      fwrite(pFrame->data[0] + y*pFrame->linesize[0], 1, width*3, pFile);
    }

  fclose(pFile);
}

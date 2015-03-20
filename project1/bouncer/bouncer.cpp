#include <iostream>
#include "ball.h"

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

extern "C"
{

#include "../ffmpeg/libavcodec/avcodec.h"
#include "../ffmpeg/libavformat/avformat.h"
#include "../ffmpeg/libswscale/swscale.h"

}

// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

using namespace std;

// This function was adapted from code found at https://lists.libav.org/pipermail/libav-user/2010-August/005159.html
// TODO: properly cite
int SaveFrame(AVFrame *pFrame, AVCodecContext *pCodecCtx, int FrameNo) {
  
  AVFrame                *pOFrame;
  AVCodecContext         *pOCodecCtx;
  AVCodec                *pOCodec;
  AVPacket                pkt;
  uint8_t                *Buffer;
  int                     BufSiz;
  int                     BufSizActual;
  AVPixelFormat           ImgFmt;
  FILE                   *JPEGFile;
  char                    JPEGFName[256];
  SwsContext              *sws_ctx;

  BufSiz = avpicture_get_size (ImgFmt, pCodecCtx->width, pCodecCtx->height );

  //Buffer = (uint8_t *)malloc ( BufSiz );
  //if ( Buffer == NULL ){
  //  return ( 0 );
  //}
  //memset ( Buffer, 0, BufSiz );
  
  pOCodec = avcodec_find_encoder ( AV_CODEC_ID_MPFF );
  if ( !pOCodec ) {
    //free ( Buffer );
    return ( 0 );
  }

  // Get the first suppported pixel format and use that.
  ImgFmt = pOCodec -> pix_fmts[0];

  // Allocate an AVFrame structure
  pOFrame=av_frame_alloc();
  pOFrame -> width = pFrame -> width;
  pOFrame -> height = pFrame -> height;
  pOFrame -> format = ImgFmt;

  if(pOFrame==NULL){
    return -1;
  }

  // Determine required buffer size and allocate buffer
  int numBytes = avpicture_get_size(ImgFmt, pCodecCtx->width, pCodecCtx->height);
  uint8_t *buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)pOFrame, buffer, ImgFmt, pCodecCtx->width, pCodecCtx->height);

  sws_ctx = sws_getContext(pCodecCtx->width,
			   pCodecCtx->height,
			   pCodecCtx->pix_fmt,
			   pCodecCtx->width,
			   pCodecCtx->height,
			   ImgFmt,
			   SWS_BILINEAR,
			   NULL,
			   NULL,
			   NULL
			   );
 
  // Convert the image from its native format to RGB
  sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
	    pFrame->linesize, 0, pCodecCtx->height,
	    pOFrame->data, pOFrame->linesize);
 

  pOCodecCtx = avcodec_alloc_context3 ( pOCodec );
  if ( !pOCodecCtx ) {
    //free ( Buffer );
    return ( 0 );
  }

  pOCodecCtx->bit_rate      = pCodecCtx->bit_rate;
  pOCodecCtx->width         = pCodecCtx->width;
  pOCodecCtx->height        = pCodecCtx->height;
  pOCodecCtx->pix_fmt       = ImgFmt;
  pOCodecCtx->codec_id      = AV_CODEC_ID_MPFF;
  pOCodecCtx->codec_type    = AVMEDIA_TYPE_VIDEO;
  pOCodecCtx->time_base.num = pCodecCtx->time_base.num;
  pOCodecCtx->time_base.den = pCodecCtx->time_base.den;

  
  if ( avcodec_open2 (pOCodecCtx, pOCodec, NULL) < 0 ) {
    //free ( Buffer );
    return ( 0 );
  }
 

  pOCodecCtx->mb_lmin        = pOCodecCtx->lmin =  pOCodecCtx->qmin * FF_QP2LAMBDA;
  pOCodecCtx->mb_lmax        = pOCodecCtx->lmax = pOCodecCtx->qmax * FF_QP2LAMBDA;
  pOCodecCtx->flags          = CODEC_FLAG_QSCALE;
  pOCodecCtx->global_quality = pOCodecCtx->qmin * FF_QP2LAMBDA;

  pOFrame->pts     = 1;
  pOFrame->quality = pOCodecCtx->global_quality;

  av_init_packet(&pkt);
  pkt.data = NULL;
  pkt.size = 0;

  int ret = 0;
  int *ret_ptr = &ret;
  BufSizActual = avcodec_encode_video2(pOCodecCtx, &pkt,  pOFrame, ret_ptr);

  sprintf ( JPEGFName, "%06d.mpff", FrameNo );
  JPEGFile = fopen ( JPEGFName, "wb" );
  fwrite ( pkt.data, 1, pkt.size, JPEGFile );
  fclose ( JPEGFile );

  avcodec_close ( pOCodecCtx );
  av_frame_free(&pOFrame);
  av_free_packet(&pkt);
  return ( BufSizActual );
  
}

int main(int argc, char *argv[]) {
  
  // Initalizing these to NULL prevents segfaults!
  AVFormatContext   *pFormatCtx = NULL;
  int               i, videoStream;
  AVCodecContext    *pCodecCtxOrig = NULL;
  AVCodecContext    *pCodecCtx = NULL;
  AVCodec           *pCodec = NULL;
  AVFrame           *pFrame = NULL;
  AVFrame           *pFrameRGB = NULL;
  AVPacket          packet;
  int               frameFinished;
  int               numBytes;
  uint8_t           *buffer = NULL;
  SwsContext *sws_ctx = NULL;

  if(argc != 2) {
    printf("Please give 1 image file as the argument.\n");
    return -1;
  }
  // Register all formats and codecs
  av_register_all();
  
  // Open video file
  if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)
    return -1; // Couldn't open file
  
  // Retrieve stream information
  if(avformat_find_stream_info(pFormatCtx, NULL)<0)
    return -1; // Couldn't find stream information
  
  // Dump information about file onto standard error
  av_dump_format(pFormatCtx, 0, argv[1], 0);
  
  // Find the first video stream
  videoStream=-1;
  for(i=0; i<pFormatCtx->nb_streams; i++)
    if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
      videoStream=i;
      break;
    }
  if(videoStream==-1)
    return -1; // Didn't find a video stream
  
  // Get a pointer to the codec context for the video stream
  pCodecCtxOrig=pFormatCtx->streams[videoStream]->codec;
  // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
  if(pCodec==NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1; // Codec not found
  }
  // Copy context
  pCodecCtx = avcodec_alloc_context3(pCodec);
  if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
    fprintf(stderr, "Couldn't copy codec context");
    return -1; // Error copying codec context
  }

  // Open codec
  if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
    return -1; // Could not open codec
  
  // Allocate video frame
  pFrame=av_frame_alloc();
  
  // Allocate an AVFrame structure
  pFrameRGB=av_frame_alloc();
  if(pFrameRGB==NULL)
    return -1;

  // Determine required buffer size and allocate buffer
  numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
			      pCodecCtx->height);
  buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
		 pCodecCtx->width, pCodecCtx->height);
  
  // initialize SWS context for software scaling
  sws_ctx = sws_getContext(pCodecCtx->width,
			   pCodecCtx->height,
			   pCodecCtx->pix_fmt,
			   pCodecCtx->width,
			   pCodecCtx->height,
			   PIX_FMT_RGB24,
			   SWS_BILINEAR,
			   NULL,
			   NULL,
			   NULL
			   );

  // Read frames and save first five frames to disk
  i=0;
  while(av_read_frame(pFormatCtx, &packet)>=0) {
    // Is this a packet from the video stream?
    if(packet.stream_index==videoStream) {
      // Decode video frame
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
      
      // Did we get a video frame?
      if(frameFinished) {
	// Convert the image from its native format to RGB
	sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
		  pFrame->linesize, 0, pCodecCtx->height,
		  pFrameRGB->data, pFrameRGB->linesize);
	
	pCodecCtx -> pix_fmt = PIX_FMT_RGB24;
	//cout << pCodecCtx -> width << endl;
	//cout << pCodecCtx->height << endl;
	for(i = 0; i < (pCodecCtx->width * pCodecCtx->height * 3); i++)
	  {
	    cout << (int)(pFrameRGB->data[0])[i] << " ";
	    if((i+1)%3 == 0){
	      cout << endl;
	    }
	  }
	
	// Save the frame to disk
	if(++i<=5)
	  SaveFrame(pFrameRGB, pCodecCtx, i);
      }
    }
    
    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }
  
  // Free the RGB image
  av_free(buffer);
  av_frame_free(&pFrameRGB);
  
  // Free the YUV frame
  av_frame_free(&pFrame);
  
  // Close the codecs
  avcodec_close(pCodecCtx);
  avcodec_close(pCodecCtxOrig);

  // Close the video file
  avformat_close_input(&pFormatCtx);
  
  return 0;
}

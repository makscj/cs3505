/*
MPFF image format decoder

TODO ADD COMMENTS PLEASE
*/


#include <inttypes.h>

#include "avcodec.h"
#include "bytestream.h"
#include "bmp.h"
#include "internal.h"
#include "msrledec.h"

static int mpff_decode_frame(AVCodecContext *avctx,
                            void *data, int *got_frame,
                            AVPacket *avpkt)
{
    const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    AVFrame *p         = data;
    unsigned int fsize, hsize;
    int width, height;
    unsigned int depth;
    BiCompression comp; // WE NEED THE H FILE FOR THIS
    unsigned int ihsize;
    int i, n, linesize, ret;
    uint8_t *ptr;
    int dsize;
    const uint8_t *buf0 = buf;

    if (buf_size < 12) {
        av_log(avctx, AV_LOG_ERROR, "buf size too small (%d)\n", buf_size);
        return AVERROR_INVALIDDATA;
    }

    if (bytestream_get_byte(&buf) != 'M' ||
        bytestream_get_byte(&buf) != 'P' ||
	bytestream_get_byte(&buf) != 'F' ||
	bytestream_get_byte(&buf) != 'F') {
        av_log(avctx, AV_LOG_ERROR, "bad magic number\n");
        return AVERROR_INVALIDDATA;
    }

    fsize = bytestream_get_le32(&buf);

    //Is this Boggle.h?
    //Is this just Fanta.c?

    if (buf_size < fsize) {
        av_log(avctx, AV_LOG_ERROR, "not enough space (%d < %u), trying to decode anyway, Help us Peter\n",
               buf_size, fsize);
        fsize = buf_size;
    }

    hsize  = bytestream_get_le32(&buf); /* header size */
    ihsize = bytestream_get_le32(&buf); /* info header size */

    
    if (ihsize + 12LL > hsize) {
        av_log(avctx, AV_LOG_ERROR, "invalid header size %u\n", hsize);
        return AVERROR_INVALIDDATA;
    }

    /* sometimes file size is set to some headers size, set a real size in that case */
    if (fsize == 12 || fsize == ihsize + 12)
      fsize = buf_size - 2; //EXCUSE ME?

    if (fsize <= hsize) {
        av_log(avctx, AV_LOG_ERROR,
               "Declared file size is less than header size (%u < %u)\n",
               fsize, hsize);
        return AVERROR_INVALIDDATA;
    }


    width  = bytestream_get_le32(&buf);
    height = bytestream_get_le32(&buf);
    depth = bytestream_get_le16(&buf);

    //TODO WHAT THE FUCK
    comp = BMP_RGB;

 
    avctx->width  = width;
    avctx->height = height > 0 ? height : -height;

    avctx->pix_fmt = AV_PIX_FMT_BGR24;

    if ((ret = ff_get_buffer(avctx, p, 0)) < 0)
        return ret;

    p->pict_type = AV_PICTURE_TYPE_I;
    p->key_frame = 1;

    buf   = buf0 + hsize;
    dsize = buf_size - hsize;

    /* Line size in file multiple of 4 */
    n = ((avctx->width * depth + 31) / 8) & ~3;

    if (n * avctx->height > dsize) {
      n = (avctx->width * depth + 7) / 8;
      if (n * avctx->height > dsize) {
	av_log(avctx, AV_LOG_ERROR, "not enough data (%d < %d)\n",
	       dsize, n * avctx->height);
	return AVERROR_INVALIDDATA;
      }
      av_log(avctx, AV_LOG_ERROR, "data size too small, assuming missing line alignment\n");
    }

    if (height > 0) {
      ptr      = p->data[0] + (avctx->height - 1) * p->linesize[0];
      linesize = -p->linesize[0];
    } else {
      ptr      = p->data[0];
      linesize = p->linesize[0];
    }

    
    
    switch (depth) {
    case 24:
      for (i = 0; i < avctx->height; i++) {
	memcpy(ptr, buf, n);
	buf += n;
	ptr += linesize;
      }
      break;
    default:
      av_log(avctx, AV_LOG_ERROR, "BMP decoder is broken\n");
      return AVERROR_INVALIDDATA;
    }


    *got_frame = 1;

    return buf_size;
}



AVCodec ff_mpff_decoder = {

  .name = "mpff",
  .long_name = NULL_IF_CONFIG_SMALL("MPFF image (a CS 3505 project)"),
  .type = AVMEDIA_TYPE_VIDEO,
  .id = AV_CODEC_ID_BMP,
  .decode = mpff_decode_frame,
};

/*
 * BMP image format decoder
 * Copyright (c) 2015 John Ballard and Maks Cegielski-Johnson
 *
 * This file is part of FFmpeg. Created for CS 3505.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <inttypes.h>

#include "avcodec.h"
#include "bytestream.h"
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
    unsigned int ihsize;
    int i, n, linesize, ret;
    uint8_t *ptr;
    int dsize;
    const uint8_t *buf0 = buf;

    if (buf_size < 12) {
        av_log(avctx, AV_LOG_ERROR, "buf size too small (%d)\n", buf_size);
        return AVERROR_INVALIDDATA;
    }

    //Checking the first few characters for a magic number :|
    if (bytestream_get_byte(&buf) != 'M' ||
        bytestream_get_byte(&buf) != 'P' ||
	bytestream_get_byte(&buf) != 'F' ||
	bytestream_get_byte(&buf) != 'F') {
        av_log(avctx, AV_LOG_ERROR, "bad magic number\n");
        return AVERROR_INVALIDDATA;
    }

    //File size
    fsize = bytestream_get_le32(&buf);

    //Is this the_real_life.h?
    //Is this just fanta.c? 


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
      fsize = buf_size - 2;

    if (fsize <= hsize) {
        av_log(avctx, AV_LOG_ERROR,
               "Declared file size is less than header size (%u < %u)\n",
               fsize, hsize);
        return AVERROR_INVALIDDATA;
    }

    //Get data from stream
    width  = bytestream_get_le32(&buf);
    height = bytestream_get_le32(&buf);
    depth = bytestream_get_le16(&buf);

    //Set context data
    avctx->width  = width;
    avctx->height = height;
    avctx->pix_fmt = AV_PIX_FMT_BGR24;

    //If there is not enough space then return a negative value to indicate failure
    if ((ret = ff_get_buffer(avctx, p, 0)) < 0)
        return ret;

    //Import picture data set
    p->pict_type = AV_PICTURE_TYPE_I;
    p->key_frame = 1;

    //Point the buffer to the start of the picture data (after the header)
    buf   = buf0 + hsize;
    //dsize - d-"rest of the image"_size :D
    dsize = buf_size - hsize;

    /* Line size in file multiple of 4 */
    n = ((avctx->width * depth + 31) / 8) & ~3;

    //Check if there is a data error 
    if (n * avctx->height > dsize) {
      n = (avctx->width * depth + 7) / 8;
      if (n * avctx->height > dsize) {
	av_log(avctx, AV_LOG_ERROR, "not enough data (%d < %d)\n",
	       dsize, n * avctx->height);
	return AVERROR_INVALIDDATA;
      }
      av_log(avctx, AV_LOG_ERROR, "data size too small, assuming missing line alignment\n");
    }

    //Set the pointer to the start of the picture data
    ptr      = p->data[0];
    //Get the linesize
    linesize = p->linesize[0];
    
    //If not a bit count of 24 then an error has occured
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
  .id = AV_CODEC_ID_MPFF,
  .decode = mpff_decode_frame,
};

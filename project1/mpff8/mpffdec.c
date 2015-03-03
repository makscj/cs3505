/*
 * MPFF image format decoder
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
	//Pointer to where the data 
    const uint8_t *buf = avpkt->data;
	//Follow the pointer to the size of the buffer from the context
    int buf_size       = avpkt->size;
	//Frame pointer, we will decode to image to this
    AVFrame *p         = data;
	//The file size and header size
    unsigned int fsize, hsize;
	//The width and height of the image
    int width, height;
	//The pixel count/depth of the image
    unsigned int depth;
	//The size of the information header
    unsigned int ihsize;
	//Different variables we will need later
    int i, n, linesize, ret;
	//Declare a pointer we will use for the image data
    uint8_t *ptr;
	//The size of the rest of the document without the header
    int dsize;
	//Pointer to the buffer that will remain constant 
    const uint8_t *buf0 = buf;

	//If the file has less bytes than the amount needed for the header than throw an exception
    if (buf_size < 12) {
        av_log(avctx, AV_LOG_ERROR, "buf size too small (%d)\n", buf_size);
        return AVERROR_INVALIDDATA;
    }

    //Checking the first few characters for the magic number :|, aka ensure that this file is an MPFF type
    if (bytestream_get_byte(&buf) != 'M' ||
        bytestream_get_byte(&buf) != 'P' ||
	bytestream_get_byte(&buf) != 'F' ||
	bytestream_get_byte(&buf) != 'F') {
        av_log(avctx, AV_LOG_ERROR, "bad magic number\n");
        return AVERROR_INVALIDDATA;
    }

    //File size
    fsize = bytestream_get_le32(&buf);
	
	//If the size of the buffer is less than the size of the file
	//BMP tried to decode so we will as well.
    if (buf_size < fsize) {
        av_log(avctx, AV_LOG_ERROR, "not enough space (%d < %u), trying to decode anyway\n",
               buf_size, fsize);
        fsize = buf_size;
    }

    hsize  = bytestream_get_le32(&buf); /* header size */
    ihsize = bytestream_get_le32(&buf); /* info header size */

	//If the size of the info header and the size of the file header is greater than the header size we got then throw an error
    if (ihsize + 12LL > hsize) {
        av_log(avctx, AV_LOG_ERROR, "invalid header size %u\n", hsize);
        return AVERROR_INVALIDDATA;
    }
	
	//If the file size is less than the header size then we have a real problem.
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
    avctx->pix_fmt = AV_PIX_FMT_PAL8;

    //If there is not enough space then return a negative value to indicate failure
    if ((ret = ff_get_buffer(avctx, p, 0)) < 0)
        return ret;

    //Important frame data set
    p->pict_type = AV_PICTURE_TYPE_I;
    p->key_frame = 1;

    //Point the buffer to the start of the picture data (after the header)
    buf   = buf0 + hsize;
    // Calculate the non-header size of the image
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

	// Calculate the number of colors
    int colors = 1 << depth;

    memset(p->data[1], 0, 1024);
	
	// Calculate the location of the pallette
    buf = buf0 + 12 + ihsize; //palette location

	// Read in the pallette
    for (i = 0; i < colors; i++)
      {
	((uint32_t*)p->data[1])[i] = 0xFFU << 24 | bytestream_get_le32(&buf);
      }

	// Set the buffer to the end of the header
    buf = buf0 + hsize;

    //Set the pointer to the start of the picture data
    ptr      = p->data[0];
    //Get the linesize
    linesize = p->linesize[0];
    
    //If not a bit count of 8 then an error has occured

    if(depth == 8)
      {
	for (i = 0; i < avctx->height; i++) {
	  memcpy(ptr, buf, n);
	  buf += n;
	  ptr += linesize;
	}
      }
    else
      {
	av_log(avctx, AV_LOG_ERROR, "MPFF decoder is broken\n");
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

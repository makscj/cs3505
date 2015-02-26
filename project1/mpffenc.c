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


#include "libavutil/imgutils.h"
#include "libavutil/avassert.h"
#include "avcodec.h"
#include "bytestream.h"
#include "internal.h"

static av_cold int mpff_encode_init(AVCodecContext *avctx){
    switch (avctx->pix_fmt) {
    case AV_PIX_FMT_BGR24:
        avctx->bits_per_coded_sample = 24;
        break;
    default:
        av_log(avctx, AV_LOG_INFO, "unsupported pixel format\n");
        return AVERROR(EINVAL);
    }

    avctx->coded_frame = av_frame_alloc();
    if (!avctx->coded_frame)
        return AVERROR(ENOMEM);

    return 0;
}

static int mpff_encode_frame(AVCodecContext *avctx, AVPacket *pkt,
                            const AVFrame *pict, int *got_packet)
{
    const AVFrame * const p = pict;
    int n_bytes_image, n_bytes_per_row, n_bytes, i, hsize, ret;
    int pad_bytes_per_row;
    int bit_count = avctx->bits_per_coded_sample;
    uint8_t *ptr, *buf;

    avctx->coded_frame->pict_type = AV_PICTURE_TYPE_I;
    avctx->coded_frame->key_frame = 1;
    
    n_bytes_per_row = ((int64_t)avctx->width * (int64_t)bit_count + 7LL) >> 3LL; /*(width*bitcount + 7) / 8 */

    pad_bytes_per_row = (4 - n_bytes_per_row) & 3; /*Get the last 2 bits*/

    n_bytes_image = avctx->height * (n_bytes_per_row + pad_bytes_per_row);

    // STRUCTURE.field refer to the MSVC documentation for BITMAPFILEHEADER
    // and related pages.
#define SIZE_BITMAPFILEHEADER 12 ////////////////////////////:)
#define SIZE_BITMAPINFOHEADER 18 //
    hsize = SIZE_BITMAPFILEHEADER + SIZE_BITMAPINFOHEADER; //pal_entires might be zero
    
    //Total bytes
    n_bytes = n_bytes_image + hsize;

    //If you're out of mem? 
    if ((ret = ff_alloc_packet2(avctx, pkt, n_bytes)) < 0)
      {
        return ret;
      }

    //Buffer contains where we are saving data to
    buf = pkt->data;

    //Start file header 
    bytestream_put_byte(&buf, 'M');                   // BITMAPFILEHEADER.bfType
    bytestream_put_byte(&buf, 'P');                   // do.
    bytestream_put_byte(&buf, 'F');
    bytestream_put_byte(&buf, 'F');
    bytestream_put_le32(&buf, n_bytes);               // BITMAPFILEHEADER.bfSize
    bytestream_put_le32(&buf, hsize);                 // BITMAPFILEHEADER.bfOffBits
    //Start info header
    bytestream_put_le32(&buf, SIZE_BITMAPINFOHEADER); // BITMAPINFOHEADER.biSize
    bytestream_put_le32(&buf, avctx->width);          // BITMAPINFOHEADER.biWidth
    bytestream_put_le32(&buf, avctx->height);         // BITMAPINFOHEADER.biHeight
    bytestream_put_le16(&buf, bit_count);             // BITMAPINFOHEADER.biBitCount
    bytestream_put_le32(&buf, n_bytes_image);         // BITMAPINFOHEADER.biSizeImage

    //ptr is the start of the image data
    ptr = p->data[0];// + (avctx->height - 1) * p->linesize[0];

    //Move the buffer pointer to the end of the header
    buf = pkt->data + hsize;
    for(i = 0; i < avctx->height; i++) {
      //copy a row of bytes from ptr to buf
      memcpy(buf, ptr, n_bytes_per_row);
      //Shift buf pointer by a row
      buf += n_bytes_per_row;
      //Add padding REMOVE?
      memset(buf, 0, pad_bytes_per_row);
      //Move buffer up by pad amount
      buf += pad_bytes_per_row;
      ptr += p->linesize[0]; // ... and go forward
    }

    pkt->flags |= AV_PKT_FLAG_KEY;
    *got_packet = 1;
    return 0;
}

static av_cold int mpff_encode_close(AVCodecContext *avctx)
{
  av_frame_free(&avctx->coded_frame);
  return 0; 
}

AVCodec ff_mpff_encoder = {
    .name           = "mpff",
    .long_name      = NULL_IF_CONFIG_SMALL("MPFF image (a CS 3505 project)"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_MPFF,
    .init           = mpff_encode_init,
    .encode2        = mpff_encode_frame,
    .close          = mpff_encode_close,
    .pix_fmts       = (const enum AVPixelFormat[]){ AV_PIX_FMT_BGR24, AV_PIX_FMT_NONE },
};

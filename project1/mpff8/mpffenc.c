/*
 * MPFFimage format decoder
 * Copyright (c) 2015 John Ballard and Maks Cegielski-Johnson
 *
 * This file is part of FFmpeg. Created for CS 3505.
 * AV_PIX_FMT_RGB8
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

/*
Initialize the encoder, will check if we are given the correct pixel format. 
*/
static av_cold int mpff_encode_init(AVCodecContext *avctx){
    
	//Currently we only support BGR24 so if any other format appears then we want to throw an exception
	if(avctx->pix_fmt == AV_PIX_FMT_RGB8)
	{
		avctx->bits_per_coded_sample = 8;
	}
	else
	{
		av_log(avctx, AV_LOG_INFO, "unsupported pixel format\n");
        return AVERROR(EINVAL);
	}
	//Allocate frame memory space
    avctx->coded_frame = av_frame_alloc();
	//No memory for the frame error
    if (!avctx->coded_frame)
        return AVERROR(ENOMEM);

    return 0;
}

/*
Encode a given frame into our MPFF format.
avctx - contains context information about the image
pkt - the packet to which we will encode the image to
pict - the picture itself, declared constant since we are not changing it
got_packet - if we finish encoding the packet we will set this to 1
*/
static int mpff_encode_frame(AVCodecContext *avctx, AVPacket *pkt,
                            const AVFrame *pict, int *got_packet)
{
  //New variable for the pointer
    const AVFrame * const p = pict;
    //Different holder variables that we will set later once we get the information
    int n_bytes_image, n_bytes_per_row, n_bytes, i, hsize, ret;
    int pad_bytes_per_row, pal_entries = 0;
    const uint32_t *pal = NULL;
    uint32_t palette256[256];//DO WE?
    int bit_count = avctx->bits_per_coded_sample;
    //Pointers for the buffer
    uint8_t *ptr, *buf;

	
    //Important constants that need to be set.
    avctx->coded_frame->pict_type = AV_PICTURE_TYPE_I;
    avctx->coded_frame->key_frame = 1;

    av_assert1(bit_count == 8);
    avpriv_set_systematic_pal2(palette256, avctx->pix_fmt);
    pal = palette256;
    
    if (pal && !pal_entries) pal_entries = 1 << bit_count;

    //Calculate the number of bytes per row. We are using the funky math that was provided in the BMP encoder.
    //This is equivalent to the equation: [width * bitcount + 7]/8
    n_bytes_per_row = ((int64_t)avctx->width * (int64_t)bit_count + 7LL) >> 3LL; /*(width*bitcount + 7) / 8 */
    
    //Get the last 2 bits, we will use this for padding
    pad_bytes_per_row = (4 - n_bytes_per_row) & 3;
	
    //Get the number of bytes in the image 
    n_bytes_image = avctx->height * (n_bytes_per_row + pad_bytes_per_row);

#define SIZE_BITMAPFILEHEADER 12 //Define a constant for the size of the file header
#define SIZE_BITMAPINFOHEADER 18 //Define a constant for the size of the information header

    //Get the total header size
    hsize = SIZE_BITMAPFILEHEADER + SIZE_BITMAPINFOHEADER; 
    
    //Total bytes
    n_bytes = n_bytes_image + hsize;

    //If we are out of memory, return the ret value
    if ((ret = ff_alloc_packet2(avctx, pkt, n_bytes)) < 0)
      {
        return ret;
      }

    //Buffer contains the point where we are saving data to
    buf = pkt->data;
    
    //Start of file header 
    bytestream_put_byte(&buf, 'M');                   // The first 4 bytes of the header, to help ffmpeg 
    bytestream_put_byte(&buf, 'P');                   // Recognize this as an .mpff file
    bytestream_put_byte(&buf, 'F');
    bytestream_put_byte(&buf, 'F');
    bytestream_put_le32(&buf, n_bytes);               // Number of bytes in the file
    bytestream_put_le32(&buf, hsize);                 // The size of the total header
    //Start of info header
    bytestream_put_le32(&buf, SIZE_BITMAPINFOHEADER); // This size of the info header`
    bytestream_put_le32(&buf, avctx->width);          // Width of the image
    bytestream_put_le32(&buf, avctx->height);         // Height of the image 
    bytestream_put_le16(&buf, bit_count);             // Depth of the image 
    bytestream_put_le32(&buf, n_bytes_image);         // The size of the image
    for (i = 0; i < pal_entries; i++)
      {
        bytestream_put_le32(&buf, pal[i] & 0xFFFFFF);
      }
    //ptr is the start of the image data
    ptr = p->data[0];

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
	  //Move the pointer to the next line
      ptr += p->linesize[0];
    }
	//Set the flags of the packet
    pkt->flags |= AV_PKT_FLAG_KEY;
	//Set the got_packet term to 1 since we finished
    *got_packet = 1;
	//Return 0 since there were no exceptions
    return 0;
}

/*
Close the encoding and free the frame. Probably important for ffmpeg.
*/
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
    .pix_fmts       = (const enum AVPixelFormat[]){ AV_PIX_FMT_RGB8, AV_PIX_FMT_NONE },
};

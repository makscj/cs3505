/*
MPFF image format encoder
Copyright (c) 2015 John Ballard and Maks Cegielski-Johnson

This file is part of FFmpeg (soon).

CS 3505 Spring 2015 


*/


#include "libavutil/imgutils.h"
#include "libavutil/avassert.h"
#include "avcodec.h"
#include "bmp.h"
#include "bytestream.h"
#include "internal.h"

static const uint32_t monoblack_pal[] = { 0x000000, 0xFFFFFF };
static const uint32_t rgb565_masks[]  = { 0xF800, 0x07E0, 0x001F };
static const uint32_t rgb444_masks[]  = { 0x0F00, 0x00F0, 0x000F };


static av_cold int mpff_encode_init(AVCodecContext *avctx){
    switch (avctx->pix_fmt) {
    case AV_PIX_FMT_BGRA:
        avctx->bits_per_coded_sample = 32;
        break;
    case AV_PIX_FMT_BGR24:
        avctx->bits_per_coded_sample = 24;
        break;
    case AV_PIX_FMT_RGB555:
    case AV_PIX_FMT_RGB565:
    case AV_PIX_FMT_RGB444:
        avctx->bits_per_coded_sample = 16;
        break;
    case AV_PIX_FMT_RGB8:
    case AV_PIX_FMT_BGR8:
    case AV_PIX_FMT_RGB4_BYTE:
    case AV_PIX_FMT_BGR4_BYTE:
    case AV_PIX_FMT_GRAY8:
    case AV_PIX_FMT_PAL8:
        avctx->bits_per_coded_sample = 8;
        break;
    case AV_PIX_FMT_MONOBLACK:
        avctx->bits_per_coded_sample = 1;
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
    int n_bytes_image, n_bytes_per_row, n_bytes, i, n, hsize, ret;
    const uint32_t *pal = NULL;
    uint32_t palette256[256];
    int pad_bytes_per_row, pal_entries = 0, compression = BMP_RGB;
    int bit_count = avctx->bits_per_coded_sample;
    uint8_t *ptr, *buf;

    avctx->coded_frame->pict_type = AV_PICTURE_TYPE_I;
    avctx->coded_frame->key_frame = 1;
    switch (avctx->pix_fmt) {
    case AV_PIX_FMT_RGB444:
        compression = BMP_BITFIELDS;
        pal = rgb444_masks; // abuse pal to hold color masks
        pal_entries = 3;
        break;
    case AV_PIX_FMT_RGB565:
        compression = BMP_BITFIELDS;
        pal = rgb565_masks; // abuse pal to hold color masks
        pal_entries = 3;
        break;
    case AV_PIX_FMT_RGB8:
    case AV_PIX_FMT_BGR8:
    case AV_PIX_FMT_RGB4_BYTE:
    case AV_PIX_FMT_BGR4_BYTE:
    case AV_PIX_FMT_GRAY8:
        av_assert1(bit_count == 8);
        avpriv_set_systematic_pal2(palette256, avctx->pix_fmt);
        pal = palette256;
        break;
    case AV_PIX_FMT_PAL8:
        pal = (uint32_t *)p->data[1];
        break;
    case AV_PIX_FMT_MONOBLACK:
        pal = monoblack_pal;
        break;
    }
    if (pal && !pal_entries) pal_entries = 1 << bit_count;
    n_bytes_per_row = ((int64_t)avctx->width * (int64_t)bit_count + 7LL) >> 3LL;
    pad_bytes_per_row = (4 - n_bytes_per_row) & 3;
    n_bytes_image = avctx->height * (n_bytes_per_row + pad_bytes_per_row);

    // STRUCTURE.field refer to the MSVC documentation for BITMAPFILEHEADER
    // and related pages.
#define SIZE_BITMAPFILEHEADER 14
#define SIZE_BITMAPINFOHEADER 40
    hsize = SIZE_BITMAPFILEHEADER + SIZE_BITMAPINFOHEADER + (pal_entries << 2);
    n_bytes = n_bytes_image + hsize;
    if ((ret = ff_alloc_packet2(avctx, pkt, n_bytes)) < 0)
        return ret;
    buf = pkt->data;
    bytestream_put_byte(&buf, 'B');                   // BITMAPFILEHEADER.bfType
    bytestream_put_byte(&buf, 'M');                   // do.
    bytestream_put_le32(&buf, n_bytes);               // BITMAPFILEHEADER.bfSize
    bytestream_put_le16(&buf, 0);                     // BITMAPFILEHEADER.bfReserved1
    bytestream_put_le16(&buf, 0);                     // BITMAPFILEHEADER.bfReserved2
    bytestream_put_le32(&buf, hsize);                 // BITMAPFILEHEADER.bfOffBits
    bytestream_put_le32(&buf, SIZE_BITMAPINFOHEADER); // BITMAPINFOHEADER.biSize
    bytestream_put_le32(&buf, avctx->width);          // BITMAPINFOHEADER.biWidth
    bytestream_put_le32(&buf, avctx->height);         // BITMAPINFOHEADER.biHeight
    bytestream_put_le16(&buf, 1);                     // BITMAPINFOHEADER.biPlanes
    bytestream_put_le16(&buf, bit_count);             // BITMAPINFOHEADER.biBitCount
    bytestream_put_le32(&buf, compression);           // BITMAPINFOHEADER.biCompression
    bytestream_put_le32(&buf, n_bytes_image);         // BITMAPINFOHEADER.biSizeImage
    bytestream_put_le32(&buf, 0);                     // BITMAPINFOHEADER.biXPelsPerMeter
    bytestream_put_le32(&buf, 0);                     // BITMAPINFOHEADER.biYPelsPerMeter
    bytestream_put_le32(&buf, 0);                     // BITMAPINFOHEADER.biClrUsed
    bytestream_put_le32(&buf, 0);                     // BITMAPINFOHEADER.biClrImportant
    for (i = 0; i < pal_entries; i++)
        bytestream_put_le32(&buf, pal[i] & 0xFFFFFF);
    // BMP files are bottom-to-top so we start from the end...
    ptr = p->data[0] + (avctx->height - 1) * p->linesize[0];
    buf = pkt->data + hsize;
    for(i = 0; i < avctx->height; i++) {
        if (bit_count == 16) {
            const uint16_t *src = (const uint16_t *) ptr;
            uint16_t *dst = (uint16_t *) buf;
            for(n = 0; n < avctx->width; n++)
                AV_WL16(dst + n, src[n]);
        } else {
            memcpy(buf, ptr, n_bytes_per_row);
        }
        buf += n_bytes_per_row;
        memset(buf, 0, pad_bytes_per_row);
        buf += pad_bytes_per_row;
        ptr -= p->linesize[0]; // ... and go back
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
  .name = "mpff",
  .long_name = NULL_IF_CONFIG_SMALL("MPFF image (a CS 3505 project)"),
  .type = AVMEDIA_TYPE_VIDEO,
  .id = AV_CODEC_ID_MPFF,
  .close = mpff_encode_close,
  .encode2 = mpff_encode_frame,
  .capabilities = CODEC_CAP_DR1,
};


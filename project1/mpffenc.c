/*
MPFF image format encoder
Copyright (c) 2015 John Ballard and Maks Cegielski-Johnson

This file is part of FFmpeg (soon).

CS 3505 Spring 2015 


*/


#include "avcodec.h"
#include "internal.h"


AVCodec ff_mpff_encoder = {
  .name = "mpff",
  .long_name = NULL_IF_CONFIG_SMALL("MAKS PHOTO FILE FORMAT"),
  .type = AVMEDIA_TYPE_VIDEO,
  .id = AV_CODEC_ID_MPFF,
};


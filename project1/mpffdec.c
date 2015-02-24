/*
MPF image format decoder

TODO ADD COMMENTS PLEASE
*/



#include "avcodec.h"
#include "internal.h"





AVCodec ff_mpff_decoder = {

  .name = "mpff",
  .long_name = NULL_IF_CONFIG_SMALL("MAKS PHOTO FILE FORMAT"),
  .type = AVMEDIA_TYPE_VIDEO,
  .id = AV_CODEC_ID_BMP,
};

#ifndef IMAGE_VALIDATE_H
#define IMAGE_VALIDATE_H

#include "types.h"
#include "image_encode.h"
#include "image_decode.h"

Status validate_arguments_encode(char *argv[], EncodeInfo *encInfo);
Status validate_arguments_decode(char *argv[], DecodeInfo *decInfo);

#endif
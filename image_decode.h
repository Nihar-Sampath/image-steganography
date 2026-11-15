#ifndef IMAGE_DECODE_H
#define IMAGE_DECODE_H

#include "types.h"

typedef struct _DecInfo
{
    // Stego Image Info
    char *stego_image_fname;
    FILE *stego_image_fptr;

    // Secret File Info
    char *usecret_fname;
    FILE *usecret_fptr;
    uint usecret_file_data_size;

    uint usecret_extn_size;
    char *usecret_file_extn;

    // Magic String Info
    uint magic_string_len;
    char *magic_string;
    

}DecodeInfo;

Status do_decode(DecodeInfo *decInfo);

Status open_files_decode_stego(DecodeInfo *decInfo);
Status open_files_decode_secret(DecodeInfo *decInfo);
void close_files_decode(DecodeInfo *decInfo);

void decode_size_of_magic_string(DecodeInfo *decInfo);
void decode_magic_string(DecodeInfo *decInfo);

void decode_size_of_secret_ext(DecodeInfo *decInfo);
void decode_secret_ext(DecodeInfo *decInfo);

void decode_secret_data_size(DecodeInfo *decInfo);
void decode_secret_data(DecodeInfo *decInfo);

void skip_header(FILE *fptr);

uint decode_int(uchar *buff);
uchar decode_char(uchar *buff);
#endif
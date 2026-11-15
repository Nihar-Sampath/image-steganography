#ifndef IMAGE_ENCODE_H
#define IMAGE_ENCODE_H

#include "types.h"

typedef struct _EncInfo
{
    // Source Image Info
    char *src_image_fname;
    FILE *src_image_fptr;
    uint image_capacity;

    // Secret File Info
    char *secret_fname;
    FILE *secret_fptr;
    uint secret_file_data_size;

    char *secret_file_ext;
    int secret_file_ext_size;
    
    // Magic String Info
    char *magic_string;
    int magic_string_len;

    // Output Image Info
    char *stego_image_fname;
    FILE *stego_image_fptr;

}EncodeInfo;

Status do_encode(EncodeInfo *encInfo);

Status open_files_encode(EncodeInfo *encInfo);
void close_files_encode(EncodeInfo *encInfo);

Status check_capacity(EncodeInfo *encInfo);

uint get_image_capacity(FILE *fptr);
uint get_secret_file_data_size(FILE *fptr);

void copy_header(FILE *src_fptr, FILE *stego_fptr);

void encode_magic_string_len(EncodeInfo *encInfo);
void encode_magic_string(EncodeInfo *encInfo);

void encode_secret_extn_size(EncodeInfo *encInfo);
void encode_secret_extn(EncodeInfo *encInfo);

void encode_secret_data_size(EncodeInfo *encInfo);
void encode_secret_data(EncodeInfo *encInfo);

void copy_remaining(FILE *src_fptr, FILE *stego_fptr);

void encode_integer(uchar *buff, uint size);
void encode_character(uchar *buff, uchar ch);
#endif
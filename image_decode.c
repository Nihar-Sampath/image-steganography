#include "image_decode.h"


Status open_files_decode_stego(DecodeInfo *decInfo)
{
    decInfo->stego_image_fptr=fopen(decInfo->stego_image_fname, "r");
    if(decInfo->stego_image_fptr==NULL)
    {
        printf("Couldn't open \"%s\" file. Make sure the file exists\n", decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

Status open_files_decode_secret(DecodeInfo *decInfo)
{
    decInfo->usecret_fname=realloc(decInfo->usecret_fname, decInfo->usecret_extn_size);
    strcat(decInfo->usecret_fname, decInfo->usecret_file_extn);
    // printf("secret file extention after merging - %s\n", decInfo->usecret_fname);
    decInfo->usecret_fptr=fopen(decInfo->usecret_fname, "w");
    if(decInfo->usecret_fptr==NULL)
    {
        printf("Couldn't create \"%s\" file\n", decInfo->usecret_fname);
        return e_failure;
    }
}
void close_files_decode(DecodeInfo *decInfo)
{
    // printf("Here is the error\n");
    if(decInfo->usecret_extn_size!=0)
        fclose(decInfo->usecret_fptr);
    fclose(decInfo->stego_image_fptr);
    // printf("There is no error here\n");
}

void skip_header(FILE *fptr)
{
    fseek(fptr, 54, SEEK_SET);
    // printf("CURRENT STEGO POSITION - %ld\n", ftell(fptr));
}

Status do_decode(DecodeInfo *decInfo)
{
    if(open_files_decode_stego(decInfo)==e_failure)
        return e_failure;

    skip_header(decInfo->stego_image_fptr);

    decode_size_of_magic_string(decInfo);
    decode_magic_string(decInfo);

    char str[100];
    printf("Please Enter the Magic String : ");
    scanf(" %[^\n]", str);

    decInfo->usecret_extn_size=0;
    // printf("Extracted Magic String: %s\nMagic String Size: %u\n", decInfo->magic_string, decInfo->magic_string_len);
    if(strcmp(decInfo->magic_string, str)!=0)
    {
        printf("Invalid Magic String.\n");
        close_files_decode(decInfo);
        return e_failure;
    }

    decode_size_of_secret_ext(decInfo);
    decode_secret_ext(decInfo);

    if(open_files_decode_secret(decInfo)==e_failure)
        e_failure;
    
    decode_secret_data_size(decInfo);
    decode_secret_data(decInfo);

    close_files_decode(decInfo);
}

void decode_secret_data_size(DecodeInfo *decInfo)
{
    uchar buff[32];
    fread(buff, 32, 1, decInfo->stego_image_fptr);

    decInfo->usecret_file_data_size=decode_int(buff);
}

void decode_size_of_secret_ext(DecodeInfo *decInfo)
{
    uchar buff[32];
    fread(buff, 32, 1, decInfo->stego_image_fptr);

    decInfo->usecret_extn_size=decode_int(buff);
    decInfo->usecret_file_extn=malloc(decInfo->usecret_extn_size+1);
}

void decode_size_of_magic_string(DecodeInfo *decInfo)
{
    uchar buff[32];
    fread(buff, 32, 1, decInfo->stego_image_fptr);
    
    decInfo->magic_string_len=decode_int(buff);
    decInfo->magic_string=malloc(decInfo->magic_string_len + 1);
}

void decode_secret_data(DecodeInfo *decInfo)
{
    for(int i=0; i<decInfo->usecret_file_data_size; i++)
    {
        uchar buff[8];
        fread(buff, 8, 1, decInfo->stego_image_fptr);
        
        char c=decode_char(buff);
        // printf("Secret data - [%d] - %c\n", i, c);
        putc(c, decInfo->usecret_fptr);
    }
}

void decode_secret_ext(DecodeInfo *decInfo)
{
    int i;
    for(i=0; i<decInfo->usecret_extn_size; i++)
    {
        uchar buff[8];
        fread(buff, 8, 1, decInfo->stego_image_fptr);

        decInfo->usecret_file_extn[i]=decode_char(buff);
        // printf("Secret - ext [%d]-%c\n", i, decInfo->usecret_file_extn[i]);
    }
    decInfo->usecret_file_extn[i]='\0';
}

void decode_magic_string(DecodeInfo *decInfo)
{
    int i;
    for(i=0; i<decInfo->magic_string_len; i++)
    {
        uchar buff[8];
        fread(buff, 8, 1, decInfo->stego_image_fptr);
        // printf("Inside Magic String - %c\n", decode_char(buff));
        decInfo->magic_string[i]=decode_char(buff);
        // printf("Magic String [i] - %c\n", decInfo->magic_string[i]);
    }
    decInfo->magic_string[i]='\0';
}

uchar decode_char(uchar *buff)
{
    uchar c=0;
    for(int i=0; i<8; i++)
    {
        c=c|((buff[i]&1)<<i);
    }
    return c;
}
uint decode_int(uchar *buff)
{
    uint size=0;
    for(int i=0; i<32; i++)
    {
        size=size|((buff[i]&1)<<i);
    }
    return size;
}
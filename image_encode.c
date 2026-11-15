#include "image_encode.h"

Status open_files_encode(EncodeInfo *encInfo)
{
    encInfo->src_image_fptr=fopen(encInfo->src_image_fname, "r");
    if(encInfo->src_image_fptr==NULL)
    {
        printf("Couldn't Open the \"%s\" File\n", encInfo->src_image_fname);
        return e_failure;
    }

    encInfo->secret_fptr=fopen(encInfo->secret_fname, "r");
    if(encInfo->secret_fptr==NULL)
    {
        printf("Couldn't Open the \"%s\" File\n", encInfo->secret_fname);
        return e_failure;
    }

    encInfo->stego_image_fptr=fopen(encInfo->stego_image_fname, "w");
    if(encInfo->stego_image_fptr==NULL)
    {
        printf("Couldn't Create the \"%s\" File\n", encInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}

uint get_image_capacity(FILE *fptr)
{
    uint width, height;
    fseek(fptr, 18, SEEK_SET);

    fread(&width, sizeof(int), 1, fptr);
    fread(&height, sizeof(int), 1, fptr);

    // printf("Width - %u\nHeight - %u\n", width, height);
    rewind(fptr);

    return width*height*3;
}

uint get_secret_file_data_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint size=ftell(fptr);
    rewind(fptr);

    return size;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity=get_image_capacity(encInfo->src_image_fptr);
    encInfo->secret_file_data_size=get_secret_file_data_size(encInfo->secret_fptr);

    char magic_strings[100];
    printf("Enter Magic String: ");
    scanf(" %[^\n]", magic_strings);

    encInfo->magic_string_len=strlen(magic_strings);
    encInfo->magic_string=malloc(encInfo->magic_string_len+1);

    strcpy(encInfo->magic_string, magic_strings);
    // printf("Magic String")
    uint total_size = (encInfo->magic_string_len)*8 + 32 + (encInfo->secret_file_data_size)*8 + 32 + (encInfo->secret_file_ext_size)*8 + 32;

    if(encInfo->image_capacity>total_size)
    {
        // printf("Image Capacity is enough\n");
        return e_success;
    }
    
    printf("Not Enough Capacity\n");
    return e_failure;
}


void copy_header(FILE *src_fptr, FILE *stego_fptr)
{
    // printf("src-%ld\nstego-%ld\n", ftell(src_fptr), ftell(stego_fptr));

    uchar buff[54];
    fread(buff, 54, 1, src_fptr);
    fwrite(buff, 54, 1, stego_fptr);

    // printf("src-%ld\nstego-%ld\n", ftell(src_fptr), ftell(stego_fptr));
}

void copy_remaining(FILE *src_fptr, FILE *stego_fptr)
{
    int c;

    while((c=getc(src_fptr))!=EOF)
    {
        putc(c, stego_fptr);
    }
}

void close_files_encode(EncodeInfo *encInfo)
{
    fclose(encInfo->src_image_fptr);
    fclose(encInfo->secret_fptr);
    fclose(encInfo->stego_image_fptr);
}

Status do_encode(EncodeInfo *encInfo)
{
    if(open_files_encode(encInfo)==e_failure)
        return e_failure;

    if(check_capacity(encInfo)==e_failure)
        return e_failure;

    copy_header(encInfo->src_image_fptr, encInfo->stego_image_fptr);

    encode_magic_string_len(encInfo);
    encode_magic_string(encInfo);

    encode_secret_extn_size(encInfo);
    encode_secret_extn(encInfo);

    encode_secret_data_size(encInfo);
    encode_secret_data(encInfo);

    copy_remaining(encInfo->src_image_fptr, encInfo->stego_image_fptr);

    close_files_encode(encInfo);
}


void encode_magic_string_len(EncodeInfo *encInfo)
{
    uchar sizebuff[32];

    fread(sizebuff, 32, 1, encInfo->src_image_fptr);

    encode_integer(sizebuff, encInfo->magic_string_len);

    fwrite(sizebuff, 32, 1, encInfo->stego_image_fptr);
    // printf("Magic String Length in Encode: %u\n", encInfo->magic_string_len);

}

void encode_secret_extn_size(EncodeInfo *encInfo)
{
    uchar sizebuff[32];

    fread(sizebuff, 32, 1, encInfo->src_image_fptr);

    encode_integer(sizebuff, encInfo->secret_file_ext_size);

    fwrite(sizebuff, 32, 1, encInfo->stego_image_fptr);
}

void encode_secret_data_size(EncodeInfo *encInfo)
{
    uchar sizebuff[32];

    fread(sizebuff, 32, 1, encInfo->src_image_fptr);

    encode_integer(sizebuff, encInfo->secret_file_data_size);

    fwrite(sizebuff, 32, 1, encInfo->stego_image_fptr);
}

void encode_magic_string(EncodeInfo *encInfo)
{
    for(int i=0; i<encInfo->magic_string_len; i++)
    {
        unsigned char buff[8];
        fread(buff, 8, 1, encInfo->src_image_fptr);
        // printf("buff before-%s\n", buff);
        // printf("In encode Magic String[%d] - %c\n", i, encInfo->magic_string[i]);
        encode_character(buff, encInfo->magic_string[i]);
        // printf("buff after-%s\n", buff);
        fwrite(buff, 8, 1, encInfo->stego_image_fptr);
    }
}

void encode_secret_extn(EncodeInfo *encInfo)
{
    for(int i=0; i<encInfo->secret_file_ext_size; i++)
    {
        uchar buff[8];
        fread(buff, 8, 1, encInfo->src_image_fptr);
        encode_character(buff, encInfo->secret_file_ext[i]);
        fwrite(buff, 8, 1, encInfo->stego_image_fptr);
    }
}

void encode_secret_data(EncodeInfo *encInfo)
{
    for(int i=0; i<encInfo->secret_file_data_size; i++)
    {
        uchar buff[8];
        fread(buff, 8, 1, encInfo->src_image_fptr);
        uchar c=getc(encInfo->secret_fptr);
        // printf("Secret data - %c\n", c);
        encode_character(buff, c);
        fwrite(buff, 8, 1, encInfo->stego_image_fptr);
    }
}

void encode_character(uchar *buff, uchar ch)
{
    for(int i=0; i<8; i++)
    {
        buff[i]=buff[i]&(~1);
        uint temp=(ch>>i)&1;
        buff[i]=buff[i]|temp;
    }
}

void encode_integer(uchar *buff, uint size)
{
    for(int i=0; i<32; i++)
    {
        buff[i]=buff[i]&(~1);
        uint temp=(size>>i)&1;
        buff[i]=buff[i]|temp;
    }
}
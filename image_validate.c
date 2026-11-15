#include "types.h"
#include "image_validate.h"

Status validate_arguments_encode(char *argv[], EncodeInfo *encInfo)
{
    // printf("Entered validate arguments\n");
    if(strcmp(&argv[2][strlen(argv[2])-4], ".bmp")==0)
    {
        encInfo->src_image_fname=malloc(strlen(argv[2])+1);
        strcpy(encInfo->src_image_fname, argv[2]);
        // printf("First argument validated\n");
    }
    else
    {
        printf("\"%s\" is invalid. Please enter \".bmp\" files only\n", argv[2]);
        return e_failure;
    }

    if(strrchr(argv[3],'.')!=NULL)
    {
        encInfo->secret_fname=malloc(strlen(argv[3])+1);
        strcpy(encInfo->secret_fname, argv[3]);

        encInfo->secret_file_ext_size=strlen(strrchr(argv[3], '.'));
        encInfo->secret_file_ext=malloc(encInfo->secret_file_ext_size+1);
        strcpy(encInfo->secret_file_ext, strrchr(argv[3], '.'));
        // printf("Inside encode validation fname - %s, extn name - %s, argv[3] - %s\n",encInfo->secret_fname, encInfo->secret_file_ext, argv[3]);
    }
    else
    {
        printf("\"%s\" is invalid. Please enter \"file with an extention\"\n", argv[3]);
        return e_failure;
    }

    if(argv[4]==NULL)
    {
        encInfo->stego_image_fname=malloc(strlen("Stego.bmp")+1);
        strcpy(encInfo->stego_image_fname, "Stego.bmp");
    }
    else
    {
        if(strcmp(&argv[4][strlen(argv[4])-4], ".bmp")==0)
        {
            encInfo->stego_image_fname=malloc(strlen(argv[4])+1);
            strcpy(encInfo->stego_image_fname, argv[4]);
        }
        else
        {
            printf("\"%s\" is invalid. Please enter \".bmp\" files only\n", argv[4]);
            return e_failure;
        }
    }

    return e_success;
}

Status validate_arguments_decode(char *argv[], DecodeInfo *decInfo)
{
    if(strcmp(&argv[2][strlen(argv[2])-4], ".bmp")==0)
    {
        decInfo->stego_image_fname=malloc(strlen(argv[2])+1);
        strcpy(decInfo->stego_image_fname, argv[2]);
        // printf("Decode Image File Copied Successfully - %s\n", decInfo->stego_image_fname);
    }
    else
    {
        printf("Please enter \".bmp\" files only.\n");
        return e_failure;
    }

    if(argv[3]==NULL)
    {
        decInfo->usecret_fname=malloc(strlen("Output")+1);
        strcpy(decInfo->usecret_fname, "Output");
        // printf("Output Secret File name copied successfully - %s\n", decInfo->usecret_fname);
    }
    else
    {
        // if(strcmp(&argv[3][strlen(argv[3])-4], ".txt")==0)
        // {
            decInfo->usecret_fname=malloc(strlen(argv[3])+1);
            strcpy(decInfo->usecret_fname, argv[3]);
            // printf("Output Secret File name copied successfully - %s\n", decInfo->usecret_fname);
        // }
        // else
        // {
            // printf("Please Enter \".txt\" files only\n");
            // return e_failure;
        // }
    }

    return e_success;
}
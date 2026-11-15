#include "types.h"
#include "image_encode.h"
#include "image_decode.h"
#include "image_validate.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if(check_mode(argc, argv)==e_encode)
    {
        // printf("Encode Mode\n");
        if(!(argc==4 || argc==5))
        {
            printf("Invalid Number of arguments.\n");
            help();
            return e_failure;
        }

        if(validate_arguments_encode(argv, &encInfo)==e_success)
        {
            // printf("All arguments are validates successfully\n");
            // printf("Source : %s\n", encInfo.src_image_fname);
            // printf("Secret : %s\n", encInfo.secret_fname);
            // printf("Output : %s\n", encInfo.stego_image_fname);
            if(do_encode(&encInfo)==e_failure)
            {
                printf("Encoding failed\n");
                return e_failure;
            }
            printf("Encoding Successful\n");
        }
        else
        {
            help();
            return e_failure;
        }
    }
    else if(check_mode(argc, argv)==e_decode)
    {
        if(!(argc==3 || argc==4))
        {
            printf("Invalid Number of arguments.\n");
            help();
            return e_failure;
        }
        if(validate_arguments_decode(argv, &decInfo)==e_success)
        {
            if(do_decode(&decInfo)==e_failure)
            {
                printf("Decoding Failed\n");
                return e_failure;
            }
            printf("Decoding successful\n");
        }
        else
        {
            help();
            return e_failure;
        }
    }
    else
    {
        printf("Invalid Operation Type.\n");
        help();
        return e_failure;
    }
}
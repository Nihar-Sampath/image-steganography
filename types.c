#include "types.h"

Operationtype check_mode(int argc, char *argv[])
{
    if(argc<3 || argc>5)
        return e_invalid;

    if(strcmp(argv[1], "-e")==0)
        return e_encode;
    else if(strcmp(argv[1], "-d")==0)
        return e_decode;
    else
        return e_invalid; 
}

void help()
{
    printf("Command for encoding:\n");
    printf("Usage: ./a.out -e <source_file_name.extension> <secret_file_name.secret_extension> [output_encoded_file_name.same_extension_as_source]\n");
    printf("Note: The output encoded file name is optional.\n\n");

    printf("Command for decoding:\n");
    printf("Usage: ./a.out -d <encoded_file_name.extension> [output_file_name]\n");
    printf("Note: You do not need to specify the extension of the secret file during decoding.\n");
    printf("      The output file name is also optional.\n");
}

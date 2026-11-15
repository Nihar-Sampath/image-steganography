#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned int uint;

typedef enum
{
    e_success,
    e_failure
}Status;

typedef enum
{
    e_encode,
    e_decode,
    e_invalid
}Operationtype;

Operationtype check_mode(int argc, char *argv[]);
void help();

#endif
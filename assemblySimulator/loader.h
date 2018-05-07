// Header file for loader.c
// Da Jason Sun (38204849)

#include <stdio.h>
#include <stdlib.h>
#include "LC4.h"


#define MEMORY_SIZE 65535
#define WORD_FGETC 2

#define N_SECTIONS 5
#define CODE 0xCADE
#define DATA 0xDADA
#define SYMBOL 0xC3B7
#define FILE_NAME 0xF17E
#define LINE_NUMBER 0x715E

void store_mem (FILE *fp, Data *d);
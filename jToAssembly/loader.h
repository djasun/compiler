// Header file for loader.c
// Jason Sun (38204849)
// 12/11/2017

#include "token.h"

#define MAX_COMMENT 1024

token *create_token (int type);
void initialise_token (token *t);
void free_token (token *t);
void load_file (FILE *jFile, FILE *asmFile, IfCount *head);
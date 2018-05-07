// Header file for LC4.c
// Da Jason Sun (38204849)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MEMORY_SIZE 65535
#define N_REGISTERS 8

typedef struct data {
	unsigned short mem[MEMORY_SIZE];
	unsigned short reg[N_REGISTERS];
	unsigned short pc;
	unsigned short psr;
} Data;

Data *start_data (void);
void end_data (Data *lc4);
void initialise_data (Data *lc4);
void print_line (FILE *fp, Data *lc4);
void set_nzp (Data *lc4, unsigned short n);
void check_action (Data *lc4);
void analyse_code (FILE *out, Data *lc4);
void print_image (FILE *fp, Data *lc4);

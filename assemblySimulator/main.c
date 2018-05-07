// C file that contains the main routine and any routines needed to run 
// the program or parse command line arguments
// Da Jason Sun (38204849)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "loader.h"

#define MEMORY_SIZE 65535
#define N_REGISTERS 8

#define CODE 0xCADE
#define DATA 0xDADA
#define SYMBOL 0xC3B7
#define FILE_NAME 0xF17E
#define LINE_NUMBER 0x715E

int main (int argc, char *argv[]) {
	if (argc < 3) {
		fprintf (stderr, "argc < 3\n");
		exit (1);
	}

	Data *lc4 = start_data();
	
	// Load all obj files
	for (int i = 2; i < argc; i++) {
		FILE *fp = fopen (argv[i], "r");
		if (fp == 0) {
			fprintf (stderr, "Unable to open %s\n", argv[i]);
			end_data (lc4);
			exit (1);
		}
		store_mem (fp, lc4);
		fclose (fp);
	}

	FILE *out = fopen (argv[1], "w");
	if (out == 0) {
		fprintf (stderr, "Unable to open %s\n", argv[1]);
		end_data (lc4);
		exit (1);
	}

	// Run program
	do {
		analyse_code (out, lc4);
	} while (lc4->pc != 0x80FF);
	analyse_code (out, lc4);

	FILE *img = fopen ("image.ppm", "w");
	if (img == 0) {
		fprintf (stderr, "Unable to open image.ppm\n");
		end_data (lc4);
		exit (1);
	}
	print_image (img, lc4);
	fclose (img);
	
	fclose (out);
	end_data (lc4);

	return EXIT_SUCCESS;
}
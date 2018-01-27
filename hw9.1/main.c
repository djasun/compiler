// Main function for hw9
// Jason Sun (38204849)
// 12/11/2017

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loader.h"

#define MAX_FILE_CHAR 1024

int main (int argc, char *argv[]) {
	// Open .j file
	FILE *jFile = fopen (argv[1], "r");
	if (jFile == 0) {
		fprintf (stderr, "Unable to open %s\n", argv[1]);
		exit(1);
	}

	// Determine .asm file name
	char asmFileName[MAX_FILE_CHAR] = {0};
	strcpy (asmFileName, argv[1]);
	char *temp = asmFileName;
	while (temp != '\0') {
		if (*temp == '.') {
			*temp = 0;
			break;
		}
		temp++;
	}
	strcat (asmFileName, ".asm");

	// Open .asm file
	FILE *asmFile = fopen (asmFileName, "w");
	if (asmFile == 0) {
		fprintf (stderr, "Unable to open %s\n", asmFileName);
		exit (1);
	}

	// Load .j and write into .asm file
	IfCount *ic = create_if ();
	fprintf (asmFile, "\t\t\t\t;; %s\n\n", asmFileName);
	load_file (jFile, asmFile, ic);

	// Close files
	fclose (jFile);
	fclose (asmFile);

	// Free nodes that have yet to be freed
	if (ic->head != NULL) {
		free_if (ic);
	}

	return EXIT_SUCCESS;
}
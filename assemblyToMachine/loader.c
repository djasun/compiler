// C file that contains the routines needed for opening and loading the object files
// Da Jason Sun (38204849)

#include "loader.h"


// Function that stores all data from obj in d->mem array
void store_mem (FILE *fp, Data *d) {
	int temp = 0, i = 0, addr = 0, n = 0;
	do {
		temp = 16 * 16 * fgetc (fp);
		temp += fgetc (fp);

		if (temp == CODE) {
			addr = 16 * 16 * fgetc (fp);
			addr += fgetc (fp);
			n = 16 * 16 * fgetc (fp);
			n += fgetc (fp);
			for (i = 0; i < n; i++) {
				d->mem[addr + i] = 16 * 16 * fgetc (fp);
				d->mem[addr + i] += fgetc (fp);
			}
		} else if (temp == DATA) {
			addr = 16 * 16 * fgetc (fp);
			addr += fgetc (fp);
			n = 16 * 16 * fgetc (fp);
			n += fgetc (fp);
			for (i = 0; i < n; i++) {
				d->mem[addr + i] = 16 * 16 * fgetc (fp);
				d->mem[addr + i] += fgetc (fp);
			}
		} else if (temp == SYMBOL) {
			fgetc (fp);
			fgetc (fp);
			n = 16 * 16 * fgetc (fp);
			n += fgetc (fp);
			for (i = 0; i < n; i++) {
				fgetc (fp);
			}
		} else if (temp == FILE_NAME) {
			n = 16 * 16 * fgetc (fp);
			n += fgetc (fp);
			for (i = 0; i < n; i++) {
				fgetc (fp);
			}
		} else if (temp == LINE_NUMBER) {
			fgetc (fp);
			fgetc (fp);
			fgetc (fp);
			fgetc (fp);
			fgetc (fp);
			fgetc (fp);
		}
		if (feof (fp)) {
			break;
		}
	} while (1);
}
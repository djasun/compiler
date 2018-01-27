// C file that contains the routines for decoding instructions
// Da Jason Sun (38204849)

#include "LC4.h"

// Function that creates, allocates memory and initialises the lc4 struct
Data *start_data (void) {
	Data *lc4 = malloc (sizeof (Data));
	if (lc4 == 0) {
		fprintf (stderr, "Out of memory\n");
		exit (1);
	}
	initialise_data (lc4);
	return lc4;
}

// Function that frees the memory allocated for the lc4 struct
void end_data (Data *lc4) {
	free (lc4);
}

// Function that initialises the lc4 struct
void initialise_data (Data *lc4) {
	for (int i = 0; i < MEMORY_SIZE; i++) {
		lc4->mem[i] = 0;
	}
	for (int i = 0; i < N_REGISTERS; i++) {
		lc4->reg[i] = 0;
	}
	lc4->pc = 0x8200;
	lc4->psr = 0x8002;
}

// Function that prints PC and MEM values to output file 
void print_line (FILE *fp, Data *lc4) {
	fwrite(&lc4->pc, 2, 1, fp);
	fwrite (&lc4->mem[lc4->pc], 2, 1, fp);
}

// Function that checks and sets the NZP bits
void set_nzp (Data *lc4, unsigned short n) {
	lc4->psr = lc4->psr & 0b1111111111111000;
	if ((signed short) n > 0) {
		lc4->psr += 0b001;
	} else if ((signed short) n == 0) {
		lc4->psr += 0b010;
	} else if ((signed short) n < 0) {
		lc4->psr += 0b100;
	}
}

// Function that checks the legality of an action before it executes
// Function exits the program if action is illegal
void check_action (Data *lc4) {
	unsigned short privCheck = lc4->psr >> 15;
	unsigned short byte1 = lc4->mem[lc4->pc] >> 12;
	
	if ((lc4->pc >= 0x2000 && lc4->pc < 0x8000) || (lc4->pc >= 0xA000 && lc4->pc <= 0xFFFF)) {
		fprintf (stderr, "PC in data section of memory\n");
		FILE *img = fopen ("image.ppm", "w");
		if (img == 0) {
			fprintf (stderr, "Unable to open image.ppm\n");
			end_data (lc4);
			exit (1);
		}
		print_image (img, lc4);
		fclose (img);

		end_data (lc4);
		exit (1);
	} else if (lc4->pc >= 0x8000 && lc4->pc < 0xA000 && privCheck != 1) {
		fprintf (stderr, "PC in OS Code and PSR[15] != 1\n");
		FILE *img = fopen ("image.ppm", "w");
		if (img == 0) {
			fprintf (stderr, "Unable to open image.ppm\n");
			end_data (lc4);
			exit (1);
		}
		print_image (img, lc4);
		fclose (img);

		end_data (lc4);
		exit (1);
	} else if (byte1 == 0b0110) {
		unsigned short s = lc4->mem[lc4->pc] >> 6;
		s = s & 0b0000000111;
		signed short sImm6 = lc4->mem[lc4->pc] & 0b0000000000111111;
		if (sImm6 >= 0b100000) {
			sImm6 = -1 * (0b1000000 - sImm6);
		}
		if ((lc4->reg[s] + sImm6 >= 0x0000 && lc4->reg[s] + sImm6 < 0x2000) || 
			(lc4->reg[s] + sImm6 >= 0x8000 && lc4->reg[s] + sImm6 < 0xA000)) {
			fprintf (stderr, "LDR an element from code section not data section\n");
			FILE *img = fopen ("image.ppm", "w");
			if (img == 0) {
				fprintf (stderr, "Unable to open image.ppm\n");
				end_data (lc4);
				exit (1);
			}
			print_image (img, lc4);
			fclose (img);

			end_data (lc4);
			exit (1);
		} else if (lc4->reg[s] + sImm6 >= 0xA000 && lc4->reg[s] + sImm6 <= 0xFFFF && privCheck != 1) {
			fprintf (stderr, "LDR an element into OS data but PSR[15] != 1\n");
			FILE *img = fopen ("image.ppm", "w");
			if (img == 0) {
				fprintf (stderr, "Unable to open image.ppm\n");
				end_data (lc4);
				exit (1);
			}
			print_image (img, lc4);
			fclose (img);

			end_data (lc4);
			exit (1);
		}
	} else if (byte1 == 0b0111) {
		unsigned short s = lc4->mem[lc4->pc] >> 6;
		s = s & 0b0000000111;
		signed short sImm6 = lc4->mem[lc4->pc] & 0b0000000000111111;
		if (sImm6 >= 0b100000) {
			sImm6 = -1 * (0b1000000 - sImm6);
		}
		if ((lc4->reg[s] + sImm6 >= 0x0000 && lc4->reg[s] + sImm6 < 0x2000) || 
			(lc4->reg[s] + sImm6 >= 0x8000 && lc4->reg[s] + sImm6 < 0xA000)) {
			fprintf (stderr, "STR an element in code section not data section\n");
			FILE *img = fopen ("image.ppm", "w");
			if (img == 0) {
				fprintf (stderr, "Unable to open image.ppm\n");
				end_data (lc4);
				exit (1);
			}
			print_image (img, lc4);
			fclose (img);

			end_data (lc4);
			exit (1);
		} else if (lc4->reg[s] + sImm6 >= 0xA000 && lc4->reg[s] + sImm6 <= 0xFFFF && privCheck != 1) {
			fprintf (stderr, "STR an element from OS data but PSR[15] != 1\n");
			FILE *img = fopen ("image.ppm", "w");
			if (img == 0) {
				fprintf (stderr, "Unable to open image.ppm\n");
				end_data (lc4);
				exit (1);
			}
			print_image (img, lc4);
			fclose (img);

			end_data (lc4);
			exit (1);
		}
	}
}

// Function that analyses memory and executes the appropriate action
void analyse_code (FILE *out, Data *lc4) {
	check_action (lc4);
	print_line (out, lc4);
	unsigned short byte1 = lc4->mem[lc4->pc] >> 12;

	if (byte1 == 0b0000) {
		unsigned short check = lc4->mem[lc4->pc] >> 9;
		signed short sImm9 = lc4->mem[lc4->pc] & 0b0000000111111111;
		if (sImm9 >= 0b100000000) {
			sImm9 = -1 * (0b1000000000 - sImm9);
		}
		check = check & 0b0000111;
		unsigned short nzpCheck = lc4->psr & 0b0000000000000111;
		if (check == 0b000) {
			// fprintf (fp, " -> NOP\n");
		} else if (check == 0b001) {
			// fprintf (fp, " -> BRp %d\n", sImm9);
			if (nzpCheck == 0b001) {
				lc4->pc += sImm9;
			}
		} else if (check == 0b010) {
			// fprintf (fp, " -> BRz %d\n", sImm9);
			if (nzpCheck == 0b010) {
				lc4->pc += sImm9;
			}
		} else if (check == 0b011) {
			// fprintf (fp, " -> BRzp %d\n", sImm9);
			if (nzpCheck == 0b010 || nzpCheck == 0b001) {
				lc4->pc += sImm9;
			}
		} else if (check == 0b100) {
			// fprintf (fp, " -> BRn %d\n", sImm9);
			if (nzpCheck == 0b100) {
				lc4->pc += sImm9;
			}
		} else if (check == 0b101) {
			// fprintf (fp, " -> BRnp %d\n", sImm9);
			if (nzpCheck == 0b100 || nzpCheck == 0b001) {
				lc4->pc += sImm9;
			}
		} else if (check == 0b110) {
			// fprintf (fp, " -> BRnz %d\n", sImm9);
			if (nzpCheck == 0b100 || nzpCheck == 0b010) {
				lc4->pc += sImm9;
			}
		} else if (check == 0b111) {
			// fprintf (fp, " -> BRnzp %d\n", sImm9);
			if (nzpCheck == 0b100 || nzpCheck == 0b010 || nzpCheck == 0b001) {
				lc4->pc += sImm9;
			}
		} else {
			// fprintf (fp, " -> INVALID INSTRUCTION\n");
		}
		lc4->pc++;
	} else if (byte1 == 0b0001) {
		unsigned short check = lc4->mem[lc4->pc] >> 3, d = lc4->mem[lc4->pc] >> 9,
			s = lc4->mem[lc4->pc] >> 6, t = lc4->mem[lc4->pc];
		check = check & 0b0000000000111;
		d = d & 0b0000111;
		s = s & 0b0000000111;
		t = t & 0b000000000111;
		if (check == 0b000) {
			// fprintf (fp, " -> ADD R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = (signed short) lc4->reg[s] + (signed short) lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b001) {
			// fprintf (fp, " -> MUL R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = (signed short) lc4->reg[s] * (signed short) lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b010) {
			// fprintf (fp, " -> SUB R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = (signed short) lc4->reg[s] - (signed short) lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b011) {
			// fprintf (fp, " -> DIV R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = (signed short) lc4->reg[s] / (signed short) lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b100 || check == 0b101 || check == 0b110 ||
			check == 0b111) {
			signed short sImm5 = lc4->mem[lc4->pc] & 0b0000000000011111;
			if (sImm5 >= 0b10000) {
				sImm5 = -1 * (0b100000 - sImm5);
			}
			// fprintf (fp, " -> ADD R%d R%d %d\n", d, s, sImm5);
			lc4->reg[d] = (signed short) lc4->reg[s] + sImm5;

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else {
			// fprintf (fp, " -> INVALID INSTRUCTION\n");
			lc4->pc++;
		}
	} else if (byte1 == 0b1010) {
		unsigned short check = lc4->mem[lc4->pc] >> 4, d = lc4->mem[lc4->pc] >> 9, 
			s = lc4->mem[lc4->pc] >> 6, t = lc4->mem[lc4->pc], uImm4 = lc4->mem[lc4->pc];
		check = check & 0b000000000011;
		d = d & 0b0000111;
		s = s & 0b0000000111;
		t = t & 0b0000000000000111;
		uImm4 = uImm4 & 0b0000000000001111;
		if (check == 0b11) {
			// fprintf (fp, " -> MOD R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = (signed short) lc4->reg[s] % (signed short) lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b00) {
			// fprintf (fp, " -> SLL R%d R%d %d\n", d, s, uImm4);
			lc4->reg[d] = lc4->reg[s] << uImm4;

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b01) {
			// fprintf (fp, " -> SRA R%d R%d %d\n", d, s, uImm4);
			unsigned short msbCheck = lc4->reg[s] >> 15;
			lc4->reg[d] = lc4->reg[s] >> uImm4;
			if (msbCheck == 0b1) {
				unsigned short shift = ~0 << uImm4;
				lc4->reg[d] = lc4->reg[d] | shift;
			}

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b10) {
			// fprintf (fp, " -> SRL R%d R%d %d\n", d, s, uImm4);
			lc4->reg[d] = lc4->reg[s] >> uImm4;

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else {
			// fprintf (fp, " -> INVALID INSTRUCTION\n");
			lc4->pc++;
		}
	} else if (byte1 == 0b0101) {
		unsigned short check = lc4->mem[lc4->pc] >> 3, d = lc4->mem[lc4->pc] >> 9, 
			s = lc4->mem[lc4->pc] >> 6, t = lc4->mem[lc4->pc];
		check = check & 0b0000000000111;
		d = d & 0b0000111;
		s = s & 0b0000000111;
		t = t & 0b0000000000000111;
		if (check == 0b000) {
			// fprintf (fp, " -> AND R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = lc4->reg[s] & lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b001) {
			// fprintf (fp, " -> NOT R%d R%d\n", d, s);
			lc4->reg[d] = ~lc4->reg[s];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b010) {
			// fprintf (fp, " -> OR R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = lc4->reg[s] | lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b011) {
			// fprintf (fp, " -> XOR R%d R%d R%d\n", d, s, t);
			lc4->reg[d] = lc4->reg[s] ^ lc4->reg[t];

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else if (check == 0b100 || check == 0b101 || check == 0b110 ||
			check == 0b111){
			signed short sImm5 = lc4->mem[lc4->pc] & 0b0000000000011111;
			if (sImm5 >= 0b10000) {
					sImm5 = -1 * (0b100000 - sImm5);
			}
			// fprintf (fp, " -> AND R%d R%d %d\n", d, s, sImm5);
			lc4->reg[d] = lc4->reg[s] & sImm5;

			set_nzp (lc4, lc4->reg[d]);
			lc4->pc++;
		} else {
			// fprintf (fp, " -> INVALID INSTRUCTION\n");
			lc4->pc++;
		}
	} else if (byte1 == 0b0110) {
		unsigned short d = lc4->mem[lc4->pc] >> 9, s = lc4->mem[lc4->pc] >> 6;
		d = d & 0b0000111;
		s = s & 0b0000000111;
		signed short sImm6 = lc4->mem[lc4->pc] & 0b0000000000111111;
		if (sImm6 >= 0b100000) {
			sImm6 = -1 * (0b1000000 - sImm6);
		}
		// fprintf (fp, " -> LDR R%d R%d %d\n", d, s, sImm6);
		lc4->reg[d] = lc4->mem[lc4->reg[s] + sImm6];

		set_nzp (lc4, lc4->reg[d]);
		lc4->pc++;
	} else if (byte1 == 0b0111) {
		unsigned short t = lc4->mem[lc4->pc] >> 9, s = lc4->mem[lc4->pc] >> 6;
		t = t & 0b0000111;
		s = s & 0b0000000111;
		signed short sImm6 = lc4->mem[lc4->pc] & 0b0000000000111111;
		if (sImm6 >= 0b100000) {
			sImm6 = -1 * (0b1000000 - sImm6);
		}
		// fprintf (fp, " -> STR R%d R%d %d\n", t, s, sImm6);
		lc4->mem[lc4->reg[s] + sImm6] = lc4->reg[t];

		lc4->pc++;
	} else if (byte1 == 0b1001) {
		unsigned short d = lc4->mem[lc4->pc] >> 9;
		d = d & 0b0000111;
		signed short sImm9 = lc4->mem[lc4->pc] & 0b0000000111111111;
		if (sImm9 >= 0b100000000) {
			sImm9 = -1 * (0b1000000000 - sImm9);
		}
		// fprintf (fp, " -> CONST R%d %d\n", d, sImm9);
		lc4->reg[d] = sImm9;

		set_nzp (lc4, lc4->reg[d]);
		lc4->pc++;
	} else if (byte1 == 0b1101) {
		unsigned short d = lc4->mem[lc4->pc] >> 9, uImm8 = lc4->mem[lc4->pc] & 0b0000000011111111;
		d = d & 0b0000111;
		// fprintf (fp, " -> HICONST R%d %d\n", d, uImm8);
		lc4->reg[d] = (lc4->reg[d] & 0xFF) | (uImm8 << 8);

		set_nzp (lc4, lc4->reg[d]);
		lc4->pc++;
	} else if (byte1 == 0b0010) {
		unsigned short check = lc4->mem[lc4->pc] >> 7, s = lc4->mem[lc4->pc] >> 9, t = lc4->mem[lc4->pc];
		check = check & 0b000000011;
		s = s & 0b0000111;
		t = t & 0b0000000000000111;
		if (check == 0b00) {
			// fprintf (fp, " -> CMP R%d R%d\n", s, t);
			unsigned short nzpCheck = (signed short) lc4->reg[s] - (signed short) lc4->reg[t];
			set_nzp (lc4, nzpCheck);
			lc4->pc++;
		} else if (check == 0b01) {
			// fprintf (fp, " -> CMPU R%d R%d\n", s, t);
			lc4->psr = lc4->psr & 0b1111111111111000;
			if (lc4->reg[s] - lc4->reg[t] > 0) {
				lc4->psr += 0b001;
			} else if (lc4->reg[s] - lc4->reg[t] == 0) {
				lc4->psr += 0b010;
			} else if (lc4->reg[s] - lc4->reg[t] < 0) {
				lc4->psr += 0b100;
			}
			lc4->pc++;
		} else if (check == 0b10) {
			signed short imm7 = lc4->mem[lc4->pc] & 0b0000000001111111;
			if (imm7 >= 0b1000000) {
					imm7 = -1 * (0b10000000 - imm7);
			}
			// fprintf (fp, " -> CMPI R%d %d\n", s, imm7);
			unsigned short nzpCheck = (signed short) lc4->reg[s] - (signed short) imm7;
			set_nzp (lc4, nzpCheck);
			lc4->pc++;
		} else if (check == 0b11) {
			unsigned short uImm7 = lc4->mem[lc4->pc] & 0b0000000001111111;
			// fprintf (fp, " -> CMPIU R%d %d\n", s, uImm7);
			lc4->psr = lc4->psr & 0b1111111111111000;
			if (lc4->reg[s] - uImm7 > 0) {
				lc4->psr += 0b001;
			} else if (lc4->reg[s] - uImm7 == 0) {
				lc4->psr += 0b010;
			} else if (lc4->reg[s] - uImm7 < 0) {
				lc4->psr += 0b100;
			}
			lc4->pc++;
		} else {
			// fprintf (fp, " -> INVALID INSTRUCTION\n");
			lc4->pc++;
		}
	} else if (byte1 == 0b0100) {
		unsigned short check = lc4->mem[lc4->pc] >> 11, s = lc4->mem[lc4->pc] >> 6;
		check = check & 0b00001;
		s = s & 0b0000000111;
		if (check == 0b0) {
			// fprintf (fp, " -> JSRR R%d\n", s);
			lc4->reg[7] = lc4->pc + 1;
			lc4->pc = lc4->reg[s];

			set_nzp (lc4, lc4->reg[7]);
		} else if (check == 0b1) {
			signed short imm11 = lc4->mem[lc4->pc];
			imm11 = imm11 & 0b0000011111111111;
			if (imm11 >= 0b10000000000) {
				imm11 = -1 * (0b100000000000 - imm11);
			}
			// fprintf (fp, " -> JSR %d\n", imm11);
			lc4->reg[7] = lc4->pc + 1;
			lc4->pc = (lc4->pc & 0x8000) | (imm11 << 4);

			set_nzp (lc4, lc4->reg[7]);
		} else {
			// fprintf (fp, " -> INVALID INSTRUCTION\n");
			lc4->pc++;
		}
	} else if (byte1 == 0b1100) {
		unsigned short check = lc4->mem[lc4->pc] >> 11, s = lc4->mem[lc4->pc] >> 6;
		check = check & 0b00001;
		s = s & 0b0000000111;
		if (check == 0b0) {
			// fprintf (fp, " -> JMPR R%d\n", s);
			lc4->pc = lc4->reg[s];
		} else if (check == 0b1) {
			signed short sImm11 = lc4->mem[lc4->pc] & 0b0000011111111111;
			if (sImm11 >= 0b10000000000) {
				sImm11 = -1 * (0b100000000000 - sImm11);
			}
			// fprintf (fp, " -> JMP %d\n", sImm11);
			lc4->pc += 1 + sImm11;
		} else {
			// fprintf (fp, " -> INVALID INSTRUCTION\n");
			lc4->pc++;
		}
	} else if (byte1 == 0b1111) {
		unsigned short uImm8 = lc4->mem[lc4->pc] & 0b0000000011111111;
		// fprintf (fp, " -> TRAP %d\n", uImm8);
		lc4->reg[7] = lc4->pc + 1;
		lc4->pc = (0x8000 | uImm8);
		lc4->psr = lc4->psr & 0b0111111111111111;
		lc4->psr += 0b1000000000000000;

		set_nzp (lc4, lc4->reg[7]);
	} else if (byte1 == 0b1000) {
		// fprintf (fp, " -> RTI\n");
		lc4->pc = lc4->reg[7];
		lc4->psr = lc4->psr & 0b0111111111111111;
	} else {
		// fprintf (fp, " -> INVALID INSTRUCTION\n");
		lc4->pc++;
	}
}

// Function that prints image from video memory
void print_image (FILE *fp, Data *lc4) {
	fprintf (fp, "P6\n 128 124 32\n");
	for (int i = 0xC000; i < 0xFE00; i++) {
		unsigned short red = lc4->mem[i] >> 10, green = lc4->mem[i] >> 5, 
			blue = lc4->mem[i];
		red = red & 0b011111;
		green = green & 0b00000011111;
		blue = blue & 0b0000000000011111;
		fprintf (fp, "%c%c%c", red, green, blue);
	}
}
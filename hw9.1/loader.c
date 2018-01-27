// File which deals with parsing through .j files and identifying tokens when appropriate
// Jason Sun (38204849)
// 12/11/2017

#include "loader.h"

// Creates token
token *create_token (int type) {
	token *t = malloc (sizeof (token));
	if (t == NULL) {
		fprintf (stderr, "Out of memory\n");
		exit (1);
	}
	initialise_token (t);
	t->type = type;
	return t;
}

// Initialises token 
void initialise_token (token *t) {
	t->type = 0;
	t->literal_value = 0;
	t->arg_no = 0;
	for (int i = 0; i < MAX_TOKEN_LENGTH; i++) {
		t->str[i] = 0;
	}
}

// Frees token
void free_token (token *t) {
	free (t);
}

// Parses through .j file and appropriately identifies tokens
void load_file (FILE *jFile, FILE *asmFile, IfCount *ic) {
	char temp[MAX_TOKEN_LENGTH] = {0};
	while (fscanf (jFile, "%s", temp) == 1) {
		if (temp[0] == ';') {
			fgets (temp, MAX_COMMENT, jFile);
			continue;
		}

		if (strcmp (temp, "defun") == 0) {
			token *t = create_token (0);
			fscanf (jFile, "%s", temp);
			strcpy (t->str, temp);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "return") == 0) {
			token *t = create_token (2);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "+") == 0) {
			token *t = create_token (3);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "-") == 0) {
			token *t = create_token (4);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "*") == 0) {
			token *t = create_token (5);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "/") == 0) {
			token *t = create_token (6);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "%") == 0) {
			token *t = create_token (7);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "and") == 0) {
			token *t = create_token (8);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "or") == 0) {
			token *t = create_token (9);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "not") == 0) {
			token *t = create_token (10);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "lt") == 0) {
			token *t = create_token (11);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "le") == 0) {
			token *t = create_token (12);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "eq") == 0) {
			token *t = create_token (13);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "ge") == 0) {
			token *t = create_token (14);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "gt") == 0) {
			token *t = create_token (15);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "if") == 0) {
			token *t = create_token (16);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "else") == 0) {
			token *t = create_token (17);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "endif") == 0) {
			token *t = create_token (18);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "drop") == 0) {
			token *t = create_token (19);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "dup") == 0) {
			token *t = create_token (20);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "swap") == 0) {
			token *t = create_token (21);
			read_token (t, asmFile, ic);
		} else if (strcmp (temp, "rot") == 0) {
			token *t = create_token (22);
			read_token (t, asmFile, ic);
		} else if (temp[0] == 'a' && temp[1] == 'r' && temp[2] == 'g') {
			token *t = create_token (23);
			strcpy (temp, &temp[3]);
			t->arg_no = atoi (temp);
			read_token (t, asmFile, ic);
		} else if (temp[0] == '0' && temp[1] == 'x') {
			token *t = create_token (24);
			t->literal_value = strtol (&temp[2], NULL, 16);
			read_token (t, asmFile, ic);
		} else if ((temp[0] >= '0' && temp[0] <= '9') || 
			(temp[0] == '-' && temp[1] >= '0' && temp[1] <= '9')) {
			token *t = create_token (24);
			t->literal_value = atoi (temp);
			read_token (t, asmFile, ic);
		}  else {
			int validFuncName = 0;
			for (int i = 0; temp[i] != '\0'; i++) {
				if ((i == 0 && temp[0] >= 'a' && temp[0] <= 'z') ||
					(i == 0 && temp[0] >= 'A' && temp[0] <= 'Z')) {
					validFuncName = 1;
				} else if ((i != 0 && temp[i] >= 'a' && temp[i] <= 'z') || 
					(i != 0 && temp[i] >= 'A' && temp[i] <= 'Z') ||
					(i != 0 && temp[i] == '_') ||
					(i != 0 && temp[i] >= '0' && temp[i] <= '9')) {
					validFuncName = 1;
				} else {
					validFuncName = 0;
					break;
				}
			}

			if (validFuncName == 1) {
				token *t = create_token (1);
				strcpy (t->str, temp);
				read_token (t, asmFile, ic);
			} else {	
				token *t = create_token (25);
				read_token (t, asmFile, ic);
			}
		}
	}
}

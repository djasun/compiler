// File that deals with taking in tokens and outputting the appropriate LC4 code into the appropriate .asm file
// Jason Sun (38204849)
// 12/11/2017

#include "token.h"

int nComps = 0;
int nIfs = 0;

// Starts if stack
IfCount *create_if () {
	IfCount *ic = malloc (sizeof (IfCount));
	if (ic == NULL) {
		fprintf (stderr, "Out of memory\n");
		exit (1);
	}
	ic->head = NULL;
	return ic;
}

// Adds node to top of if stack
void push_if (IfCount *ic, int ifN) {
	if (ic->head == NULL) {
		Node *n = malloc (sizeof (Node));
		if (n == NULL) {
			fprintf (stderr, "Out of memory\n");
			exit (1);
		}
		n->ifN = ifN;
		n->next = NULL;
		ic->head = n;
	} else {
		Node *n = malloc (sizeof (Node));
		if (n == NULL) {
			fprintf (stderr, "Out of memory\n");
			exit (1);
		}
		n->ifN = ifN;
		n->next = ic->head;
		ic->head = n;
	}
}

// Removes node from top of if stack
void pop_if (IfCount *ic) {
	if (ic->head == NULL) {
		fprintf (stderr, "Popping an empty stack\n");
		exit (1);
	} else if (ic->head->next == NULL) {
		free (ic->head);
		ic->head = NULL;
	} else {
		Node *newHead = ic->head->next;
		free (ic->head);
		ic->head = newHead;
	}
}

// Removes all nodes from if stack
void free_if (IfCount *ic) {
	if (ic->head == NULL) {
		fprintf (stderr, "Freeing an empty stack\n");
		exit (1);
	} else if (ic->head->next == NULL) {
		free (ic->head);
		ic->head = NULL;
	} else {
		while (ic->head->next != NULL) {
			Node *newNext = ic->head->next;
			free (ic->head);
			ic->head = newNext;
		}
	}
}

// Takes in token and outputs lc4 code into .asm file
void read_token (token *t, FILE *asmFile, IfCount *ic) {
	if (t->type == 0) { // DEFUN
		fprintf (asmFile, "\t;; DEFUN\n");
		fprintf (asmFile, "\t\t.CODE\n");
		fprintf (asmFile, "\t\t.FALIGN\n");
		fprintf (asmFile, "%s\n", t->str);
		fprintf (asmFile, "\tADD R6, R6, #-3\n");
		fprintf (asmFile, "\tSTR R7, R6, #1\n");
		fprintf (asmFile, "\tSTR R5, R6, #0\n");
		fprintf (asmFile, "\tADD R5, R6, #0\n\n");
	} else if (t->type == 1) { // IDENT
		fprintf (asmFile, "\t;; IDENT\n");
		fprintf (asmFile, "\tJSR %s\n\n", t->str);
	} else if (t->type == 2) { // RETURN
		fprintf (asmFile, "\t;; RETURN\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n");
		fprintf (asmFile, "\tSTR R0, R5, #2\n");
		fprintf (asmFile, "\tADD R6, R5, #0\n");
		fprintf (asmFile, "\tLDR R5, R6, #0\n");
		fprintf (asmFile, "\tLDR R7, R6, #1\n");
		fprintf (asmFile, "\tADD R6, R6, #2\n"); // #2 instead of #3 to have RV at top of stack
		fprintf (asmFile, "\tRET\n\n");		
	} else if (t->type == 3) { // PLUS
		fprintf (asmFile, "\t;; PLUS\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); // TOP
		fprintf (asmFile, "\tLDR R1, R6, #1\n"); // 2ND
		fprintf (asmFile, "\tADD R2, R0, R1\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 4) { // MINUS
		fprintf (asmFile, "\t;; MINUS\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tSUB R2, R0, R1\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 5) { // MUL
		fprintf (asmFile, "\t;; MUL\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tMUL R2, R0, R1\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 6) { // DIV
		fprintf (asmFile, "\t;; DIV\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tDIV R2, R0, R1\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 7) { // MOD
		fprintf (asmFile, "\t;; MOD\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tMOD R2, R0, R1\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 8) { // AND
		fprintf (asmFile, "\t;; AND\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tAND R2, R0, R1\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 9) { // OR
		fprintf (asmFile, "\t;; OR\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tOR R2, R0, R1\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 10) { // NOT
		fprintf (asmFile, "\t;; NOT\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n");
		fprintf (asmFile, "\tNOT R2, R0\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
	} else if (t->type == 11) { // LT
		fprintf (asmFile, "\t;; LT\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tCMP R0, R1\n");
		fprintf (asmFile, "\tBRn NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tBRz ZERO%d\n", nComps);
		fprintf (asmFile, "\tBRp POSITIVE%d\n", nComps);

		fprintf (asmFile, "NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #1\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "ZERO%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "POSITIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "COMP_END%d\n", nComps);
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
		nComps++;
	} else if (t->type == 12) { // LE
		fprintf (asmFile, "\t;; LE\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tCMP R0, R1\n");
		fprintf (asmFile, "\tBRn NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tBRz ZERO%d\n", nComps);
		fprintf (asmFile, "\tBRp POSITIVE%d\n", nComps);

		fprintf (asmFile, "NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #1\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "ZERO%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #1\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "POSITIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "COMP_END%d\n", nComps);
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
		nComps++;
	} else if (t->type == 13) { // EQ
		fprintf (asmFile, "\t;; EQ\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tCMP R0, R1\n");
		fprintf (asmFile, "\tBRn NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tBRz ZERO%d\n", nComps);
		fprintf (asmFile, "\tBRp POSITIVE%d\n", nComps);

		fprintf (asmFile, "NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "ZERO%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #1\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "POSITIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "COMP_END%d\n", nComps);
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
		nComps++;
	} else if (t->type == 14) { // GE
		fprintf (asmFile, "\t;; GE\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tCMP R0, R1\n");
		fprintf (asmFile, "\tBRn NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tBRz ZERO%d\n", nComps);
		fprintf (asmFile, "\tBRp POSITIVE%d\n", nComps);

		fprintf (asmFile, "NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "ZERO%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #1\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "POSITIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #1\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "COMP_END%d\n", nComps);
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
		nComps++;
	} else if (t->type == 15) { // GT
		fprintf (asmFile, "\t;; GT\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n"); 
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tCMP R0, R1\n");
		fprintf (asmFile, "\tBRn NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tBRz ZERO%d\n", nComps);
		fprintf (asmFile, "\tBRp POSITIVE%d\n", nComps);

		fprintf (asmFile, "NEGATIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "ZERO%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #0\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "POSITIVE%d\n", nComps);
		fprintf (asmFile, "\tCONST R2, #1\n");
		fprintf (asmFile, "\t JMP COMP_END%d\n", nComps);

		fprintf (asmFile, "COMP_END%d\n", nComps);
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");
		nComps++;
	} else if (t->type == 16) { // IF
		push_if (ic, nIfs);
		nIfs++;
		fprintf (asmFile, "\t;; IF\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n");
		fprintf (asmFile, "\tLDR R0, R6, #-1\n");
		fprintf (asmFile, "\tBRp IF%d\n", ic->head->ifN);
		fprintf (asmFile, "\tBRnz ELSE%d\n", ic->head->ifN);
		fprintf (asmFile, "IF%d\n\n", ic->head->ifN);
	} else if (t->type == 17) { // ELSE
		fprintf (asmFile, "\t;; ELSE\n");
		fprintf (asmFile, "\tJMP ENDIF%d\n", ic->head->ifN);
		fprintf (asmFile, "ELSE%d\n\n", ic->head->ifN);
	} else if (t->type == 18) { // ENDIF
		fprintf (asmFile, "\t;; ENDIF\n");
		fprintf (asmFile, "ENDIF%d\n\n", ic->head->ifN);
		pop_if (ic);
	} else if (t->type == 19) { // DROP
		fprintf (asmFile, "\t;; DROP\n");
		fprintf (asmFile, "\tADD R6, R6, #1\n\n");
	} else if (t->type == 20) { // DUP
		fprintf (asmFile, "\t;; DUP\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n");
		fprintf (asmFile, "\tADD R6, R6, #-1\n");
		fprintf (asmFile, "\tSTR R0, R6, #0\n\n");
	} else if (t->type == 21) { // SWAP
		fprintf (asmFile, "\t;; SWAP\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n");
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tSTR R1, R6, #0\n");
		fprintf (asmFile, "\tSTR R0, R6, #1\n\n");		
	} else if (t->type == 22) { // ROT
		fprintf (asmFile, "\t;; ROT\n");
		fprintf (asmFile, "\tLDR R0, R6, #0\n");
		fprintf (asmFile, "\tLDR R1, R6, #1\n");
		fprintf (asmFile, "\tLDR R2, R6, #2\n");
		fprintf (asmFile, "\tSTR R0, R6, #1\n");
		fprintf (asmFile, "\tSTR R1, R6, #2\n");
		fprintf (asmFile, "\tSTR R2, R6, #0\n\n");		
	} else if (t->type == 23) { // ARG
		fprintf (asmFile, "\t;; ARG\n");
		fprintf (asmFile, "\tLDR R0, R5, #%d\n", t->arg_no + 2);
		fprintf (asmFile, "\tSTR R0, R6, #-1\n");
		fprintf (asmFile, "\tADD R6, R6, #-1\n\n");
	} else if (t->type == 24) { // LITERAL
		fprintf (asmFile, "\t;; LITERAL\n");
		fprintf (asmFile, "\tADD R6, R6, #-1\n");
		fprintf (asmFile, "\tCONST R0, #%d\n", t->literal_value & 0b0000000011111111);
		fprintf (asmFile, "\tHICONST R0 #%d\n", (t->literal_value & 0b1111111100000000) >> 8);
		fprintf (asmFile, "\tSTR R0, R6, #0\n\n");
	} else if (t->type == 25) { // BROKEN_TOKEN
		fprintf (asmFile, "\t;; BROKEN_TOKEN\n\n");
		fprintf (stderr, "Broken token, exiting\n");
		exit (1);
	}
	free (t);
}
// Header file for token.c
// Jason Sun (38204849)
// 12/11/2017

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 250
#define MAX_IFS 1024

typedef struct {
  enum { DEFUN, IDENT, RETURN, 
         PLUS, MINUS, MUL, DIV, MOD, 
         AND, OR, NOT, 
         LT, LE, EQ, GE, GT,
         IF, ELSE, ENDIF, 
         DROP, DUP, SWAP, ROT,
	       ARG,
         LITERAL,
         BROKEN_TOKEN } type;
  int literal_value;    // this field used to store the value of literal tokens
  int arg_no;           // this field used to store the index of argument literals argN
  char str[MAX_TOKEN_LENGTH];
} token;

// Node struct for if stack
typedef struct node Node;
struct node {
  int ifN;
  Node *next;
};

// If stack
typedef struct ifCount IfCount;
struct ifCount {
  Node *head;
};

IfCount *create_if ();
void push_if (IfCount *ic, int ifN);
void pop_if (IfCount *ic);
void free_if (IfCount *ic);
void read_token (token *t, FILE *asmFile, IfCount *head);
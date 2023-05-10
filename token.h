/*
 * token.h
 */

#ifndef TOKEN_H
#define TOKEN_H

// the maximum length of a token is 250
#define MAX_TOKEN_LENGTH 250

#include <stdio.h>
#include <stdbool.h>

typedef enum { DEFUN, IDENT, RETURN, 
	       PLUS, MINUS, MUL, DIV, MOD, 
	       AND, OR, NOT, 
	       LT, LE, EQ, GE, GT,
	       IF, ELSE, ENDIF, 
               WHILE, ENDWHILE,
	       DROP, DUP, SWAP, ROT,
	       ARG,
	       LITERAL,
	       BAD_TOKEN } token_type;

typedef struct token_st {
  token_type type;
  int literal_value;    // this field used to store the value of literal tokens
  int arg_no;           // this field used to store the index of argument literals argN
  char str[MAX_TOKEN_LENGTH + 1];  // This character array stores the token string
} token;

// line_number : You can use this to keep track of the current line number you
// are at in the J source file which can be useful in reporting syntax errors.
// Be sure to include a line that says 'int line_number;' in token.c outside
// of any functions if you would like to make use of this.
extern int line_number;

// gets the next J token from the specified input file.
// 
// Arguments:
// - j_file: the file stream to read from. Should be a valid file and have read permissions.
// - output: the output parameter to "return" the read token. The type must always be set.
//           If the token is of type IDENT, the name of the identifier should
//           stored in the field `str` as a null-terminated string.
//           If the token is of type LITERAL, then the field literal_value
//           should contain the equivalent integer value of the literal token.
//           If the token is of type ARG, then the arg number 'n' should be
//           stored in the field 'arg_no'. For the remaining cases, the
//           field can be set to whatever is desired.
// 
// Return Value:
// - true if a token was read successfully
// - false if a token could not be read due to either hitting the end of file
//   or some unrecoverable error
bool next_token(FILE *j_file, token *output);

// Extra function which you may wish to define and use, but are not required to use

// Prints a token to the specified file
//
// Arguments:
// - f: a file stream that should be valid and have write permissions.
//      The token is printed to this file in whatever format you like.
//      To print to the terminal, pass in 'std_out'.
// - to_print: the token to be printed to the file
void print_token (FILE* f, token to_print);

#endif
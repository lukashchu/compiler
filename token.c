/*
 * token.c
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

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
bool next_token(FILE *j_file, token *output) {
    char currentline[MAX_TOKEN_LENGTH + 1];
    int linescan = fscanf(j_file, "%s", currentline);
    if (linescan < 0) {
        return false;
    }
    
    if (strcmp(currentline, "defun") == 0) {
        output->type = DEFUN;
    } else if (strcmp(currentline, "return") == 0) {
        output->type = RETURN;
    } else if (strcmp(currentline, "+") == 0) {
        output->type = PLUS;
    } else if (strcmp(currentline, "-") == 0) {
        output->type = MINUS;
    } else if (strcmp(currentline, "*") == 0) {
        output->type = MUL;
    } else if (strcmp(currentline, "/") == 0) {
        output->type = DIV;
    } else if (strcmp(currentline, "%") == 0) {
        output->type = MOD;
    } else if (strcmp(currentline, "and") == 0) {
        output->type = AND;
    } else if (strcmp(currentline, "or") == 0) {
        output->type = OR;
    } else if (strcmp(currentline, "not") == 0) {
        output->type = NOT;
    } else if (strcmp(currentline, "lt") == 0) {
        output->type = LT;
    } else if (strcmp(currentline, "le") == 0) {
        output->type = LE;
    } else if (strcmp(currentline, "eq") == 0) {
        output->type = EQ;
    } else if (strcmp(currentline, "ge") == 0) {
        output->type = GE;
    } else if (strcmp(currentline, "gt") == 0) {
        output->type = GT;
    } else if (strcmp(currentline, "if") == 0) {
        output->type = IF;
    } else if (strcmp(currentline, "else") == 0) {
        output->type = ELSE;
    } else if (strcmp(currentline, "endif") == 0) {
        output->type = ENDIF;
    } else if (strcmp(currentline, "while") == 0) {
        output->type = WHILE;
    } else if (strcmp(currentline, "endwhile") == 0) {
        output->type = ENDWHILE;
    } else if (strcmp(currentline, "drop") == 0) {
        output->type = DROP;
    } else if (strcmp(currentline, "dup") == 0) {
        output->type = DUP;
    } else if (strcmp(currentline, "swap") == 0) {
        output->type = SWAP;
    } else if (strcmp(currentline, "rot") == 0) {
        output->type = ROT;
    } else if (strcmp(currentline, "bad_token") == 0) {
        output->type = BAD_TOKEN;
    } else if (currentline[0] == 'a' && currentline[1] == 'r' && currentline[2] == 'g') {
        char *argnum = &currentline[3];
        sscanf(argnum, "%d", &(output->arg_no));
        output->type = ARG;
    } else if (currentline[0] == '0' && currentline[1] == 'x') {
        output->type = LITERAL;
        sscanf(currentline, "%x", &(output->literal_value));
    } else if (sscanf(currentline, "%d", &(output->literal_value)) == 1) {
        output->type = LITERAL;
    } else if (currentline[0] == ';') {
        char c;
        while ((c = getc(j_file)) >= 0) {
            if (c == '\n') {
                next_token(j_file, output);
                break;
            }
        }
    } else {
        output->type = IDENT;
        sscanf(currentline, "%s", output->str);
    }
    
    return true;
}

// Prints a token to the specified file
//
// Arguments:
// - f: a file stream that should be valid and have write permissions.
//      The token is printed to this file in whatever format you like.
//      To print to the terminal, pass in 'std_out'.
// - to_print: the token to be printed to the file
void print_token (FILE* f, token to_print) {
    if (to_print.type == DEFUN) {
        fprintf(f, "defun");
    } else if (to_print.type == IDENT) {
        fprintf(f, "%s", to_print.str);
    } else if (to_print.type == RETURN) {
        fprintf(f, "return");
    } else if (to_print.type == PLUS) {
        fprintf(f, "+");
    } else if (to_print.type == MINUS) {
        fprintf(f, "-");
    } else if (to_print.type == MUL) {
        fprintf(f, "*");
    } else if (to_print.type == DIV) {
        fprintf(f, "/");
    } else if (to_print.type == MOD) {
        fprintf(f, "%%");
    } else if (to_print.type == AND) {
        fprintf(f, "and");
    } else if (to_print.type == OR) {
        fprintf(f, "or");
    } else if (to_print.type == NOT) {
        fprintf(f, "not");
    } else if (to_print.type == LT) {
        fprintf(f, "lt");
    } else if (to_print.type == LE) {
        fprintf(f, "le");
    } else if (to_print.type == EQ) {
        fprintf(f, "eq");
    } else if (to_print.type == GE) {
        fprintf(f, "ge");
    } else if (to_print.type == GT) {
        fprintf(f, "gt");
    } else if (to_print.type == IF) {
        fprintf(f, "if");
    } else if (to_print.type == ELSE) {
        fprintf(f, "else");
    } else if (to_print.type == ENDIF) {
        fprintf(f, "endif");
    } else if (to_print.type == WHILE) {
        fprintf(f, "while");
    } else if (to_print.type == ENDWHILE) {
        fprintf(f, "endwhile");
    } else if (to_print.type == DROP) {
        fprintf(f, "drop");
    } else if (to_print.type == DUP) {
        fprintf(f, "dup");
    } else if (to_print.type == SWAP) {
        fprintf(f, "swap");
    } else if (to_print.type == ROT) {
        fprintf(f, "rot");
    } else if (to_print.type == ARG) {
        fprintf(f, "arg%d", to_print.arg_no);
    } else if (to_print.type == LITERAL) {
        fprintf(f, "%d", to_print.literal_value);
    } else if (to_print.type == BAD_TOKEN) {
        fprintf(f, "bad_token");
    } 
}
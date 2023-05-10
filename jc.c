/*
 * jc.c
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <libgen.h>

#include "jc.h"
#include "token.h"

// main() function which parses command line arguments and 
// functions to read the J file and write the asm file
int main(int argc, char** argv) {
    char filename[strlen(argv[1]) + 3];
    strcpy(filename, argv[1]);
    filename[strlen(filename) - 1] = 'a';
    strcat(filename, "sm\0");

    FILE *j_file = fopen(argv[1], "r");
    FILE *asm_file = fopen(filename, "w");

    char fileheader[strlen(argv[1]) - 1];
    memcpy(fileheader, &filename, (strlen(argv[1]) - 2));
    fileheader[strlen(argv[1]) - 2] = '\0';

    Nested *if_ne = malloc(sizeof(Nested));
    if_ne->back = NULL;
    Nested *while_ne = malloc(sizeof(Nested));
    while_ne->back = NULL;

    int return_no = 1;
    int cmp_no = 1;
    int if_no = 1;
    int while_no = 1;

    token *output = malloc(sizeof(token));
    while (next_token(j_file, output)) {
        if (output->type == DEFUN) {
            next_token(j_file, output);
            if (output->type == IDENT) {
                fprintf(asm_file, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;%s;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n", output->str);
                fprintf(asm_file, "\t\t.CODE\n");
                fprintf(asm_file, "\t\t.FALIGN\n");
                fprintf(asm_file, "%s\n", output->str);
                fprintf(asm_file, "\t;; prologue\n");
                fprintf(asm_file, "\tSTR R7, R6, #-2\t;; save return address\n");
                fprintf(asm_file, "\tSTR R5, R6, #-3\t;; save base pointer\n");
                fprintf(asm_file, "\tADD R6, R6, #-3\n");
                fprintf(asm_file, "\tADD R5, R6, #0\n");
                fprintf(asm_file, "\t;; function body\n");
            }
        } else if (output->type == IDENT) {
            fprintf(asm_file, "\tJSR %s\n", output->str);
            fprintf(asm_file, "\tADD R6, R6, #-1\n");
        } else if (output->type == RETURN) {
            fprintf(asm_file, "\tLDR R7, R6, #0\t;; grab return value\n");
            fprintf(asm_file, "L%d_%s\n", return_no, basename(fileheader));
            return_no++;
            fprintf(asm_file, "\t;; epilogue\n");
            fprintf(asm_file, "\tADD R6, R5, #0\t;; pop locals off stack\n");
            fprintf(asm_file, "\tADD R6, R6, #3\t;; free space for return address, base pointer, and return value\n");
            fprintf(asm_file, "\tSTR R7, R6, #-1\t;; store return value\n");
            fprintf(asm_file, "\tLDR R5, R6, #-3\t;; restore base pointer\n");
            fprintf(asm_file, "\tLDR R7, R6, #-2\t;; restore return address\n");
            fprintf(asm_file, "\tRET\n\n");
        } else if (output->type == PLUS) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tADD R0, R0, R1\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == MINUS) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tSUB R0, R0, R1\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == MUL) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tMUL R0, R0, R1\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == DIV) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tDIV R0, R0, R1\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == MOD) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tMOD R0, R0, R1\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == AND) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tAND R0, R0, R1\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == OR) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tOR R0, R0, R1\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == NOT) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tNOT R0, R0\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == LT) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tCMP R0, R1\n");
            fprintf(asm_file, "\tBRzp FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #1\n");
            fprintf(asm_file, "\tJMP END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #0\n");
            fprintf(asm_file, "END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
            cmp_no++;
        } else if (output->type == LE) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tCMP R0, R1\n");
            fprintf(asm_file, "\tBRp FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #1\n");
            fprintf(asm_file, "\tJMP END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #0\n");
            fprintf(asm_file, "END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
            cmp_no++;
        } else if (output->type == EQ) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tCMP R0, R1\n");
            fprintf(asm_file, "\tBRnp FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #1\n");
            fprintf(asm_file, "\tJMP END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #0\n");
            fprintf(asm_file, "END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
            cmp_no++;
        } else if (output->type == GE) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tCMP R0, R1\n");
            fprintf(asm_file, "\tBRn FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #1\n");
            fprintf(asm_file, "\tJMP END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #0\n");
            fprintf(asm_file, "END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
            cmp_no++;
        } else if (output->type == GT) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tCMP R0, R1\n");
            fprintf(asm_file, "\tBRnz FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #1\n");
            fprintf(asm_file, "\tJMP END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "FALSE_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tCONST, R0, #0\n");
            fprintf(asm_file, "END_CMP_%d\n", cmp_no);
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
            cmp_no++;
        } else if (output->type == IF) {
            if (if_ne->back == NULL) {
                NestedNode *if_node = malloc(sizeof(NestedNode));
                if_node->val = if_no;
                if_node->has_else = false;
                if_ne->back = if_node;
            } else {
                NestedNode *if_node = malloc(sizeof(NestedNode));
                if_node->val = if_no;
                if_node->has_else = false;
                if_node->prev = if_ne->back;
                if_ne->back = if_node;
            }
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tCMPI R0, #0\n");
            fprintf(asm_file, "\tBRnz ELSE_%d\n", if_no);
            if_no++;
        } else if (output->type == ELSE) {
            fprintf(asm_file, "\tJMP ENDIF_%d\n", ((if_ne->back)->val));
            fprintf(asm_file, "ELSE_%d\n", ((if_ne->back)->val));
            ((if_ne->back)->has_else) = true;
        } else if (output->type == ENDIF) {
            if (!((if_ne->back)->has_else)) {
                fprintf(asm_file, "ELSE_%d\n", ((if_ne->back)->val));
            }
            fprintf(asm_file, "ENDIF_%d\n", ((if_ne->back)->val));
            NestedNode *if_node = (if_ne->back)->prev;
            free(if_ne->back);
            if_ne->back = if_node;
        } else if (output->type == WHILE) {
            if (while_ne->back == NULL) {
                NestedNode *while_node = malloc(sizeof(NestedNode));
                while_node->val = while_no;
                while_ne->back = while_node;
            } else {
                NestedNode *while_node = malloc(sizeof(NestedNode));
                while_node->val = while_no;
                while_node->prev = while_ne->back;
                while_ne->back = while_node;
            }
            fprintf(asm_file, "WHILE_%d\n", while_no);
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tADD R6, R6, #1\n");
            fprintf(asm_file, "\tCMPI R0, #0\n");
            fprintf(asm_file, "\tBRnz ENDWHILE_%d\n", while_no);
            while_no++;
        } else if (output->type == ENDWHILE) {
            fprintf(asm_file, "\tJMP WHILE_%d\n", ((while_ne->back)->val));
            fprintf(asm_file, "ENDWHILE_%d\n", ((while_ne->back)->val));
            NestedNode *while_node = (while_ne->back)->prev;
            free(while_ne->back);
            while_ne->back = while_node;
        } else if (output->type == DROP) {
            fprintf(asm_file, "\tADD R6, R6, #1\n");
        } else if (output->type == DUP) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tADD R6, R6, #-1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == SWAP) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tSTR R0, R6, #1\n");
            fprintf(asm_file, "\tSTR R1, R6, #0\n");
        } else if (output->type == ROT) {
            fprintf(asm_file, "\tLDR R0, R6, #0\n");
            fprintf(asm_file, "\tLDR R1, R6, #1\n");
            fprintf(asm_file, "\tLDR R2, R6, #2\n");
            fprintf(asm_file, "\tSTR R2, R6, #0\n");
            fprintf(asm_file, "\tSTR R0, R6, #1\n");
            fprintf(asm_file, "\tSTR R1, R6, #2\n");
        } else if (output->type == ARG) {
            fprintf(asm_file, "\tLDR R0, R5, #%d\n", (output->arg_no + 2));
            fprintf(asm_file, "\tADD R6, R6, #-1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == LITERAL) {
            fprintf(asm_file, "\tCONST R0, #%d\n", (output->literal_value & 0xFF));
            if (((output->literal_value & 0xFF00) >> 8) != 0) {
                fprintf(asm_file, "\tHICONST R0, #%d\n", ((output->literal_value & 0xFF00) >> 8));
            }
            fprintf(asm_file, "\tADD R6, R6, #-1\n");
            fprintf(asm_file, "\tSTR R0, R6, #0\n");
        } else if (output->type == BAD_TOKEN) {
            free(output);
            fclose(j_file);
            fclose(asm_file);
            return EXIT_FAILURE;
        } 
    }

    free(if_ne);
    free(while_ne);
    free(output);
    fclose(j_file);
    fclose(asm_file);
    return EXIT_SUCCESS;
}
/*
 * jc.h
 */

#define JC_H

#include <stdio.h>
#include <stdbool.h>

// Node of the nested while/if statements
typedef struct nested_node_st {
  int val;              // this field used to store the number value of this nested structure
  struct nested_node_st *prev;          // this field used to store the index of argument literals argN
  bool has_else;        // this field is used for if statements to check if they had an else branch
} NestedNode;

// Queue of sorts for the nested while/if statements
typedef struct nested_st {
  NestedNode *back;        // The back of the nested queue
} Nested;

// main() function which parses command line arguments and 
// functions to read the J file and write the asm file
int main(int argc, char** argv);
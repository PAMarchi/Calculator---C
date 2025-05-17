/* This program is a math interpreter, it uses the Shunting-yard + RPN evaluator as algorithm to evaluate infix expressions.
   It was made by Pedro Arthur Marchi [github.com/PAMarchi]. */

#ifndef MATH_INTERPRETER_H
#define MATH_INTERPRETER_H

#include "datastructures.h"
#include "lexer.h"
#include "parser.h"

/* Function that evaluates a math expression, it does the lexing and parsing (Shunting-Yard+RPN evaluation) parts
   As this function receives an array of chars (with NULL terminator at the end), 
   everything should be separated (for example 2.2 should be '2','.','2'; functions like sqrt should have the chars separated aswell)
   It returns the result as a double
   It receives the expression as a array of chars */
double Math_interpreter_evaluate_expression(char *expression,  bool *flag_err);

#endif
/* This program is part of the math interpreter, it is a parser that implements an expression syntax verifier, Shunting-yard algorithm
   and a RPN evaluator. All of those functions depends on the infix expression being already tokenized by the lexer and in a array of string format.
   It was made by Pedro Arthur Marchi [github.com/PAMarchi]. */

#ifndef PARSER_H
#define PARSER_H

#include "datastructures.h"
#include <stdbool.h>
#include <math.h>

typedef enum{
  
  LEFT,
  RIGHT
} Associativity;

/* Function to return the precendence of a operator 
   It returns the precedence in form of a int
   It receives the operator to evaluate */
int Parser_precedence_of(const char *op);

/* Function to return the associativity of a operator
   It returns if the operator has RIGHT or LEFT associativity 
   It receives a operator */
Associativity Parser_assoc_of(const char *op);

/* Function to tell if a token is a operator or not
   It returns true if the token is a operator
   It receives the token */
bool Parser_is_operator(const char *tok);

/* Similar to Parser_is_operator, but considers unary operator as such 
   It returns true if the token is a operator
   It receives the token */
bool Parser_is_any_operator(const char *tok);

/* Function to tell if a token is a number or not
   It returns true if the token is a number
   It receives the token */
bool Parser_is_number(const char *tok);

/* Function to tell if a token is a function or not
   It returns true if the token is a function
   It receives the token */
bool Parser_is_function(const char *tok);

/* Function to return the arity of an operator 
   It returns 1 if is unary or 2 for binary */
int Parser_arity_of(const char *op);

/* Function to allocate memory for a string 
   It returns a pointer to the string 
   It receives a string */
char *Parser_add_token(const char *src);

/* Function to analyse the syntax of the array representing the expression
   It returns true if the syntax is correct
   It receives the whole expression array */
bool Parser_is_syntax_correct(char **expression);

/* Function to convert infix (The infix is the NULL terminated array of tokens) tokens in RPN 
   It returns a new NULL terminated array of tokens in Reverse Polish Notation (RPN)
   It receives an NULL terminated array of tokens */
char **Parser_Shunting_yard(char **infix);

/* Function to evaluete a NULL terminated array of tokens in RPN 
   It returns the result of the expression in double format
   It receives a NULL terminated array in RPN */
double Parser_evaluate_rpn(char **rpn);

#endif
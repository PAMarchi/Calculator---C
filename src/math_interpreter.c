/* This program is a math interpreter, it uses the Shunting-yard + RPN evaluator as algorithm to evaluate infix expressions.
   It was made by Pedro Arthur Marchi [github.com/PAMarchi]. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/math_interpreter.h"

/* Function to free tokens memory 
   It receives tokens as a array of string */
void free_tokens(char **tokens){

  for(char **tok=tokens; *tok; tok++){
    free(*tok);
  }
  free(tokens);
}

/* Function that evaluates a math expression, it does the lexing and parsing (Shunting-Yard+RPN evaluation) parts
   As this function receives an array of chars (with NULL terminator at the end), 
   everything should be separated (for example 2.2 should be '2','.','2'; functions like sqrt should have the chars separated aswell)
   It returns the result as a double
   It receives the expression as a array of chars */
double Math_interpreter_evaluate_expression(char *expression, bool *flag_err){

  char *copy = strdup(expression);
  char **tokens = Lexer_tokenize(copy);
  free(copy);

  bool is_valid = Parser_is_syntax_correct(tokens);
  if(!is_valid){
    *flag_err = true;
    free_tokens(tokens);
    return 0.0;
  }
  
  char **rpn = Parser_Shunting_yard(tokens);
    
  double result = Parser_evaluate_rpn(rpn);

  free_tokens(tokens);
  free_tokens(rpn);

  return result;
}
/* This program is part of the math interpreter, it is a parser that implements an expression syntax verifier, Shunting-yard algorithm
   and a RPN evaluator. All of those functions depends on the infix expression being already tokenized by the lexer and in a array of string format.
   It was made by Pedro Arthur Marchi [github.com/PAMarchi]. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <errno.h>

#include "../include/parser.h"

/* Function to tell if a token is a operator or not
   It returns true if the token is a operator
   It receives the token */
bool Parser_is_operator(const char *tok){

  return strlen(tok) == 1 && strchr("+-*/%^", tok[0]);
}

/* Similar to Parser_is_operator, but considers unary operator as such 
   It returns true if the token is a operator
   It receives the token */
bool Parser_is_any_operator(const char *tok){
  
  return Parser_is_operator(tok) || strcmp(tok, "u-") == 0;
}

/* Function to tell if a token is a number or not
   It returns true if the token is a number
   It receives the token */
bool Parser_is_number(const char *tok){

  return isdigit(tok[0]) || tok[0] == '.';
}

/* Function to tell if a token is a function or not
   It returns true if the token is a function
   It receives the token */
bool Parser_is_function(const char *tok){

  return isalpha(tok[0]) && strlen(tok)>1;
}

/* Function to return the precendence of a operator 
   It returns the precedence in form of a int
   It receives the operator to evaluate */
int Parser_precedence_of(const char *op){

  if(strcmp(op, "u-")==0)  
    return 5;
  if(strcmp(op, "^")==0)   
    return 4;
  if(strchr("*/%", op[0])) 
    return 3;
  if(strchr("+-", op[0]))  
    return 2;

  return 0;
}

/* Function to return the associativity of a operator
   It returns if the operator has RIGHT or LEFT associativity 
   It receives a operator */
Associativity Parser_assoc_of(const char *op){

  if(strcmp(op, "^")==0 || strcmp(op, "u-")==0)
    return RIGHT;

  return LEFT;
}

/* Function to return the arity of an operator 
   It returns 1 if is unary or 2 for binary */
int Parser_arity_of(const char *op){

  if(Parser_is_function(op) || strcmp(op, "u-")==0) 
    return 1;
  if(Parser_is_operator(op)) 
    return 2;

  return 0;
}

/* Function to analyse the syntax of the array representing the expression
   It returns true if the syntax is correct
   It receives the whole expression array */
bool Parser_is_syntax_correct(char **expression){

  int parentheses=0;
  char *previous_tok = NULL; // Previous token

  for(unsigned int i=0; expression[i]!=NULL; i++){

    char *current_token = expression[i];
    char *next_token = expression[i+1]; // Could be NULL

    // If the token is a number
    if(Parser_is_number(current_token)){

      unsigned int number_lenght = strlen(current_token);
      unsigned int dot_count=0;

      // Roam token to see if it find 2 dots, which is -> syntax error
      for(unsigned int j=0; j<(number_lenght-1); j++){

        if(current_token[j]=='.')
          dot_count++;
      }

      // If there is more than 1 dot in the expression -> error
      if(dot_count>1)
        return false;

      // If the number has a dot as the last char (no numbers after) -> syntax error
      if(current_token[number_lenght-1] == '.')
        return false;
    }

    // If the token is a function (like sqrt)
    else if(Parser_is_function(current_token)){

      // It should necessarily have a parentheses after, but it still checks right here
      if(next_token==NULL || strcmp(next_token, "(")!=0)
        return false;
      
    }

    // If is a '(', which here is not listed as an operator
    else if(strcmp(current_token, "(")==0)
      parentheses++;

    // If is a ')', which here is not listed as an operator
    else if(strcmp(current_token, ")")==0){

      if(parentheses==0)
        return false;
      
      parentheses--;
    }

    // If the token is a operator
    else if(Parser_is_operator(current_token)){

      bool is_unary = false;

      // Detect unary operator if "-" comes 
      // in the beginning of the expression, or
      // after "(" or another operator
      if(strcmp(current_token, "-")==0 && (previous_tok==NULL || Parser_is_operator(previous_tok) || strcmp(previous_tok, "(")==0))
        is_unary = true;

      if(is_unary){

        // Unary only comes before number, "(" or function
        if(next_token==NULL || !(Parser_is_number(next_token) || strcmp(next_token, "(")==0 || Parser_is_function(next_token)))
          return false;
      }
      // If its binary
      else{

        // Binary only comes after number or ")"
        if(!(previous_tok && (Parser_is_number(previous_tok) || strcmp(previous_tok, ")")==0)))
          return false;

        // Binary only comes before number, "(", function or unary
        if(next_token==NULL || !(Parser_is_number(next_token) || strcmp(next_token, "(")==0 || Parser_is_function(next_token) || strcmp(next_token, "-")==0))
          return false;
      }
    }
  
    // If the token is nothing listed
    else
      return false;

    previous_tok = current_token;
  }

  // Parentheses not balanced
  if(parentheses!=0)
    return false;
  
  // Last token cant be a binary operator
  if(previous_tok && Parser_is_operator(previous_tok))
    return false;
  
  return true;
}

/* Function to allocate memory for a string 
   Used in the Shunting-yard to duplicate tokens of the original array
   It returns a pointer to the string 
   It receives a string */
char *Parser_add_token(const char *src){

  size_t lenght = strlen(src);

  char *tok = malloc(lenght + 1);
  if (!tok) 
    return NULL;
  
  memcpy(tok, src, lenght + 1);

  return tok;
}

/* Function to convert infix (The infix is the NULL terminated array of tokens) tokens in RPN 
   It must be called after is_syntax_correct and only if the returned value is true
   It returns a new NULL terminated array of tokens in Reverse Polish Notation (RPN)
   It receives an NULL terminated array of tokens */
char **Parser_Shunting_yard(char **infix){

  TokenStack operator_stack;
  TokenQueue output_queue;
  Stack_init(&operator_stack);
  Queue_init(&output_queue);

  for(unsigned int i=0; infix[i]!=NULL; i++){

    char *tok          = infix[i];
    char *previous_tok = (i>0 ? infix[i-1] : NULL);

    // If the token is a number place the number in the output queue
    if(Parser_is_number(tok))
      Queue_enqueue(&output_queue, Parser_add_token(tok));

    // If the token is a function push it to the stack
    else if(Parser_is_function(tok))
      Stack_push(&operator_stack, Parser_add_token(tok));

    else if(Parser_is_operator(tok)){

      // Analyse if "-" is unary and if so, change to "u-"
      if(strcmp(tok, "-")==0 && (previous_tok==NULL || Parser_is_operator(previous_tok) || strcmp(previous_tok, "(")==0)){

        tok = "u-";
      }

      while(!Stack_is_empty(&operator_stack)){

        char *top = Stack_peek(&operator_stack);

        if(Parser_is_any_operator(top) && ((Parser_assoc_of(tok)==LEFT && Parser_precedence_of(tok) <= Parser_precedence_of(top)) || (Parser_assoc_of(tok)==RIGHT && Parser_precedence_of(tok) < Parser_precedence_of(top))))
          Queue_enqueue(&output_queue, Stack_pop(&operator_stack));
        else
          break;
      }

      Stack_push(&operator_stack, Parser_add_token(tok));
    }

    // If the token is a "("
    else if(strcmp(tok, "(")==0)
      Stack_push(&operator_stack, Parser_add_token(tok));

    // If the token is a ")"
    else if(strcmp(tok, ")")==0){

      // Pop until "("
      while(!Stack_is_empty(&operator_stack) && strcmp(Stack_peek(&operator_stack), "(")!=0)
        Queue_enqueue(&output_queue, Stack_pop(&operator_stack));

      // Remove "("
      if(!Stack_is_empty(&operator_stack) && strcmp(Stack_peek(&operator_stack), "(")==0)
        free(Stack_pop(&operator_stack));

      // If there is a function
      if(!Stack_is_empty(&operator_stack) && Parser_is_function(Stack_peek(&operator_stack)))
        Queue_enqueue(&output_queue, Stack_pop(&operator_stack));
    }
  }

  //quando o infix está vazio -> colocar tudo de stack no output queue
  while(!Stack_is_empty(&operator_stack))
    Queue_enqueue(&output_queue, Stack_pop(&operator_stack));

  // Convert the output_queue in array RPN
  char **rpn = Queue_to_array(&output_queue);
  Stack_free(&operator_stack);
  Queue_free(&output_queue);
  return rpn;
}

/* Function to evaluete a NULL terminated array of tokens in RPN 
   It returns the result of the expression in double format
   It receives a NULL terminated array in RPN */
double Parser_evaluate_rpn(char **rpn){

  DoubleStack values;
  DoubleStack_init(&values);

  for(unsigned int i=0; rpn[i]!=NULL; i++){

    char *tok = rpn[i];

    // If its a number
    if(Parser_is_number(tok))
      DoubleStack_push(&values,atof(tok));
    
    // If its an operator or function
    else{

      int arity = Parser_arity_of(tok); // Returns 1 for "-u" or "sqrt"; 2 for the rest of operators
      double args[2];

      for(int j=arity-1; j>=0; j--)
        args[j] = DoubleStack_pop(&values);

      double result;
      
      // Unary functions
      if(arity==1){

        if(strcmp(tok, "u-")==0)
          result = -args[0];
        else if(strcmp(tok, "sqrt")==0){

          if(args[0]<0) // If the number is negative
            result = NAN;
          else
            result = sqrt(args[0]);
        }
          
      }

      // Binary functions
      else{

        if(strcmp(tok, "+")==0)
          result = args[0] + args[1];
        else if(strcmp(tok, "-")==0)
          result = args[0] - args[1];
        else if(strcmp(tok, "*")==0)
          result = args[0] * args[1];
        else if(strcmp(tok, "^")==0)
          result = pow(args[0], args[1]);
        else if(strcmp(tok, "/")==0){

          if(args[1]==0.0) // Division by 0
            result=NAN;
          else 
            result = args[0] / args[1];
        }
        else if(strcmp(tok, "%")==0){
          
          if(args[1]==0.0) // Division by 0
            result=NAN;
          else 
            result = (int) args[0] % (int) args[1];
        }
      }

      DoubleStack_push(&values, result);
    }
  }

  double final_result = DoubleStack_pop(&values);
  DoubleStack_free(&values);

  return final_result;
}


  
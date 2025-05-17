/* This program is part of the math interpreter, it is a lexer that tokenizes an infix expression in form of a char array.
   It was made by Pedro Arthur Marchi [github.com/PAMarchi]. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/lexer.h"

/* Function to add a token, and does it by allocating memory and assigning a string to this area.
   It returns a pointer to the string allocated, and receives a string to allocate */
char *Lexer_add_token(const char *expression){

  unsigned long expr_lenght = strlen(expression);
  
  char *token = malloc((expr_lenght+1) * sizeof(char));
  
  strcpy(token, expression);
  
  return token;
}
  
  
/* Function to tokenize a mathematical expression.
   It receives a array of char (should be without spacing between chars)
   and returns a array of string */
char **Lexer_tokenize(char *expression){
    
  char **tokens; // Array of strings to store the tokens
  char supported_operators[] = {'+', '-', '/', '*', '%', '(', ')', '^', '\0'}; // Array of supported operators
      
    
  unsigned long total_chars = strlen(expression);
  unsigned long max_tokens = (total_chars*2)+1; // Have some margin
    
  tokens = malloc(max_tokens * sizeof(*tokens));
  if(!tokens) 
    return NULL;
      
    
  unsigned long index = 0;
  unsigned long token_index = 0;
  unsigned long tmp_for_realnum_index = 0;
  unsigned long tmp_for_letter_index = 0; 

  char tmp_for_realnum[(total_chars*2)+1]; // Temporary array to store digits, in order to tokenize a real number
  tmp_for_realnum[0] = '\0'; // Initialize the array with 0
  
  char tmp_for_letter[total_chars+1]; // Temporary array to store letters, in order to tokenize a function like sqrt 
  tmp_for_letter[0] = '\0'; // Initialize the array with 0

  while(index < total_chars){

    // In case of the char is a digit or dot
    if(isdigit(expression[index]) || expression[index] == '.'){
  
      // In case of the first char of the number is a dot -> add a 0 before
      if(expression[index] == '.' && tmp_for_realnum_index == 0){
        tmp_for_realnum[tmp_for_realnum_index++] = '0'; // Store digit 0 and increase index by 1
      }

      tmp_for_realnum[tmp_for_realnum_index++] = expression[index]; // Store current digit and increase index by 1
      tmp_for_realnum[tmp_for_realnum_index] = '\0'; // Add NULL terminator
  
      // If the next char is not a digit or a dot or if the next position is the NULL terminator
      if(index+1 >= total_chars || (!isdigit(expression[index+1]) && expression[index+1] != '.')){

        // Add the token, see if the pointer is not NULL (if so, free all memory already allocated), and increase the token index by 1
        tokens[token_index] = Lexer_add_token(tmp_for_realnum); 
        if(!tokens[token_index]){

          for(unsigned long i = 0; i < token_index; i++){
            free(tokens[i]);
          }
          free(tokens);
          return NULL;
        }
        token_index++;
          
        // Reset tmp_for_realnum
        tmp_for_realnum[0] = '\0';
        tmp_for_realnum_index = 0;
      }
    }
  
    // In case of the char is a operator
    else if(strchr(supported_operators, expression[index])){
      
      char str_tmp[2] = {expression[index], '\0'};

      // In case the char is a ( and the previous char is a number or ')'
      if(index>0 && expression[index] == '(' && (isdigit(expression[index-1]) || expression[index-1]==')')){

        char str_tmp2[2] = {'*', '\0'}; 

        tokens[token_index] = Lexer_add_token(str_tmp2);
        if(!tokens[token_index]){

          for(unsigned long i = 0; i < token_index; i++){
            free(tokens[i]);
          }
          free(tokens);
          return NULL;
        }
        token_index++;
      }

      int flag_add_asterisk=0;

      // In case the char is a ) and the next char is a number
      if(index+1<total_chars && expression[index] == ')' && isdigit(expression[index+1]))
        flag_add_asterisk=1;
    
      // Add the token, see if the pointer is not NULL (if so, free all memory already allocated), and increase the token index by 1
      tokens[token_index] = Lexer_add_token(str_tmp); 
      if(!tokens[token_index]){

        for(unsigned long i = 0; i < token_index; i++){
          free(tokens[i]);
        }
        free(tokens);
        return NULL;
      }
      token_index++;

      // In case the char is a ) and the next char is a number
      if(flag_add_asterisk==1){

        char str_tmp2[2] = {'*', '\0'}; 

        tokens[token_index] = Lexer_add_token(str_tmp2);
        if(!tokens[token_index]){

          for(unsigned long i = 0; i < token_index; i++){
            free(tokens[i]);
          }
          free(tokens);
          return NULL;
        }
        token_index++;
      }
    }
  
    // In case of the char is a letter, this is necessary to support functions like square root (sqrt)
    else if(isalpha(expression[index])){

      // In case the char is a ( and the previous char is a number or ')'
      if(index>0 && (isdigit(expression[index-1]) || expression[index-1]==')')){

        char str_tmp2[2] = {'*', '\0'}; 

        tokens[token_index] = Lexer_add_token(str_tmp2);
        if(!tokens[token_index]){

          for(unsigned long i = 0; i < token_index; i++){
            free(tokens[i]);
          }
          free(tokens);
          return NULL;
        }
        token_index++;
      }
      
      while(index < total_chars && isalpha(expression[index])){

        tmp_for_letter[tmp_for_letter_index++] = expression[index]; // Store current letter and increase index by 1
        tmp_for_letter[tmp_for_letter_index] = '\0'; // Add NULL terminator

        index++;
      }

      // Add the token, see if the pointer is not NULL (if so, free all memory already allocated), and increase the token index by 1
      tokens[token_index] = Lexer_add_token(tmp_for_letter); 
      if(!tokens[token_index]){

        for(unsigned long i = 0; i < token_index; i++){
          free(tokens[i]);
        }
        free(tokens);
        return NULL;
      }
      token_index++;

      // Reset tmp_for_letter 
      tmp_for_letter[0] = '\0';
      tmp_for_letter_index = 0;

      continue; // Go to the next iteration of while in order to skip the index incrementation below
    }

    index++; // Increments index
  }
  
  tokens[token_index] = NULL; // Indicates the end of the used memory positions
  return tokens;
}
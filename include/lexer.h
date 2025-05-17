/* This program is part of the math interpreter, it is a lexer that tokenizes an infix expression in form of a char array.
   It was made by Pedro Arthur Marchi [github.com/PAMarchi]. */

#ifndef LEXER_H
#define LEXER_H

/* Function to tokenize a mathematical expression.
   It receives a array of char (should be without spacing between chars)
   and returns a array of string */
   char **Lexer_tokenize(char *expression);

#endif
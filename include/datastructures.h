#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <stdbool.h>

// ------------------------------------------------ Tokens Stack ------------------------------------------------
                                                  
typedef struct{
  
  char **data; // Pointer to the tokens array
  unsigned int size; // Current number of tokens stored
  unsigned int cap; // Numbers of tokens that can be stored without realloc
} TokenStack;
  
/* Function to create and initialize the stack
   It receives a reference to the stack */
void Stack_init(TokenStack *stack);

/* Function to free the stack
   It receives a reference to the stack to free */
void Stack_free(TokenStack *stack);

/* Function to tell whether the stack is empty or not
   It returns true if is empty
   It receives a reference to the stack */
bool Stack_is_empty(TokenStack *stack);

/* Function to insert an element in the top of the stack, do the realloc if necessary
   It receives a reference to the stack and the token to push */
void Stack_push(TokenStack *stack, char *tok);

/* Function to remove the element of the top of the stack
   It returns the token popped
   It receives a reference to the stack */
char *Stack_pop(TokenStack *stack);
   
/* Function to return the value of the element of the top of the stack 
   It returns the token
   It receives a reference to the stack */
char *Stack_peek(TokenStack *stack);

// ------------------------------------------------ Token Queue ------------------------------------------------

typedef struct{
  
  char **data; // Pointer to the tokens array
  unsigned int size; // Current number of tokens stored
  unsigned int cap; // Numbers of tokens that can be stored without realloc
} TokenQueue;
  
/* Function to create and initialize the queue
   It receives a reference to the queue */
void Queue_init(TokenQueue *queue);

/* Function to free the queue
   It receives a reference to the queue to free */
void Queue_free(TokenQueue *queue);

/* Function to tell whether the queue is empty or not
   It returns true if is empty
   It receives a reference to the queue */
bool Queue_is_empty(TokenQueue *queue);

/* Function to insert an element at the end of the queue, do the realloc if necessary
   It receives a reference to the queue and the token to push */
void Queue_enqueue(TokenQueue *queue, char *tok);

/* Function to remove the element of the front of the queue and move the others
   It returns the token dequeued
   It receives a reference to the queue */
char *Queue_dequeue(TokenQueue *queue);
   
/* Function to convert the current content into a NULL-terminated array (Required to the function that will evaluate the RPN)
   It returns a new malloc
   It receives a reference to the queue */
char **Queue_to_array(TokenQueue *queue);

// ------------------------------------------------ Double Stack ------------------------------------------------

typedef struct{

  double *data; // Dynamic array of double values
  unsigned int size; // Current number of values stored
  unsigned int cap; // Numbers of values that can be stored without realloc
} DoubleStack;

/* Function to create and initialize the stack
   It receives a reference to the stack */
void DoubleStack_init(DoubleStack *dstack);

/* Function to free the stack
   It receives a reference to the stack to free */
void DoubleStack_free(DoubleStack *dstack);
   
/* Function to tell whether the stack is empty or not
   It returns true if is empty
   It receives a reference to the stack */
bool DoubleStack_is_empty(DoubleStack *dstack);
   
/* Function to insert an element in the top of the stack, do the realloc if necessary
   It receives a reference to the stack and the token to push */
void DoubleStack_push(DoubleStack *dstack, double value);
   
/* Function to remove the element of the top of the stack
   It returns the popped value or 0.0 if empty
   It receives a reference to the stack */
double DoubleStack_pop(DoubleStack *dstack);
      
/* Function to return the value of the element of the top of the stack 
   It returns the value of the top, or 0.0 if empty
   It receives a reference to the stack */
double DoubleStack_peek(DoubleStack *dstack);

#endif
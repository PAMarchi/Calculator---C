#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../include/datastructures.h"

// ------------------------------------------------ Tokens Stack ------------------------------------------------

/* Function to create and initialize the stack
   It receives a reference to the stack */
void Stack_init(TokenStack *stack){

  stack->data = NULL;
  stack->size = stack->cap = 0;
}

/* Function to free the stack
   It receives a reference to the stack to free */
void Stack_free(TokenStack *stack){

  free(stack->data);
  stack->data = NULL;
  stack->size = stack->cap = 0;
}

/* Function to tell whether the stack is empty or not
   It returns true if is empty
   It receives a reference to the stack */
bool Stack_is_empty(TokenStack *stack){
  
  return (stack->size == 0);
}

/* Function to insert an element in the top of the stack, do the realloc if necessary
   It receives a reference to the stack and the token to push */
void Stack_push(TokenStack *stack, char *tok){

  unsigned int cap = stack->cap;
  unsigned int size = stack->size;

  if(size == cap){
    unsigned int newcap = cap ? cap*2 : 4;
    stack->data = realloc(stack->data, newcap * sizeof(char*));
    stack->cap = newcap;
  }
  stack->data[stack->size++]=tok;
}

/* Function to remove the element of the top of the stack
   It returns the token popped
   It receives a reference to the stack */
char *Stack_pop(TokenStack *stack){

  // If the stack is empty
  if(Stack_is_empty(stack))
    return NULL;

  return stack->data[--stack->size];
}
  
/* Function to return the value of the element of the top of the stack 
   It returns the token
   It receives a reference to the stack */
char *Stack_peek(TokenStack *stack){

  if(Stack_is_empty(stack))
    return NULL;

  return stack->data[stack->size-1];
}

// ------------------------------------------------ Token Queue ------------------------------------------------

/* Function to create and initialize the queue
   It receives a reference to the queue */
void Queue_init(TokenQueue *queue){

  queue->data = NULL;
  queue->size = queue->cap = 0;
}

/* Function to free the queue
   It receives a reference to the queue to free */
void Queue_free(TokenQueue *queue){

  free(queue->data);
  queue->data = NULL;
  queue->size = queue->cap = 0;
}

/* Function to tell whether the queue is empty or not
   It returns true if is empty
   It receives a reference to the queue */
bool Queue_is_empty(TokenQueue *queue){

  return (queue->size == 0);
}

/* Function to insert an element at the end of the queue, do the realloc if necessary
   It receives a reference to the queue and the token to push */
void Queue_enqueue(TokenQueue *queue, char *tok){

  unsigned int cap = queue->cap;
  unsigned int size = queue->size;

  if(size == cap){
    unsigned int newcap = cap ? cap*2 : 4;
    queue->data = realloc(queue->data, newcap * sizeof(char*));
    queue->cap = newcap;
  }
  queue->data[queue->size++]=tok; 
}

/* Function to remove the element of the front of the queue and move the others
   It returns the token dequeued
   It receives a reference to the queue */
char *Queue_dequeue(TokenQueue *queue){

  if(Queue_is_empty(queue))
    return NULL;

  char *token_dequeued = queue->data[0];

  unsigned int index = 0;
  unsigned int size = queue->size;
  while(index<size-1){

    queue->data[index] = queue->data[index+1];
    index++;
  }

  queue->size--;
  return token_dequeued; 
}
   
/* Function to convert the current content into a NULL-terminated array (Required to the function that will evaluate the RPN)
   It returns a new malloc
   It receives a reference to the queue */
char **Queue_to_array(TokenQueue *queue){
  
  // Transforms data[0...size-1] into a char** NULL terminated
  char **out = malloc((queue->size+1) * sizeof(char*));
  memcpy(out, queue->data, queue->size * sizeof(char*));
  out[queue->size] = NULL;
  
  return out;
}

// ------------------------------------------------ Double Stack ------------------------------------------------

/* Function to create and initialize the stack
   It receives a reference to the stack */
void DoubleStack_init(DoubleStack *dstack){
  
  dstack->data = NULL;
  dstack->size = dstack->cap = 0;
}

/* Function to free the stack
   It receives a reference to the stack to free */
void DoubleStack_free(DoubleStack *dstack){

  free(dstack->data);
  dstack->data = NULL;
  dstack->size = dstack->cap = 0;
}
      
/* Function to tell whether the stack is empty or not
   It returns true if is empty
   It receives a reference to the stack */
bool DoubleStack_is_empty(DoubleStack *dstack){
  return (dstack->size == 0);
}
      
/* Function to insert an element in the top of the stack, do the realloc if necessary
   It receives a reference to the stack and the token to push */
void DoubleStack_push(DoubleStack *dstack, double value){

  unsigned int cap = dstack->cap;
  unsigned int size = dstack->size;

  if(size == cap){
    unsigned int newcap = cap ? cap*2 : 4;
    dstack->data = realloc(dstack->data, newcap * sizeof(double));
    dstack->cap = newcap;
  }
  dstack->data[dstack->size++]=value;
}
      
/* Function to remove the element of the top of the stack
   It returns the popped value or 0.0 if empty
   It receives a reference to the stack */
double DoubleStack_pop(DoubleStack *dstack){

  // If the stack is empty
  if(DoubleStack_is_empty(dstack))
    return 0.0;

  return dstack->data[--dstack->size];
}
         
/* Function to return the value of the element of the top of the stack 
   It returns the value of the top, or 0.0 if empty
   It receives a reference to the stack */
double DoubleStack_peek(DoubleStack *dstack){

  if(DoubleStack_is_empty(dstack))
    return 0.0;

  return dstack->data[dstack->size-1];
}
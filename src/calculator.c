/* This program is a calculator, it has GUI (using GTK) and uses the Shunting-yard + evaluation of RPN as algorithm.
   It was made by Pedro Arthur Marchi [github.com/PAMarchi]. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <gtk-4.0/gtk/gtk.h> // gtk library for GUI

#include "../include/math_interpreter.h"

#define INITIAL_SIZE_BUFFER 2
#define TOTAL_ELEMENTS 30


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum for the UI element
typedef enum {
  window,
  grid,
  result_field,
  result,
  separator,
  input_field,
  input,
  button_back,
  button_clear,
  button_9,
  button_8,
  button_7,
  button_mod,
  button_openbrackets,
  button_division,
  button_6,
  button_5,
  button_4,
  button_sqrt,
  button_closebrackets,
  button_minus,
  button_3,
  button_2,
  button_1,
  button_equals,
  button_multiplication,
  button_plus,
  button_power,
  button_dot,
  button_0
} _ELEMENT;


/* Struct for the program buffer, store references to the input and result fields,
   also store the reference to the expression char array and the result char array */ 
typedef struct{

  GtkWidget *result_field, *input_field; // Reference to input and result text in the GUI

  char *expression;                      // Array of char expression will be use to tokenize    
  char *result;                          // Result of the operation          
} calculator_buffer;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *double_to_string(double value){

  int space_needed = snprintf(NULL, 0, "%g", value);

  // Formatting failed
  if(space_needed<0)
    return NULL;
  
  space_needed++; // For the '\0'

  char *str_res = malloc(space_needed * sizeof(char));
  if(str_res==NULL)
    return NULL;

  snprintf(str_res, space_needed, "%g", value);
  return str_res;
}

/* Function to calculate the result of the current expression 
   It returns a bool telling if there was a error or not
   It receives a pointer to the buffer and a boolean flag for error checking */
bool get_result(gpointer buffer, bool *has_err){

  calculator_buffer *pBuffer = buffer;

  char *expression = pBuffer->expression;
  char *result     = pBuffer->result;

  double expression_res;
  
  expression_res = Math_interpreter_evaluate_expression(expression, has_err);
  if(*has_err)
    return *has_err; // Which is true
  
  result = double_to_string(expression_res);
  if(result==NULL)
    return *has_err=true;

  pBuffer->result = result; // Redirect the pointer to the new memory area

  return *has_err;
}

/* Function to assign the char to the expression in buffer.
   It returns a  int, 1 if realloc fail and 0 if sucess,
   it receives two arguments, the value of the char to assign
   and a generic pointer to the buffer */
int register_char(char c, gpointer buffer){
 
  calculator_buffer *pBuffer = buffer;
  char *oldbuffer = pBuffer->expression; // Pointer to improve readability, points to main expression


  unsigned long old_lenght = strlen(oldbuffer); // Current lenght of the array
  unsigned long new_lenght = old_lenght+2; // Char + '\0'


  char *newbuffer = realloc(oldbuffer, new_lenght * sizeof(char));

  if(!newbuffer) // If fail
    return 1;

  pBuffer->expression = newbuffer; // Redirect the pointer to the new memory area

  // Concat chars for the main expression
  newbuffer[new_lenght-2] = c;
  newbuffer[new_lenght-1] = '\0';
  
  return 0;
}


/* Function to assign a string to the expression in buffer.
   The string will be separated in multiple chars inside this function.
   It returns a  int, 1 if realloc fail and 0 if sucess,
   it receives two arguments, the value of the string to assign
   and a generic pointer to the buffer */
int register_function(char *str, gpointer buffer){

  unsigned long str_lenght = strlen(str);

  calculator_buffer *pBuffer = buffer;
  char *oldbuffer = pBuffer->expression; // Pointer to improve readability, points to main expression

  unsigned long old_lenght = strlen(oldbuffer); // Current lenght of the array
  unsigned long new_lenght = old_lenght + (str_lenght+1); // The lenght of the string + '\0'

  char *newbuffer = realloc(oldbuffer, new_lenght * sizeof(char));

  if(!newbuffer) // If fail
    return 1;

  pBuffer->expression = newbuffer; // Redirect the pointer to the new memory area

  unsigned long index_to_write = old_lenght; // Index to point to where it should start writing the chars

  for(unsigned long i=0; i<str_lenght; i++){
    newbuffer[index_to_write++] = str[i];
  }
  newbuffer[index_to_write] = '\0';

  // Add the '(' for the next char
  register_char('(', pBuffer);

  return 0;
}


/* Function to clear the expression buffer,
   it receives a generic pointer to the buffer*/
void clean_buffer(gpointer buffer){

  calculator_buffer *pBuffer = buffer;

  pBuffer->expression[0] = '\0'; // Clean the expression field
}


/* Function to remove the last char from the expression buffer,
   it receives a generic pointer to the buffer*/
void remove_one_char_buffer(gpointer buffer){

  calculator_buffer *pBuffer = buffer;
  
  unsigned long lenght = strlen(pBuffer->expression);

  // If there is no char, return
  if(lenght==0)
    return;

  unsigned long last_char = lenght-1; // Index to point to where the last char is written, which is the lenght - '\0'

  if(pBuffer->expression[last_char]=='('){
  
    pBuffer->expression[last_char] = '\0'; // To delete the '('

    if(last_char==0)
      return;

    last_char--;

    // If the char to delete is a letter, that means that is a function, so it needs to be erased entirely
    if(isalpha(pBuffer->expression[last_char])){

      while(last_char>0 && isalpha(pBuffer->expression[last_char])){
        last_char--;
      }
    
      // In case last char stopped at a non letter char
      if(!isalpha(pBuffer->expression[last_char]))
        last_char++;
      
      pBuffer->expression[last_char] = '\0';
    }

    return;
  }
  
  pBuffer->expression[last_char] = '\0';
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* Function that will be called from the buttons listener function
   It returns nothing and receives a reference of the pressed button
   and a generic pointer to the application buffer */
void on_button_clicked(GtkButton *b, gpointer buffer){

  calculator_buffer *pBuffer = buffer;

  int button_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(b), "button_id")); // Retrieves data from calling function in order to identify the button that was pressed
  
  switch(button_id){
    case button_back:
      remove_one_char_buffer(pBuffer);
      gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_clear:
      clean_buffer(pBuffer);
      gtk_label_set_label(GTK_LABEL(pBuffer->input_field), "");
      gtk_label_set_label(GTK_LABEL(pBuffer->result_field), "");
      break;
    case button_9:
      if(register_char('9', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_8:
      if(register_char('8', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_7:
      if(register_char('7', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_mod:
      if(register_char('%', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_openbrackets:
      if(register_char('(', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_division:
      if(register_char('/', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_6:
      if(register_char('6', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_5:
      if(register_char('5', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_4:
      if(register_char('4', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_sqrt:
      if(register_function("sqrt", pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_closebrackets:
      if(register_char(')', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_minus:
      if(register_char('-', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_3:
      if(register_char('3', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_2:
      if(register_char('2', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_1:
      if(register_char('1', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_equals:
      bool has_err=false;
      get_result(pBuffer, &has_err);
      if(!has_err)
        gtk_label_set_label(GTK_LABEL(pBuffer->result_field), pBuffer->result);
      else 
        gtk_label_set_label(GTK_LABEL(pBuffer->result_field), "Erro");
      break;
    case button_multiplication:
      if(register_char('*', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_plus:
      if(register_char('+', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_power:
      if(register_char('^', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_dot:
      if(register_char('.', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
    case button_0:
      if(register_char('0', pBuffer) == 0)
        gtk_label_set_label(GTK_LABEL(pBuffer->input_field), pBuffer->expression);
      break;
  }
}


/* Function that initiate all the buttons to listen when clicked.
   Also the function set the id of the button via set data method.
   It returns nothing and receives a reference to a pointer
   (which is an array of pointers in the calling function) of UI elements */
void buttons_set_listener(GtkWidget **element, gpointer buffer){

  // For loop to insert data to button handler function and activate the button listeners
  for(int i=7; i<TOTAL_ELEMENTS; i++){
    g_object_set_data(G_OBJECT(element[i]), "button_id", GINT_TO_POINTER(i));
    g_signal_connect(element[i], "clicked", G_CALLBACK(on_button_clicked), buffer);
  }
}


/* Function to set the program window and its elements.
   It returns nothing, and receives a reference to the app 
   and program buffer in generic pointer format*/
void activate(GtkApplication *app, gpointer buffer){

  GtkBuilder *builder; // GUI builder
  GtkWidget *element[TOTAL_ELEMENTS]; // Array of pointers that will point to all elements from GUI

  builder = gtk_builder_new_from_file("GUI.glade"); // Assing the GUI from glade to the builder

  // Referentiating the GUI elements to element pointers
  element[window]                          = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  element[grid]                            = GTK_WIDGET(gtk_builder_get_object(builder, "WindowGrid"));
  element[result_field]                    = GTK_WIDGET(gtk_builder_get_object(builder, "ResultField"));
  element[result]                          = GTK_WIDGET(gtk_builder_get_object(builder, "Result"));
  element[separator]                       = GTK_WIDGET(gtk_builder_get_object(builder, "Separator"));
  element[input_field]                     = GTK_WIDGET(gtk_builder_get_object(builder, "InputField"));
  element[input]                           = GTK_WIDGET(gtk_builder_get_object(builder, "Input"));
  element[button_back]                     = GTK_WIDGET(gtk_builder_get_object(builder, "button_back"));
  element[button_clear]                    = GTK_WIDGET(gtk_builder_get_object(builder, "button_clear"));
  element[button_9]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_9"));
  element[button_8]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_8"));
  element[button_7]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_7"));
  element[button_mod]                      = GTK_WIDGET(gtk_builder_get_object(builder, "button_mod"));
  element[button_openbrackets]             = GTK_WIDGET(gtk_builder_get_object(builder, "button_openbrackets"));
  element[button_division]                 = GTK_WIDGET(gtk_builder_get_object(builder, "button_division"));
  element[button_6]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_6"));
  element[button_5]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_5"));
  element[button_4]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_4"));
  element[button_sqrt]                     = GTK_WIDGET(gtk_builder_get_object(builder, "button_sqrt"));
  element[button_closebrackets]            = GTK_WIDGET(gtk_builder_get_object(builder, "button_closebrackets"));
  element[button_minus]                    = GTK_WIDGET(gtk_builder_get_object(builder, "button_minus"));
  element[button_3]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_3"));
  element[button_2]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_2"));
  element[button_1]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_1"));
  element[button_equals]                   = GTK_WIDGET(gtk_builder_get_object(builder, "button_equals"));
  element[button_multiplication]           = GTK_WIDGET(gtk_builder_get_object(builder, "button_multiplication"));
  element[button_plus]                     = GTK_WIDGET(gtk_builder_get_object(builder, "button_plus"));
  element[button_power]                    = GTK_WIDGET(gtk_builder_get_object(builder, "button_power"));
  element[button_dot]                      = GTK_WIDGET(gtk_builder_get_object(builder, "button_dot"));
  element[button_0]                        = GTK_WIDGET(gtk_builder_get_object(builder, "button_0"));

  // Assign the reference to input and result text in the GUI to the buffer
  calculator_buffer *pBuffer = buffer;
  pBuffer->input_field = element[input];
  pBuffer->result_field = element[result];

  // Set listeners to the buttons and make them work
  buttons_set_listener(element, buffer);

  // For showing the application window
  gtk_window_set_application(GTK_WINDOW(element[window]), app); // Sets the program window
  gtk_widget_show(GTK_WIDGET(element[window])); // Show the program window
}

int main(int argc, char *argv[]){

  g_setenv("LC_ALL", "C", 1); // Dot used as decimal separator

  char *expression = malloc(INITIAL_SIZE_BUFFER * sizeof(char)); // Init the expression char array with INITIAL_SIZE_BUFFER times the size of char
  
  expression[0] = '\0'; // Initiate the expression with end of string

  calculator_buffer buffer = {0}; // Initiate the program buffer with 0s

  buffer.expression = expression; // Assign to buffer var the dynamic allocated char array

  // GUI --------------------------------------------------------------

  GtkApplication *app;
  
  app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK (activate), &buffer); // Calls the activate function to set the window and components, also passes the buffer to the function in generic pointer format

  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  // ------------------------------------------------------------------

  free(buffer.expression);
  free(buffer.result);
  return status;
}
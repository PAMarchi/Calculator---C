#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "../include/math_interpreter.h"

typedef struct{

  char *expression;
  double expected_output;
  bool expected_error;
} Test;

/* Function to compare the expected and real results 
   It returns a boolean, true means the result is equals to the expected 
   It receives the expected and real result and error */
bool is_result_correct(double expected_res, double real_res, bool expected_err, bool real_err){

  // If the expected and real error match
  if(real_err==expected_err){

    // If the expected result is NAN
    if(isnan(expected_res)){
      
      if(isnan(real_res))
        return true;
      else
        return false;
    }
    // If the expected result is double
    else{

      if(real_res==expected_res)
        return true;
      else
        return false;
    }

  }
  // If expected and real error dont match
  else
    return false;
}

int main(){

  Test to_test[] = {
                    // Simple ops
                    {"3+10"            ,  13.0, false},
                    {"2*3"             ,   6.0, false},
                    {"3/2"             ,   1.5, false},
                    {"0.5*3"           ,   1.5, false},
                    {".1*10"           ,   1.0, false},
                    // Precendence and parentheses
                    {"2+2*3"           ,   8.0, false},
                    {"(2)(1.5)3"       ,   9.0, false},
                    {"2.2(.5+1.5)"     ,   4.4, false},
                    // Unary
                    {"-2.2(.5+1.5)"    ,  -4.4, false},
                    {"2^-3"            , 0.125, false},
                    // Functions
                    {"2sqrt(9)2"       ,    12, false},
                    {"-2*(sqrt(6+3)/2)",  -3.0, false},
                    {"sqrt(-2)"        ,   NAN, false},
                    // Mod
                    {"5%2"             ,   1.0, false},
                    // Syntax errors
                    {"."               ,   0.0,  true},
                    {"5%"              ,   0.0,  true},
                    {"(1+2"            ,   0.0,  true},
                    // NULL
                    {NULL              ,   0.0,  true}
                  };

  int fail=0;

  for(int i=0; to_test[i].expression!=NULL; i++){
  
    bool error=false;
    char *copy = strdup(to_test[i].expression);

    double result = Math_interpreter_evaluate_expression(copy, &error);
    free(copy);

    // If the test failed
    if(!is_result_correct(to_test[i].expected_output, result, to_test[i].expected_error, error)){
      
      fprintf(stderr, "\nTest %d failed. Output: %lf; Expected output: %lf. Error: %d; Expected error: %d\n", i, result, to_test[i].expected_output, error, to_test[i].expected_error);
      fail++;
    }
    else
      printf("\nTest %d passed. Result: %lf\n", i, result);

  }

  if(fail!=0){

    fprintf(stderr, "\n%d test(s) failed.\n", fail);
    return EXIT_FAILURE;
  }
  else{

    printf("\nAll tests passed!\n");
    return EXIT_SUCCESS;
  }
}
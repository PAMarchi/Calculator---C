# Calculator

A calculator made in C language. It implements some operations and has support to brackets and real numbers.

## Supported operations

- addition (+)
- subtraction (-)
- multiplication (*)
- division (/)
- power (^)
- mod (%)
- square root (√)/(sqrt)
  
## About files organization and algorithms used

The program uses the GTK library to implement a GUI. For the calculator algorithm, it uses the Shunting-yard to convert the input into RPN (Reverse Polish Notation) that is further evaluated.

Regarding the files:

- calculator: main program.
- datastructures: contains data structures implementations, like stacks and queues.
- lexer: convert the input into tokens.
- parser: contains a function that analyzes the input syntax, also comprehends the mathematical analysis part of the calculator (Shunting-yard + RPN evaluation).
- math_interpreter: interface between the GUI (main program) and the logical part.

## Dependencies

This program depends on GTK4 library.

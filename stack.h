#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

// Settings for using stack:
//-----------------------------------------------------------------------------
#define START_NUM_OF_ELEMENTS 2
#define STACK_MULTIPLY_CONST 2
#define STACK_DIVIDE_CONST 2
#define STACK_DIVIDE_TRIGGER 4
#define KEY_FOR_PRINTF "%d "      // for int
#define TYPE_OF_ELEMENT int
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Stack {
    int start_canary_of_stack_struct;
    void* stack_pointer;
    int num_of_elements;
    int max_num_of_elements;
    int size_of_element;
    int offset;
    int size_of_allocated_mem;
    int size_of_canary;
    unsigned long hash;
    int end_canary_of_stack_struct;
};

enum Error {
    OK = 0,
    CANT_ALLOCATE_MEMORY_INIT_STACK,
    CANT_ALLOCATE_MEMORY_PUSH,
    NO_ELEMENTS_TO_POP,
    CANT_ALLOCATE_MEMORY_POP,
    NO_ELEMENTS_TO_TOP,
    INVALID_CANARY_OF_STACK,
    INVALID_CANARY_OF_STACK_ARRAY,
    CANT_INSTALL_CANARIES,
    INVALID_HASH_OF_STACK
};


int stack_init (struct Stack* stack, int size_of_element);
int stack_push (struct Stack* stack, void* element);
int stack_pop (struct Stack* stack, void* return_element);
int stack_print (struct Stack* stack);
int stack_top (struct Stack* stack, void* return_element);
bool stack_isEmpty (struct Stack* stack);
int stack_size (struct Stack* stack);
int stack_print_file(struct Stack* stack, FILE* file);
int stack_destructor(struct Stack* stack);

void end_of_program ();


#endif


// Plan to do
// Unit tests.
#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

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
    int end_canary_of_stack_struct;
};

enum Error {
    OK = 0,
    CANT_ALLOCATE_MEMORY_INIT_STACK,
    CANT_ALLOCATE_MEMORY_PUSH,
    NO_ELEMENTS_TO_POP,
    CANT_ALLOCATE_MEMORY_POP
};


int stack_init (struct Stack* stack, int size_of_element);
int stack_push (struct Stack* stack, void* element);
int stack_pop (struct Stack* stack, void* return_element);
int stack_print (struct Stack* stack);


#endif


// Need to realise
// push() to insert an element into the stack
// pop() to remove an element from the stack
// top() Returns the top element of the stack.
// isEmpty() returns true is stack is empty else false
// size() returns the size of stack

// Сделать функцию обработки и вывода сообщений по ошибке errno 
#include "stack.h"

#define START_NUM_OF_ELEMENTS 2
#define STACK_MULTIPLY_CONST 2
#define STACK_DIVIDE_CONST 2
#define STACK_DIVIDE_TRIGGER 4
#define KEY_FOR_PRINTF "%d "      // for int
#define TYPE_OF_ELEMENT int

#define CANARY_FOR_STACK 0xDEDBAD

// struct Stack {
//     int start_canary_of_stack_struct;
//     void* stack_pointer;
//     int num_of_elements;
//     int max_num_of_elements;
//     int size_of_element;
//     int offset;
//     int size_of_allocated_mem;
//     int end_canary_of_stack_struct;
// };

int stack_init (struct Stack* stack, int size_of_element) {
    assert (stack);

    stack->start_canary_of_stack_struct = CANARY_FOR_STACK;
    stack->end_canary_of_stack_struct = CANARY_FOR_STACK;
    stack->num_of_elements = 0;
    stack->max_num_of_elements = START_NUM_OF_ELEMENTS;
    stack->size_of_element = size_of_element;
    stack->offset = 0;
    stack->size_of_allocated_mem = stack->max_num_of_elements * stack->size_of_element;
    stack->stack_pointer = (void*)malloc(stack->size_of_allocated_mem);

    if (!stack->stack_pointer) return CANT_ALLOCATE_MEMORY_INIT_STACK;

    return OK;
}

int stack_push (struct Stack* stack, void* element) {
    assert (stack);

    if (stack->num_of_elements == stack->max_num_of_elements) {
        stack->size_of_allocated_mem += stack->max_num_of_elements * stack->size_of_element;
        stack->max_num_of_elements *= STACK_MULTIPLY_CONST;
        stack->stack_pointer = (void*)realloc(stack->stack_pointer, stack->size_of_allocated_mem);
        if (!stack->stack_pointer) return CANT_ALLOCATE_MEMORY_PUSH;
    }
    memcpy (stack->stack_pointer + stack->offset, element, stack->size_of_element);
    stack->num_of_elements += 1;
    stack->offset += stack->size_of_element;
    return OK;
}

int stack_pop (struct Stack* stack, void* return_element) {
    assert (stack);

    if (stack->num_of_elements == 0) return NO_ELEMENTS_TO_POP;
    stack->num_of_elements -= 1;
    stack->offset -= stack->size_of_element;
    memcpy (return_element, stack->stack_pointer + stack->offset, stack->size_of_element);

    if (stack->num_of_elements * STACK_DIVIDE_TRIGGER <= stack->max_num_of_elements) {
        stack->max_num_of_elements /= STACK_DIVIDE_CONST;
        stack->size_of_allocated_mem -= stack->max_num_of_elements * stack->size_of_element;
        stack->stack_pointer = (void*)realloc(stack->stack_pointer, stack->size_of_allocated_mem);
        if (!stack->stack_pointer) return CANT_ALLOCATE_MEMORY_POP;
    }

    return OK;
}

int stack_print (struct Stack* stack) {
    assert (stack);

    for (int i = stack->offset; i >= 0; i -= stack->size_of_element) {
        printf (KEY_FOR_PRINTF, *(TYPE_OF_ELEMENT*) stack->stack_pointer + i);
        printf ("\n");
    }
    return OK;
}
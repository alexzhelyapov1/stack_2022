#include "stack.h"
#include "stack_dev.h"

FILE* log_file = file_logging_init("log_file.txt");

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

int stack_init(struct Stack* stack, int size_of_element) {
    assert(stack);

    stack->start_canary_of_stack_struct = CANARY_FOR_STACK;
    stack->end_canary_of_stack_struct = CANARY_FOR_STACK;
    stack->num_of_elements = 0;
    stack->max_num_of_elements = START_NUM_OF_ELEMENTS;
    stack->size_of_element = size_of_element;
    stack->offset = sizeof (CANARY_FOR_STACK_ARRAY);
    stack->size_of_canary = sizeof (CANARY_FOR_STACK_ARRAY);
    stack->size_of_allocated_mem = stack->max_num_of_elements * stack->size_of_element + 2 * stack->size_of_canary;
    stack->stack_pointer = (void*)malloc(stack->size_of_allocated_mem);
    if (!stack->stack_pointer) return CANT_ALLOCATE_MEMORY_INIT_STACK;
    if (!install_canaries(stack)) return CANT_INSTALL_CANARIES;

    return OK;
}

int stack_push(struct Stack* stack, void* element) {
    assert(stack);
    dpf(fprintf(log_file, "Pushing elemnt %d\n", *((int*)element));)

    if (stack->num_of_elements == stack->max_num_of_elements) {
        stack->size_of_allocated_mem += stack->max_num_of_elements * stack->size_of_element;
        stack->max_num_of_elements *= STACK_MULTIPLY_CONST;
        stack->stack_pointer = (void*)realloc(stack->stack_pointer, stack->size_of_allocated_mem);
        if (!stack->stack_pointer) return CANT_ALLOCATE_MEMORY_PUSH;
        if (!install_canaries(stack)) return CANT_INSTALL_CANARIES;
    }
    memcpy(stack->stack_pointer + stack->offset, element, stack->size_of_element);
    stack->num_of_elements += 1;
    stack->offset += stack->size_of_element;
    return OK;
}

int stack_pop(struct Stack* stack, void* return_element) {
    assert(stack);

    if (stack_isEmpty(stack)) return NO_ELEMENTS_TO_POP;
    stack->num_of_elements -= 1;
    stack->offset -= stack->size_of_element;
    memcpy(return_element, stack->stack_pointer + stack->offset, stack->size_of_element);

    if (stack->num_of_elements * STACK_DIVIDE_TRIGGER <= stack->max_num_of_elements) {
        stack->max_num_of_elements /= STACK_DIVIDE_CONST;
        stack->size_of_allocated_mem -= stack->max_num_of_elements * stack->size_of_element;
        stack->stack_pointer = (void*)realloc(stack->stack_pointer, stack->size_of_allocated_mem);
        if (!stack->stack_pointer) return CANT_ALLOCATE_MEMORY_POP;
        if (!install_canaries(stack)) return CANT_INSTALL_CANARIES;
    }

    dpf(fprintf(log_file, "Popping elemnt %d\n", *((int*)return_element));)
    return OK;
}

int stack_print(struct Stack* stack) {
    assert(stack);

    dpf(fprintf(log_file, "Printing stack. First element on the top is = %d\n",    \
                *((int*)(stack->stack_pointer + stack->offset)));)
    printf("Stack: ");
    for (int i = stack->offset - stack->size_of_element; i >= 0; i -= stack->size_of_element) {
        printf(KEY_FOR_PRINTF, *(TYPE_OF_ELEMENT*)(stack->stack_pointer + i));
        printf(" ");
    }
    if (stack_isEmpty(stack))
        printf("*stack is empty*");
    printf("\n");
    return OK;
}

int stack_top(struct Stack* stack, void* return_element) {
    assert(stack);

    if (stack_isEmpty(stack)) {
        return NO_ELEMENTS_TO_TOP;
    }
    int local_offset = stack->offset - stack->size_of_element;
    memcpy(return_element, stack->stack_pointer + local_offset, stack->size_of_element);
    return OK;
}

bool stack_isEmpty(struct Stack* stack) {
    assert(stack);

    if (stack->num_of_elements == 0) return true;
    return false;
}

int stack_size (struct Stack* stack) {
    assert(stack);

    return stack->num_of_elements;
}

int stack_valid (struct Stack* stack) {
    assert(stack);

    if (stack->start_canary_of_stack_struct != CANARY_FOR_STACK || \
        stack->end_canary_of_stack_struct != CANARY_FOR_STACK)
            return INVALID_CANARY_OF_STACK;
    if (!strcmp((char*)stack->stack_pointer, CANARY_FOR_STACK_ARRAY) || \
        !strcmp((char*)(stack->stack_pointer + stack->max_num_of_elements * stack->size_of_element), CANARY_FOR_STACK_ARRAY))
            return INVALID_CANARY_OF_STACK_ARRAY;
    return OK;
}

int install_canaries(struct Stack* stack) {
    assert(stack);

    memcpy(stack->stack_pointer, CANARY_FOR_STACK_ARRAY, sizeof (CANARY_FOR_STACK_ARRAY));
    memcpy(stack->stack_pointer + stack->max_num_of_elements * stack->size_of_element + stack->size_of_canary, \
        CANARY_FOR_STACK_ARRAY, sizeof (CANARY_FOR_STACK_ARRAY));
    return OK;
}











FILE* file_logging_init(char* name_of_file) {
    FILE* f = fopen(name_of_file, "w");
    if (!f) {
        printf("Can't open file for logging!\n");
        return NULL;
    }
    return f;
}

void end_of_program() {
    fclose(log_file);
}
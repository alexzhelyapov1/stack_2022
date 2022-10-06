#include "stack.h"

FILE* log_file = file_logging_init("log_file.txt");


int stack_init(struct Stack* stack, int size_of_element) {
    assert(stack);

    stack->start_canary_of_stack_struct = CANARY_FOR_STACK;
    stack->end_canary_of_stack_struct = CANARY_FOR_STACK;
    stack->num_of_elements = 0;
    stack->max_num_of_elements = START_NUM_OF_ELEMENTS;
    stack->size_of_element = size_of_element;
    stack->offset = sizeof(CANARY_FOR_STACK_ARRAY);
    stack->size_of_canary = sizeof(CANARY_FOR_STACK_ARRAY);
    stack->size_of_allocated_mem = stack->max_num_of_elements * stack->size_of_element + 2 * stack->size_of_canary;
    stack->stack_pointer = (void*)malloc(stack->size_of_allocated_mem);
    if (!stack->stack_pointer) {
        DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! Can't allocate memory in func Init Stack\n");)
        return CANT_ALLOCATE_MEMORY_INIT_STACK;
    }
    if (install_canaries(stack)) {
        DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! Can't install memory in func Init Stack\n");)
        return CANT_INSTALL_CANARIES;
    }
    stack->hash = hash(stack->stack_pointer, stack->size_of_allocated_mem);

    return OK;
}

int stack_push(struct Stack* stack, void* element) {
    assert(stack);
    DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "Pushing element %d\n", *((int*)element));)

    if (stack->num_of_elements == stack->max_num_of_elements) {
        stack->size_of_allocated_mem += stack->max_num_of_elements * stack->size_of_element * (STACK_MULTIPLY_CONST - 1);
        stack->max_num_of_elements *= STACK_MULTIPLY_CONST;
        stack->stack_pointer = (void*)realloc(stack->stack_pointer, stack->size_of_allocated_mem);
        DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "\nStack array reallocated. New params: \n"
                      "max number of elements = %d\nsize of allocated memory = %d\nsize of 2 canaries = %d\n\n", \
                       stack->max_num_of_elements, stack->size_of_allocated_mem, 2 * stack->size_of_canary);)
        if (!stack->stack_pointer) {
            DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! Can't allocate memory in func push\n");)
            return CANT_ALLOCATE_MEMORY_PUSH;
        }
        if (install_canaries(stack)) {
            DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! Can't install canaries in func push\n");)
            return CANT_INSTALL_CANARIES;
        }
    }

    memcpy(stack->stack_pointer + stack->offset, element, stack->size_of_element);
    stack->num_of_elements += 1;
    stack->offset += stack->size_of_element;
    stack->hash = hash(stack->stack_pointer, stack->size_of_allocated_mem);
    DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "After pushing ");)
    DEBUG_MESSAGE(stack_print_file(stack, log_file));
    return OK;
}

int stack_pop(struct Stack* stack, void* return_element) {
    assert(stack);

    if (stack_isEmpty(stack)) {
        DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! Nothing to pop\n");)
        return NO_ELEMENTS_TO_POP;
    }
    stack->num_of_elements -= 1;
    stack->offset -= stack->size_of_element;
    memcpy(return_element, stack->stack_pointer + stack->offset, stack->size_of_element);

    if (stack->num_of_elements * STACK_DIVIDE_TRIGGER <= stack->max_num_of_elements) {
        stack->max_num_of_elements /= STACK_DIVIDE_CONST;
        stack->size_of_allocated_mem -= stack->max_num_of_elements * stack->size_of_element;
        DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "\nStack array reallocated. New params: \nmax number of elements = %d\n"
                                  "size of allocated memory = %d\nsize of 2 canaries = %d\n\n", stack->max_num_of_elements,
                                   stack->size_of_allocated_mem, 2 * stack->size_of_canary);)
        stack->stack_pointer = (void*)realloc(stack->stack_pointer, stack->size_of_allocated_mem);
        if (!stack->stack_pointer) {
            DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! CANT_ALLOCATE_MEMORY_POP\n");)
            return CANT_ALLOCATE_MEMORY_POP;
        }
        if (install_canaries(stack)) {
            DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! CANT_INSTALL_CANARIES in pop\n");)
            return CANT_INSTALL_CANARIES;
        }
    }
    stack->hash = hash(stack->stack_pointer, stack->size_of_allocated_mem);
    DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "Popping elemnt %d\n", *((int*)return_element));)
    DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "After popping ");)
    DEBUG_MESSAGE(stack_print_file(stack, log_file);)
    return OK;
}

int stack_print(struct Stack* stack) {
    assert(stack);

    printf("Stack: ");
    for (int i = stack->offset - stack->size_of_element; i >= stack->size_of_canary; i -= stack->size_of_element) {
        printf(KEY_FOR_PRINTF, *(TYPE_OF_ELEMENT*)(stack->stack_pointer + i));
        printf(" ");
    }
    if (stack_isEmpty(stack))
        printf("*stack is empty*");
    printf("\n");
    return OK;
}

int stack_print_file(struct Stack* stack, FILE* file) {
    assert(stack);

    fprintf(file, "Stack: ");
    for (int i = stack->offset - stack->size_of_element; i >= stack->size_of_canary; i -= stack->size_of_element) {
        fprintf(file, KEY_FOR_PRINTF, *(TYPE_OF_ELEMENT*)(stack->stack_pointer + i));
        fprintf(file, " ");
    }
    if (stack_isEmpty(stack))
        fprintf(file, "*stack is empty*");
    fprintf(file, "\n");
    return OK;
}

int stack_top(struct Stack* stack, void* return_element) {
    assert(stack);

    if (stack_isEmpty(stack)) {
        DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! NO_ELEMENTS_TO_TOP\n");)
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

int stack_destructor(struct Stack* stack) {
    free (stack->stack_pointer);
    return OK;
}

int stack_valid (struct Stack* stack) {
    assert(stack);

    if (stack->start_canary_of_stack_struct != CANARY_FOR_STACK || \
        stack->end_canary_of_stack_struct != CANARY_FOR_STACK) {
            DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! INVALID_CANARY_OF_STACK in validation\n");)
            return INVALID_CANARY_OF_STACK;
            }
    if (strcmp((char*)stack->stack_pointer, CANARY_FOR_STACK_ARRAY) || \
        strcmp((char*)(stack->stack_pointer + stack->max_num_of_elements * stack->size_of_element + stack->size_of_canary), \
                                                                                                     CANARY_FOR_STACK_ARRAY)) {
            DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "Comparing first canary \"%s\", \"%s\".\n", 
                                     (char*)stack->stack_pointer, CANARY_FOR_STACK_ARRAY);)
            DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "Comparing second canary \"%s\", \"%s\".\n", 
                                (char*)(stack->stack_pointer + stack->max_num_of_elements * stack->size_of_element +
                                stack->size_of_canary), CANARY_FOR_STACK_ARRAY);)
            DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! INVALID_CANARY_OF_STACK_ARRAY in validation\n");)
            return INVALID_CANARY_OF_STACK_ARRAY;
    }
    if (hash(stack->stack_pointer, stack->size_of_allocated_mem) != stack->hash) {
        DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "Comparing hash. Hash expected = %lu, hash received = %lu\n", 
                                  hash(stack->stack_pointer, stack->size_of_allocated_mem), stack->hash);)
        DEBUG_MESSAGE(debug_print(DEBUG_ERRORS, "ERROR! INVALID_HASH_OF_STACK in validation\n");)
        return INVALID_HASH_OF_STACK;
    }
    return OK;
}

int install_canaries(struct Stack* stack) {
    assert(stack);

    memcpy(stack->stack_pointer, CANARY_FOR_STACK_ARRAY, sizeof(CANARY_FOR_STACK_ARRAY));
    memcpy(stack->stack_pointer + stack->max_num_of_elements * stack->size_of_element + stack->size_of_canary, \
        CANARY_FOR_STACK_ARRAY, sizeof(CANARY_FOR_STACK_ARRAY));
    return OK;
}

FILE* file_logging_init(const char* name_of_file) {
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

unsigned long hash(void *data, int len_of_mem) {
    assert(data);

    int c = 0;
    int i = 0;
    unsigned long hash = 5381;

    while (i < len_of_mem) {
        c = *((char*)data + i);
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        i++;
    }
    DEBUG_MESSAGE(debug_print(DEBUG_MAIN_STEPS, "Hash calculated = %lu\n", hash);)
    return hash;
}

void print_name_of_err (int er) {
    FILE* temp = log_file;
    log_file = stdout;
    switch (er) {
    case OK:
        fprintf (log_file, "No errors. Stack is OK\n");
        break;
    case CANT_ALLOCATE_MEMORY_INIT_STACK:
        fprintf (log_file, "Error is \"%s\"\n", "CANT_ALLOCATE_MEMORY_INIT_STACK");
        break;
    case CANT_ALLOCATE_MEMORY_PUSH:
        fprintf (log_file, "Error is \"%s\"\n", "CANT_ALLOCATE_MEMORY_PUSH");
        break;
    case NO_ELEMENTS_TO_POP:
        fprintf (log_file, "Error is \"%s\"\n", "NO_ELEMENTS_TO_POP");
        break;
    case CANT_ALLOCATE_MEMORY_POP:
        fprintf (log_file, "Error is \"%s\"\n", "CANT_ALLOCATE_MEMORY_POP");
        break;
    case NO_ELEMENTS_TO_TOP:
        fprintf (log_file, "Error is \"%s\"\n", "NO_ELEMENTS_TO_TOP");
        break;
    case INVALID_CANARY_OF_STACK:
        fprintf (log_file, "Error is \"%s\"\n", "INVALID_CANARY_OF_STACK");
        break;
    case INVALID_CANARY_OF_STACK_ARRAY:
        fprintf (log_file, "Error is \"%s\"\n", "INVALID_CANARY_OF_STACK_ARRAY");
        break;
    case CANT_INSTALL_CANARIES:
        fprintf (log_file, "Error is \"%s\"\n", "CANT_INSTALL_CANARIES");
        break;
    case INVALID_HASH_OF_STACK:
        fprintf (log_file, "Error is \"%s\"\n", "INVALID_HASH_OF_STACK");
        break;
    default:
        fprintf (log_file, "Undefined error\n");
    }
    log_file = temp;
}

void debug_print(enum debug_level level, const char* format, ...) {
    va_list args;
    va_start(args, format);
    if (level <= DEBUG_LEVEL) {
        vfprintf(log_file, format, args);
    }
    va_end(args);
}
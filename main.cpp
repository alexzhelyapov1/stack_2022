#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Delete after test
int stack_valid(struct Stack* stack);

int main() {
    struct Stack stack;

    stack_init(&stack, sizeof(int));
    int a = 5;
    stack_push(&stack,(void*)&a);
    stack_push(&stack,(void*)&(a = 4));
    stack_push(&stack,(void*)&(a = 3));
    stack_print(&stack);
    stack_pop(&stack, &a);
    stack_pop(&stack, &a);
    stack_pop(&stack, &a);
    printf("Error = %d\n", stack_pop(&stack, &a));
    stack_print(&stack);
    printf("Valid = %d\n", stack_valid(&stack));
    end_of_program();
    return 0;
}
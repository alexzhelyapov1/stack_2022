#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main () {
    struct Stack stack;

    stack_init (&stack, sizeof(int));
    int a = 5;
    stack_push (&stack, (void*)&a);
    a = 4;
    stack_push (&stack, (void*)&a);
    stack_print (&stack);
    return 0;
}
#include "stack.h"

int test_1();


int test_main() {
    test_1();
}


int test_1() {
    struct Stack stack;
    int er = 0;

    int a = 0;
    if ((er = stack_init(&stack, sizeof(int)))) print_name_of_err(er);
    for (int i = 0; i < 25; i++) {
        stack_push(&stack,(void*)&(i));
    }
    er += stack_print(&stack);
    for (int i = 0; i < 26; i++) {
        // stack_print(&stack);
        if (er = stack_pop(&stack, &a)) print_name_of_err(er);
    }
    er += stack_print(&stack);
    printf("destructing stack\n");
    printf("Result of test is = %d, sum of errors = %d\n", stack_valid(&stack), er);
    stack_destructor(&stack);
    // er += printf("Error = %d\n", stack_pop(&stack, &a));
    // er += stack_print(&stack);
    // er += printf("Valid = %d\n", stack_valid(&stack));
    end_of_program();
}
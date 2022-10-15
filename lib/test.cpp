#include "stack.h"

int test_1();

int test_main() {
    test_1();
}

int test_1() {
    struct Stack stack;
    int er = 0;
    int a = 0;
    
    if (er = stack_init(&stack, sizeof(int))) print_name_of_err(er);

    for (int i = 0; i < 25; i++) {
        if (er = stack_push(&stack,(void*)&(i))) print_name_of_err(er);
    }

    if (er = stack_print_file(&stack, stdout)) print_name_of_err(er);

    for (int i = 0; i < 26; i++) {
        if (er = stack_pop(&stack, &a)) print_name_of_err(er);
    }

    if (er = stack_print_file(&stack, stdout)) print_name_of_err(er);

    printf("Destructing stack\n");

    if (er = stack_destructor(&stack)) print_name_of_err(er);

    end_of_program();
}
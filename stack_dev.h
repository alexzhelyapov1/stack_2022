#ifndef STACK_DEV_H
#define STACK_DEV_H

#define CANARY_FOR_STACK 0xDEDBAD
#define CANARY_FOR_STACK_ARRAY "This is secret message!"


// Debug settings
//-----------------------------------------------------------------------------
#define dpf(code) code          // Debug print to file
#define dpfa(code)          // Debug print to file all steps
//-----------------------------------------------------------------------------

FILE* file_logging_init(const char* name_of_file);
int stack_valid(struct Stack* stack);
int install_canaries(struct Stack* stack);
void print_name_of_err (int er);
unsigned long hash(void *data, int len_of_mem);

#endif
#include "stack.h"
#ifndef _HELPERS_H_
#define _HELPERS_H_

int stack_fprint(FILE * const restrict stream, Stack * const restrict stack);

int stack_revers(Stack * const restrict stack);

int stack_find_and_delete_max_elem(Stack * const restrict stack, const void * const restrict ptr, size_t *byte_count, int (*compar)(const void * const restrict, const size_t, const void * const restrict, const size_t));

int stack_selection_sort(Stack * const restrict stack, int (*compar)(const void * const restrict, const size_t, const void * const restrict, const size_t));

int stack_error_code_fprint(FILE * const restrict stream, const errno_t code);

#endif
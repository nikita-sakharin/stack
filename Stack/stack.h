#ifndef _STACK_H_
#define _STACK_H_

typedef int errno_t;

#define ERROR_SUCCESS                 (0)
#define ERROR_MEMORY_ALLOCATION       (1 << 0)
#define ERROR_NULL_POINTER            (1 << 1)
#define ERROR_POINTER_TO_NULL_POINTER (1 << 2)
#define ERROR_NULL_SIZE               (1 << 3)
#define ERROR_EMPTY_STACK             (1 << 4)
#define ERROR_CORRUPTED_DATA          (1 << 5)

typedef struct stacknode StackNode;

typedef struct stack Stack;

errno_t stack_create(Stack ** const restrict stack_head);

errno_t stack_destroy(Stack ** const restrict stack_head);

errno_t stack_is_empty(const Stack *const restrict stack_head, bool *const result);

errno_t stack_elem_count(const Stack * const restrict stack_head, size_t * const restrict elem_count);

errno_t stack_push(Stack * const restrict stack_head, const void *const restrict ptr, const size_t byte_count);

errno_t stack_top(const Stack *const restrict stack_head, void *const restrict ptr, const size_t byte_count);

errno_t stack_top_size(const Stack * const restrict stack_head, size_t * const restrict byte_count);

errno_t stack_element_max_size(const Stack * const restrict stack_head, size_t * const restrict max);

errno_t stack_pop(Stack *const restrict stack_head);

#endif
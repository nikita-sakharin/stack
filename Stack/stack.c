#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "stack.h"

struct stacknode
{
	void *data_ptr;
	size_t size;
	StackNode *under;
};

struct stack
{
	StackNode *head;
};

errno_t stack_create(Stack ** const restrict stack_head)
{
	if (stack_head == (Stack **) NULL)
	{
		return ERROR_NULL_POINTER;
	}

	*stack_head = (Stack *) calloc(1, sizeof(Stack));

	if (*stack_head == (Stack *) NULL)
	{
		return ERROR_MEMORY_ALLOCATION;
	}

	(*stack_head)->head = (StackNode *) NULL;

	return ERROR_SUCCESS;
}

errno_t stack_destroy(Stack ** const restrict stack_head)
{
	if (stack_head == (Stack **) NULL)
	{
		return ERROR_NULL_POINTER;
	}

	if (*stack_head == (Stack *) NULL)
	{
		return ERROR_POINTER_TO_NULL_POINTER;
	}

	register StackNode *temp = (*stack_head)->head;
	while (temp != (StackNode *) NULL)
	{
		StackNode *local_temp = temp->under;

		if (temp->data_ptr != NULL)
		{
			free(temp->data_ptr);
		}

		free(temp);

		temp = local_temp;
	}

	free(*stack_head);

	*stack_head = (Stack *) NULL;

	return ERROR_SUCCESS;
}

errno_t stack_is_empty(const Stack * const restrict stack_head, bool * const restrict result)
{
	if (stack_head == (Stack *) NULL || result == (bool *) NULL)
	{
		return ERROR_NULL_POINTER;
	}

	*result = (bool) (stack_head->head == (StackNode *) NULL);

	return ERROR_SUCCESS;
}

errno_t stack_elem_count(const Stack * const restrict stack_head, size_t * const restrict elem_count)
{
	if (stack_head == (Stack *) NULL || elem_count == (size_t *) NULL)
	{
		return ERROR_NULL_POINTER;
	}

	register StackNode *temp = stack_head->head;
	register size_t count = (size_t) 0u;
	while (temp != (StackNode *) NULL)
	{
		temp = temp->under;
		++count;
	}

	*elem_count = count;

	return ERROR_SUCCESS;
}

errno_t stack_push(Stack * const restrict stack_head, const void *const restrict ptr, const size_t byte_count)
{
	if (stack_head == (Stack *) NULL || ptr == NULL)
	{
		return ERROR_NULL_POINTER;
	}

	if (byte_count == (size_t) 0u)
	{
		return ERROR_NULL_SIZE;
	}

	StackNode *stack_node = (StackNode *) calloc((size_t) 1u, sizeof(StackNode));
	if (stack_node == (StackNode *) NULL)
	{
		return ERROR_MEMORY_ALLOCATION;
	}

	stack_node->data_ptr = calloc(byte_count, (size_t) 1u);
	if (stack_node->data_ptr == NULL)
	{
		free((void *) stack_node);
		return ERROR_MEMORY_ALLOCATION;
	}

	for (register size_t i = (size_t) 0u; i < byte_count; ++i)
	{
		((unsigned char *) stack_node->data_ptr)[i] = ((unsigned char *) ptr)[i];
	}

	stack_node->size = byte_count;

	stack_node->under = stack_head->head;

	stack_head->head = stack_node;

	return ERROR_SUCCESS;
}

errno_t stack_top(const Stack *const restrict stack_head, void *const restrict ptr, const size_t byte_count)
{
	if (stack_head == (Stack *) NULL || ptr == NULL)
	{
		return ERROR_NULL_POINTER;
	}

	if (stack_head->head == (StackNode *) NULL)
	{
		return ERROR_EMPTY_STACK;
	}

	if (stack_head->head->data_ptr == NULL || stack_head->head->size == (size_t) 0u)
	{
		return ERROR_CORRUPTED_DATA;
	}

	if (byte_count == (size_t) 0u)
	{
		return ERROR_NULL_SIZE;
	}

	for (register size_t i = (size_t) 0u; i < byte_count && i < stack_head->head->size; ++i)
	{
		((unsigned char *) ptr)[i] = ((unsigned char *) stack_head->head->data_ptr)[i];
	}

	return ERROR_SUCCESS;
}

errno_t stack_top_size(const Stack * const restrict stack_head, size_t * const restrict byte_count)
{
	if (byte_count == (size_t *) NULL)
	{
		return ERROR_NULL_POINTER;
	}

	if (stack_head == (Stack *) NULL)
	{
		*byte_count = (size_t) 0u;
		return ERROR_NULL_POINTER;
	}

	if (stack_head->head == (StackNode *) NULL)
	{
		*byte_count = (size_t) 0u;
		return ERROR_EMPTY_STACK;
	}

	if (stack_head->head->data_ptr == NULL || stack_head->head->size == (size_t) 0u)
	{
		return ERROR_CORRUPTED_DATA;
	}

	*byte_count = stack_head->head->size;

	return ERROR_SUCCESS;
}

errno_t stack_element_max_size(const Stack * const restrict stack_head, size_t * const restrict max)
{
	if (stack_head == (Stack *) NULL || max == (size_t *) NULL)
	{
		return ERROR_NULL_POINTER;
	}

	if (stack_head->head == (StackNode *) NULL)
	{
		return ERROR_EMPTY_STACK;
	}

	register StackNode *temp = stack_head->head;
	register size_t max_size = (size_t) 0u;
	while (temp != (StackNode *) NULL)
	{
		if (temp->data_ptr == NULL || temp->size == (size_t) 0u)
		{
			*max = (size_t) 0u;
			return ERROR_CORRUPTED_DATA;
		}

		if (temp->size > max_size)
		{
			max_size = temp->size;
		}

		temp = temp->under;
	}

	*max = max_size;

	return ERROR_SUCCESS;
}

errno_t stack_pop(Stack *const restrict stack_head)
{
	if (stack_head == (Stack *) NULL)
	{
		return ERROR_NULL_POINTER;
	}

	if (stack_head->head == (StackNode *) NULL)
	{
		return ERROR_EMPTY_STACK;
	}

	register StackNode *temp = stack_head->head;
	stack_head->head = temp->under;
	if (temp->data_ptr != NULL)
	{
		free(temp->data_ptr);
	}

	free(temp);

	return ERROR_SUCCESS;
}
#include <stdio.h>
#include <stdbool.h>
#include "helpers.h"

int stack_fprint(FILE * const restrict stream, Stack * const restrict stack)
{
	if (stream == (FILE *) NULL || stack == (Stack *) NULL)
	{
		return EOF;
	}

	errno_t local_errors = ERROR_SUCCESS;

	bool is_empty = false;

	local_errors = stack_is_empty(stack, &is_empty);
	if (local_errors)
	{
		return EOF;
	}
	if (is_empty)
	{
		fputs("Stack is empty.\n", stream);
		return 0;
	}

	Stack *temp = (Stack *) NULL;

	local_errors = stack_create(&temp);
	if (local_errors)
	{
		return EOF;
	}

	int count = 0;

	while(stack_is_empty(stack, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(stack, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(stack, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		for (size_t i = (size_t) 0u; i < curr_size; ++i, ++count)
		{
			local_errors = fprintf(stream, "%02hhx", buffer[i]);
			if (local_errors < 0)
			{
				stack_destroy(&temp);
				return EOF;
			}

			if (i == curr_size - (size_t) 1u)
			{
				local_errors = fputc('\n', stream);
			}
			else
			{
				local_errors = fputc(' ', stream);
			}
			if (local_errors == EOF)
			{
				stack_destroy(&temp);
				return EOF;
			}
		}
		local_errors = fputc('\n', stream);
		if (local_errors == EOF)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_pop(stack);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_push(temp, buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}
	}

	while(stack_is_empty(temp, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(temp, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(temp, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_pop(temp);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_push(stack, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}
	}

	local_errors = stack_destroy(&temp);
	if (local_errors)
	{
		return EOF;
	}

	return count;
}

int stack_revers(Stack * const restrict stack)
{
	if (stack == (Stack *) NULL)
	{
		return EOF;
	}

	errno_t local_errors = ERROR_SUCCESS;

	Stack *temp = (Stack *) NULL;

	local_errors = stack_create(&temp);
	if (local_errors)
	{
		return EOF;
	}

	bool is_empty = false;

	while (stack_is_empty(stack, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(stack, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(stack, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_pop(stack);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_push(temp, buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}
	}

	Stack *local_temp = (Stack *) NULL;

	local_errors = stack_create(&local_temp);
	if (local_errors)
	{
		stack_destroy(&temp);
		return EOF;
	}

	while (stack_is_empty(temp, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(temp, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(temp, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}

		local_errors = stack_pop(temp);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}

		local_errors = stack_push(local_temp, buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}
	}

	while (stack_is_empty(local_temp, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(local_temp, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(local_temp, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}

		local_errors = stack_pop(local_temp);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}

		local_errors = stack_push(stack, buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			stack_destroy(&local_temp);
			return EOF;
		}
	}

	local_errors = stack_destroy(&temp);
	if (local_errors)
	{
		stack_destroy(&local_temp);
		return EOF;
	}

	local_errors = stack_destroy(&local_temp);
	if (local_errors)
	{
		return EOF;
	}

	return 0;
}

int stack_find_and_delete_max_elem(Stack * const restrict stack, const void * const restrict ptr, size_t *byte_count, int (*compar)(const void * const restrict, const size_t, const void * const restrict, const size_t))
{
	if (stack == (Stack *) NULL || ptr == NULL || byte_count == (size_t *) NULL || compar == (int (*)(const void *, size_t, const void *, size_t)) NULL)
	{
		return EOF;
	}

	errno_t local_errors = ERROR_SUCCESS;

	bool is_empty = false;

	local_errors = stack_is_empty(stack, &is_empty);
	if (local_errors)
	{
		return EOF;
	}
	if (is_empty)
	{
		return EOF;
	}

	Stack *temp = (Stack *) NULL;

	local_errors = stack_create(&temp);
	if (local_errors)
	{
		return EOF;
	}

	size_t max_elem_size = (size_t) 0u;

	local_errors = stack_element_max_size(stack, &max_elem_size);
	if (local_errors)
	{
		stack_destroy(&temp);
		return EOF;
	}

	unsigned char curr_max_elem[max_elem_size];

	size_t size_of_curr_max = (size_t) 0u;

	local_errors = stack_top_size(stack, &size_of_curr_max);
	if (local_errors)
	{
		stack_destroy(&temp);
		return EOF;
	}

	local_errors = stack_top(stack, (void *) curr_max_elem, size_of_curr_max);
	if (local_errors)
	{
		stack_destroy(&temp);
		return EOF;
	}

	while (stack_is_empty(stack, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(stack, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(stack, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_pop(stack);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_push(temp, buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		int compar_result = compar(buffer, curr_size, curr_max_elem, size_of_curr_max);
		if (compar_result > 1 || compar_result < -1)
		{
			stack_destroy(&temp);
			return EOF;
		}

		if (compar_result == 1)
		{
			for (size_t i = (size_t) 0u; i < curr_size; ++i)
			{
				curr_max_elem[i] = buffer[i];
			}
			size_of_curr_max = curr_size;
		}
	}

	bool is_find = false;

	while (stack_is_empty(temp, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(temp, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(temp, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_pop(temp);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		if (!is_find)
		{
			int compar_result = compar(buffer, curr_size, curr_max_elem, size_of_curr_max);
			if (compar_result > 1 || compar_result < -1)
			{
				stack_destroy(&temp);
				return EOF;
			}

			if (compar_result >= 0)
			{
				is_find = true;
				continue;
			}
		}

		local_errors = stack_push(stack, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}
	}

	for (size_t i = (size_t) 0u; i < size_of_curr_max; ++i)
	{
		((unsigned char *) ptr)[i] = curr_max_elem[i];
	}

	*byte_count = size_of_curr_max;

	local_errors = stack_destroy(&temp);
	if (local_errors)
	{
		return EOF;
	}

	return 0;
}


int stack_selection_sort(Stack * const restrict stack, int (*compar)(const void * const restrict, const size_t, const void * const restrict, const size_t))
{
	if (stack == (Stack *) NULL || compar == (int (*)(const void *, size_t, const void *, size_t)) NULL)
	{
		return EOF;
	}

	errno_t local_errors = ERROR_SUCCESS;

	bool is_empty = false;

	local_errors = stack_is_empty(stack, &is_empty);
	if (local_errors)
	{
		return EOF;
	}
	if (is_empty)
	{
		return 0;
	}

	Stack *temp = (Stack *) NULL;

	local_errors = stack_create(&temp);
	if (local_errors)
	{
		return EOF;
	}

	while (stack_is_empty(stack, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_element_max_size(stack, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_find_and_delete_max_elem(stack, (void *) buffer, &curr_size, compar);
		if (local_errors == EOF)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_push(temp, buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}
	}

	while(stack_is_empty(temp, &is_empty), !is_empty)
	{
		size_t curr_size = (size_t) 0u;

		local_errors = stack_top_size(temp, &curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		unsigned char buffer[curr_size];

		local_errors = stack_top(temp, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_pop(temp);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}

		local_errors = stack_push(stack, (void *) buffer, curr_size);
		if (local_errors)
		{
			stack_destroy(&temp);
			return EOF;
		}
	}

	stack_destroy(&temp);
	if (local_errors)
	{
		return EOF;
	}

	return 0;
}

int stack_error_code_fprint(FILE * const restrict stream, const errno_t code)
{
	if (stream == (FILE *) NULL)
	{
		return EOF;
	}

	if (code == ERROR_SUCCESS)
	{
		fputs("No errors.\n", stream);
		return 0;
	}

	int count = 0;

	if (code & ERROR_MEMORY_ALLOCATION)
	{
		fputs("Error memory allocation.\n", stream);
		++count;
	}
	if (code & ERROR_NULL_POINTER)
	{
		fputs("Error null pointer.\n", stream);
		++count;
	}
	if (code & ERROR_POINTER_TO_NULL_POINTER)
	{
		fputs("Error pointer to null pointer.\n", stream);
		++count;
	}
	if (code & ERROR_NULL_SIZE)
	{
		fputs("Error null size.\n", stream);
		++count;
	}
	if (code & ERROR_EMPTY_STACK)
	{
		fputs("Error empty stack.\n", stream);
		++count;
	}
	if (code & ERROR_CORRUPTED_DATA)
	{
		fputs("Error corrupted data.\n", stream);
		++count;
	}

	return count;
}
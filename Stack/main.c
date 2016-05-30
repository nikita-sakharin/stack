#include <stdio.h>
#include <stdbool.h>
#include "stack.h"
#include "helpers.h"

int cmp(const void * const restrict value1, const size_t size1, const void * const restrict value2, const size_t size2);

int main(void)
{
	errno_t local_errors = ERROR_SUCCESS;

	Stack *stack = (Stack *) NULL;
	local_errors = stack_create(&stack);
	if (local_errors)
	{
		stack_error_code_fprint(stdout, local_errors);
		return 1;
	}

	int code = 0;
	while (code != EOF && !ferror(stdin) && !feof(stdin))
	{
		puts("Press E   - for check is stack empty.");
		puts("Press O   - for pop.");
		puts("Press P   - for push.");
		puts("Press T   - for top.");
		puts("Press f   - for find and delete max element.");
		puts("Press p   - for print stack contain.");
		puts("Press r   - for revers stack.");
		puts("Press s   - for sort stack contain.");
		puts("Press EOF - for end.");
		putchar('\n');

		code = getchar();
		if (code == '\n')
		{
			continue;
		}
		getchar();
		putchar('\n');

		switch (code)
		{
		case 'E':
		{
			bool is_empty = false;
			local_errors = stack_is_empty(stack, &is_empty);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}
			if (is_empty)
			{
				puts("Stack is empty.\n");
			}
			else
			{
				puts("Stack is not empty.\n");
			}
			break;
		}
		case 'O':
		{
			bool is_empty = false;
			local_errors = stack_is_empty(stack, &is_empty);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}

			if (is_empty)
			{
				puts("Stack is empty.\n");
				continue;
			}

			local_errors = stack_pop(stack);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}

			puts("Pop opperation success.\n");

			break;
		}
		case 'P':
		{
			puts("Enter size of data:\n");
			size_t count = (size_t) 0u;
			local_errors = scanf("%zu", &count);
			if (local_errors == EOF)
			{
				puts("Error reading from stdin");
				continue;
			}
			if (!count)
			{
				puts("0 data size.\n");
				continue;
			}
			putchar('\n');

			puts("Enter data in hex form:\n");
			unsigned char buffer[count];
			for (size_t i = (size_t) 0u; i < count; ++i)
			{
				local_errors = scanf("%hhx", buffer + i);
				if (local_errors == EOF)
				{
					puts("Error reading from stdin");
					continue;
				}
			}
			getchar();
			putchar('\n');

			local_errors = stack_push(stack, buffer, count);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}

			puts("Push opperation success.\n");

			break;
		}
		case 'T':
		{
			bool is_empty = false;
			local_errors = stack_is_empty(stack, &is_empty);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}
			if (is_empty)
			{
				puts("Stack is empty.\n");
				continue;
			}

			size_t count = (size_t) 0u;
			local_errors = stack_top_size(stack, &count);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}

			unsigned char buffer[count];
			local_errors = stack_top(stack, buffer, count);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}

			puts("The contain of stack top is:\n");

			for (size_t i = (size_t) 0u; i < count; ++i)
			{
				printf("%hhx", buffer[i]);
				if (i == count - (size_t) 1u)
				{
					local_errors = putchar('\n');
				}
				else
				{
					local_errors = putchar(' ');
				}
			}
			putchar('\n');

			puts("Top opperation success.\n");

			break;
		}
		case 'f':
		{
			bool is_empty = false;
			local_errors = stack_is_empty(stack, &is_empty);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}
			if (is_empty)
			{
				puts("Stack is empty.\n");
				continue;
			}

			size_t max_size = (size_t) 0u;
			local_errors = stack_element_max_size(stack, &max_size);
			if (local_errors)
			{
				stack_error_code_fprint(stdout, local_errors);
				continue;
			}

			unsigned char buffer[max_size];

			local_errors = stack_find_and_delete_max_elem(stack, buffer, &max_size, cmp);
			if (local_errors == EOF)
			{
				puts("Finding error.\n");
				continue;
			}

			puts("The contain of stack max element.\n");
			for (size_t i = (size_t) 0u; i < max_size; ++i)
			{
				printf("%hhx", buffer[i]);

				if (i == max_size - (size_t) 1u)
				{
					putchar('\n');
				}
				else
				{
					putchar(' ');
				}
			}
			putchar('\n');

			puts("Find and delete opperation success.\n");

			break;
		}
		case 'p':
		{
			puts("The contain of stack:\n");
			local_errors = stack_fprint(stdout, stack);
			if (local_errors == EOF)
			{
				puts("Error stack print.\n");
				continue;
			}
			printf("Number of printed byte of stack data is: %d.\n\n", local_errors);
			break;
		}
		case 'r':
		{
			local_errors = stack_revers(stack);
			if (local_errors == EOF)
			{
				puts("Error stack revers.\n");
				continue;
			}
			puts("Revers opperation success.\n");
			break;
		}
		case 's':
		{
			local_errors = stack_selection_sort(stack, cmp);
			if (local_errors == EOF)
			{
				puts("Error stack sort.\n");
				continue;
			}
			puts("Sorting opperation success.\n");
			break;
		}
		case EOF:
		{
			puts("Finishing.");
			break;
		}
		default:
		{
			puts("Wrong opperation code.");
			break;
		}
		}
	}

	local_errors = stack_destroy(&stack);
	if (local_errors)
	{
		stack_error_code_fprint(stdout, local_errors);
		return 1;
	}

	return 0;
}

int cmp(const void * const restrict value1, const size_t size1, const void * const restrict value2, const size_t size2)
{
	if (value1 == NULL || size1 == (size_t) 0u || value2 == NULL || size2 == (size_t) 0u)
	{
		return 2;
	}

	size_t max = size1 >= size2 ? size1 : size2;
	unsigned char *ptr1 = (unsigned char *) value1, *ptr2 = (unsigned char *) value2;

	for (size_t i = max - 1u; i != (size_t) -1; --i)
	{
		if (i >= size1)
		{
			if (ptr2[i] > 0)
			{
				return -1;
			}
		}
		else if (i >= size2)
		{
			if (ptr1[i] > 0)
			{
				return 1;
			}
		}
		else
		{
			if (ptr1[i] > ptr2[i])
			{
				return 1;
			}
			else if (ptr1[i] < ptr2[i])
			{
				return -1;
			}
		}
	}

	return 0;
}
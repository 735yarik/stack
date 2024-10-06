#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int stack_elem_t;

const size_t INIT_CAP = 2;
const unsigned long long STACK_DEFAULT_VALUE = 0xDEDEDEDE;

enum modes
{

    INCREASE,
    DECREASE,

};

struct StackStruct
{

    stack_elem_t *stack = NULL;
    size_t position = 0;
    size_t cap = 0;
    FILE *file = NULL;

};

void Ctor(StackStruct *stack_info, size_t init_cap);

void Dtor(StackStruct *stack_info);

void stack_push(StackStruct *stack_info, stack_elem_t new_elem);

stack_elem_t stack_pop(StackStruct *stack_info);

void stack_fprintf(StackStruct *stack_info);

void stack_printf(StackStruct *stack_info);

void stack_resize(StackStruct *stack_info, int mode);

void stack_realloc(StackStruct *stack_info);

void dump(StackStruct *stack_info, size_t error_code);

void verification(StackStruct *stack_info, size_t *error_code);

void my_assert(StackStruct *stack_info);

int main()
{

    StackStruct stack_info = {};

    int mode = 0;
    stack_elem_t new_elem = 0;

    Ctor(&stack_info, INIT_CAP);

    while(mode != -1)
    {

        printf("\nmode: ");
        scanf("%d", &mode);

        if (mode == 1)          // 1 - push    2 - pop
        {
            printf("pushin ");
            scanf("%d", &new_elem);
            stack_push(&stack_info, new_elem);
            stack_fprintf(&stack_info);
        }

        if (mode == 2)
        {
            printf("poppin %d\n", stack_pop(&stack_info));
        }

    }
    Dtor(&stack_info);
}

void Ctor(StackStruct *stack_info, size_t init_cap)
{

    assert(stack_info != NULL);
    assert(init_cap != 0);

    stack_info->cap = init_cap;
    stack_info->stack = (stack_elem_t *) calloc(init_cap, sizeof(stack_elem_t));
    assert(stack_info->stack != NULL);

    stack_info->position = 0;

    memset(stack_info->stack, STACK_DEFAULT_VALUE, init_cap * sizeof(stack_elem_t));

}

void Dtor(StackStruct *stack_info)
{

    my_assert(stack_info);

    free(stack_info->stack);
    stack_info->stack = NULL;
    stack_info->cap = 0;
    stack_info->position = 0;

}

void stack_push(StackStruct *stack_info, stack_elem_t new_elem)
{

    my_assert(stack_info);

    if (stack_info->position == stack_info->cap)
    {
        stack_resize(stack_info, INCREASE);
    }

    *(stack_info->stack + stack_info->position) = new_elem;
    stack_info->position++;

    my_assert(stack_info);

}

stack_elem_t stack_pop(StackStruct *stack_info)
{

    my_assert(stack_info);

    if (stack_info->position == stack_info->cap / 2 - 3)
    {
        stack_resize(stack_info, DECREASE);
    }

    stack_elem_t popped_elem = stack_info->stack[stack_info->position-1];
    stack_info->stack[stack_info->position] = STACK_DEFAULT_VALUE;
    stack_info->position--;

    my_assert(stack_info);

    return popped_elem;

}

void stack_fprintf(StackStruct *stack_info)
{

    my_assert(stack_info);

    for(size_t c = 0; c < stack_info->position; c++)
    {
        fprintf(stack_info->file, "%d ", stack_info->stack[c]);
    }

    fprintf(stack_info->file, "\n");

    my_assert(stack_info);

}

void stack_printf(StackStruct *stack_info)
{

    my_assert(stack_info);

    for(size_t c = 0; c < stack_info->position; c++)
    {
        printf("%d ", stack_info->stack[c]);
    }

    printf("\n");

    my_assert(stack_info);

}

void stack_resize(StackStruct *stack_info, int mode)
{

    my_assert(stack_info);
    assert(mode == INCREASE || mode == DECREASE);

    if (mode == INCREASE)
    {
        stack_info->cap *= 2;
        stack_realloc(stack_info);
        memset(stack_info->stack + stack_info->cap / 2, STACK_DEFAULT_VALUE, stack_info->cap * sizeof(stack_elem_t) / 2);
    }
    else
    {
        stack_info->cap /= 2;
        stack_realloc(stack_info);
    }

    my_assert(stack_info);

}

void stack_realloc(StackStruct *stack_info)
{

    my_assert(stack_info);

    stack_elem_t *new_stack = (stack_elem_t *) realloc(stack_info->stack, stack_info->cap * sizeof(stack_elem_t));
    assert(new_stack != NULL);

    stack_info->stack = new_stack;

    my_assert(stack_info);

}

void dump(StackStruct *stack_info, size_t error_code)
{

    stack_info->file = fopen("log.txt", "a");
    if (stack_info->file == NULL)
    {
        printf("\nfopen error");
        printf("\ndump: %s  %s\n", __TIME__, __DATE__);
        printf("error code = %d\nstack pointer = %d\nposition = %d\ncapacity = %d\nstack ---> ", error_code, stack_info->position, stack_info->cap);
        stack_printf(stack_info);
    }
    else
    {
        fprintf(stack_info->file, "\ndump: %s  %s\n", __TIME__, __DATE__);
        fprintf(stack_info->file, "error code = %d\nposition = %d\ncapacity = %d\nstack ---> ", error_code, stack_info->position, stack_info->cap);
        stack_fprintf(stack_info);
        assert(fclose(stack_info->file) == 0);
    }
    
}

void verification(StackStruct *stack_info, size_t *error_code)
{

    if (stack_info == NULL)
    {
        *error_code += 100;
    }

    if (stack_info->cap == 0)
    {
        *error_code += 10;
    }

    if (stack_info->stack == 0)
    {
        *error_code += 1;
    }

}

void my_assert(StackStruct *stack_info)
{

    size_t error_code = 0;

    verification(stack_info, &error_code);

    if (error_code != 0)
    {
        dump(stack_info, error_code);
        exit(0);
    }

}
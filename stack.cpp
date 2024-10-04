#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int stack_elem_t;

const size_t INIT_CAP = 2;
const stack_elem_t STACK_DEFAULT_VALUE = 1;

enum modes
{

    INCREASE,
    DECREASE,

};

struct StackStruct
{

    stack_elem_t *stack = NULL;
    size_t elems_num = 0;
    size_t cap = 0;
    FILE *file = NULL;

};

void Ctor(StackStruct *stack_info, size_t init_cap);

void Dtor(StackStruct *stack_info);

void stack_push(StackStruct *stack_info, stack_elem_t new_elem);

stack_elem_t stack_pop(StackStruct *stack_info);

void stack_print(stack_elem_t *stack, size_t elems_num);

void stack_realloc(StackStruct *stack_info, int mode);

void dump(StackStruct *stack_info);

int verification(StackStruct *stack_info);

int main()
{

    StackStruct stack_info = {};

    Ctor(&stack_info, INIT_CAP);

    int mode = 0;
    stack_elem_t new_elem = 0;

    while(mode != -1)
    {

        printf("\nmode: ");
        scanf("%d", &mode);

        if (mode == 1)          // 1 - push    2 - pop
        {
            scanf("%d", &new_elem);
            stack_push(&stack_info, new_elem);
            stack_print(stack_info.stack, stack_info.elems_num);
        }

        if (mode == 2)
        {
            stack_pop(&stack_info);
            stack_print(stack_info.stack, stack_info.elems_num);
        }
        dump(&stack_info);
    }
    Dtor(&stack_info);
}

void Ctor(StackStruct *stack_info, size_t init_cap)
{

    stack_info->cap = init_cap;
    stack_info->stack = (stack_elem_t *) calloc(init_cap, sizeof(stack_elem_t));
    assert(stack_info->stack != NULL);

    stack_info->elems_num = 0;

    memset(stack_info->stack, STACK_DEFAULT_VALUE, init_cap * sizeof(stack_elem_t)); //memset????

    stack_print(stack_info->stack, init_cap); //

}

void Dtor(StackStruct *stack_info)
{

    free(stack_info->stack);
    stack_info->stack = NULL;
    stack_info->cap = 0;
    stack_info->elems_num = 0;

}

void stack_push(StackStruct *stack_info, stack_elem_t new_elem)
{

    if (stack_info->elems_num == stack_info->cap)
    {
        stack_realloc(stack_info, INCREASE);
    }

    *(stack_info->stack + stack_info->elems_num) = new_elem;
    printf("pushin ");
    stack_info->elems_num++;

}

stack_elem_t stack_pop(StackStruct *stack_info)
{

    if (stack_info->elems_num == stack_info->cap / 2 - 3)
    {
        stack_realloc(stack_info, DECREASE);
    }

    printf("poppin %d\n", stack_info->stack[stack_info->elems_num-1]);
    stack_elem_t popped_elem = stack_info->stack[stack_info->elems_num-1];
    stack_info->stack[stack_info->elems_num] = STACK_DEFAULT_VALUE;
    stack_info->elems_num--;

    return popped_elem;

}

void stack_print(stack_elem_t *stack, size_t elems_num)
{

    for(size_t c = 0; c < elems_num; c++)
    {
        printf("%d ", stack[c]);
    }

    printf("\n");

}


void stack_realloc(StackStruct *stack_info, int mode)
{

    stack_elem_t *new_stack = NULL;

    switch(mode)
    {
        case INCREASE:

            stack_info->cap *= 2;
            new_stack = (stack_elem_t *) realloc(stack_info->stack, stack_info->cap * sizeof(stack_elem_t));
            assert(new_stack != NULL);

            stack_info->stack = new_stack;
            break;

        case DECREASE:

            stack_info->cap /= 2;
            new_stack = (stack_elem_t *) realloc(stack_info->stack, stack_info->cap * sizeof(stack_elem_t));
            assert(new_stack != NULL);

            stack_info->stack = new_stack;
            break;
    }

}

void dump(StackStruct *stack_info)
{

    printf("\ndump:\nelems_num = %d\ncapacity = %d\nstack ---> ", stack_info->elems_num, stack_info->cap);
    stack_print(stack_info->stack, stack_info->elems_num);
    
}

int verification(StackStruct *stack_info)
{

    if (stack_info->cap < stack_info->elems_num)
    {
        return 0;
    }

    if (stack_info->cap == 0)
    {
        return 1;
    }

    if (stack_info->stack == 0)
    {
        return 2;
    }

}
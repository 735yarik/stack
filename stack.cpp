#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int stack_elem_t;

const size_t INIT_CAP = 2;
const unsigned long long STACK_DEFAULT_VALUE = 0xDEDEDEDE;
const int L_canary = 0xDEADF00D;
const int R_canary = 0xBADC0FFE;
const int DJB2 = 5381;

enum modes
{

    INCREASE,
    DECREASE,

};

struct StackStruct
{

    int L_CANARY = 0xDEADF00D;

    FILE *file = NULL;

    stack_elem_t *stack = NULL;
    size_t position = 0;
    size_t cap = 0;
    unsigned long stack_hash = DJB2;
    unsigned long struct_hash = DJB2;

    int R_CANARY = 0xBADC0FFE;

};

void Ctor(StackStruct *stack_info, size_t init_cap);

void Dtor(StackStruct *stack_info);

void stack_push(StackStruct *stack_info, stack_elem_t new_elem);

stack_elem_t stack_pop(StackStruct *stack_info);

void stack_fprintf(StackStruct *stack_info);

void stack_printf(StackStruct *stack_info);

void stack_resize(StackStruct *stack_info, int mode);

void dump(StackStruct *stack_info, size_t error_code);

void verification(StackStruct *stack_info, size_t *error_code);

unsigned long stack_hash(StackStruct *stack_info);

unsigned long struct_hash(StackStruct *stack_info);

void stack_assert(StackStruct *stack_info);

int main()
{

    StackStruct stack_info = {};

    int mode = 0;
    stack_elem_t new_elem = 0;

    Ctor(&stack_info, INIT_CAP);

    printf("choose mode: 1 - push    2 - pop    3 - dump");

    while(mode != -1)
    {

        printf("\nmode: ");
        scanf("%d", &mode);

        switch(mode)          // 1 - push    2 - pop    3 - dump    -1 - end program
        {
            case(1):
            {
                printf("pushin ");
                scanf("%d", &new_elem);
                stack_push(&stack_info, new_elem);
                stack_fprintf(&stack_info);

                break;
            }

            case(2):
            {
                if (stack_info.position != 0)
                {
                printf("poppin %d\n", stack_pop(&stack_info));
                }
                else
                {
                    printf("stack has 0 elements\n");
                }

                break;
            }

            case(3):
            {
                dump(&stack_info, 0);

                break;
            }

            default:
            {
                printf("wrong input\n");
            }
        }

        printf("stack ---> ");
        stack_printf(&stack_info);
    }
    Dtor(&stack_info);

}

void Ctor(StackStruct *stack_info, size_t init_cap)
{

    assert(stack_info != NULL);
    assert(init_cap != 0);

    stack_info->cap = init_cap;
    stack_info->position = 0;
    stack_info->stack = (stack_elem_t *) calloc(init_cap + 2, sizeof(stack_elem_t));
    assert(stack_info->stack != NULL);

    stack_info->stack++;

    stack_info->stack[-1] = L_canary;
    stack_info->stack[init_cap] = R_canary;

    memset(stack_info->stack + 1, STACK_DEFAULT_VALUE, (init_cap - 1) * sizeof(stack_elem_t));

}

void Dtor(StackStruct *stack_info)
{

    stack_assert(stack_info);

    free(--stack_info->stack);
    stack_info->stack = NULL;
    stack_info->cap = 0;
    stack_info->position = 0;
    stack_info->file = NULL;
    stack_info->stack_hash = 0;
    stack_info->L_CANARY = 0;
    stack_info->R_CANARY = 0;

}

void stack_push(StackStruct *stack_info, stack_elem_t new_elem)
{

    stack_assert(stack_info);

    if (stack_info->position == stack_info->cap)
    {
        stack_resize(stack_info, INCREASE);
    }

    stack_info->stack[stack_info->position] = new_elem;
    stack_info->position++;

    stack_info->stack_hash = stack_hash(stack_info);

    stack_assert(stack_info);

}

stack_elem_t stack_pop(StackStruct *stack_info)
{

    stack_assert(stack_info);

    if (stack_info->position == stack_info->cap / 2 - 3)
    {
        stack_resize(stack_info, DECREASE);
    }

    stack_elem_t popped_elem = stack_info->stack[stack_info->position - 1];
    stack_info->stack[stack_info->position] = STACK_DEFAULT_VALUE;
    stack_info->position--;

    stack_info->stack_hash = stack_hash(stack_info);

    stack_assert(stack_info);

    return popped_elem;

}

void stack_fprintf(StackStruct *stack_info)
{

    stack_assert(stack_info);

    for (size_t c = 0; c < stack_info->position; c++)
    {
        fprintf(stack_info->file, "%d ", stack_info->stack[c]);
    }

    fprintf(stack_info->file, "\n");

    stack_assert(stack_info);

}

void stack_printf(StackStruct *stack_info)
{

    stack_assert(stack_info);

    for (size_t c = 0; c < stack_info->position; c++)
    {
        printf("%d ", stack_info->stack[c]);
    }

    printf("\n");

    stack_assert(stack_info);

}

void stack_resize(StackStruct *stack_info, int mode)
{

    stack_assert(stack_info);
    assert(mode == INCREASE || mode == DECREASE);

    stack_info->cap = (mode == INCREASE) ? stack_info->cap * 2 : stack_info->cap / 2;


    stack_elem_t *new_stack = (stack_elem_t *) realloc(stack_info->stack - 1, (stack_info->cap + 2) * sizeof(stack_elem_t));
    assert(new_stack != NULL);

    stack_info->stack = new_stack + 1;
    stack_info->stack[stack_info->cap] = R_canary;


    if (mode == INCREASE)
    {
        memset(&stack_info->stack[stack_info->cap / 2 + 1], STACK_DEFAULT_VALUE, (stack_info->cap - 2) * sizeof(stack_elem_t) / 2);
    }


    stack_assert(stack_info);

}

void dump(StackStruct *stack_info, size_t error_code)
{

    stack_info->file = fopen("log.txt", "a");
    
    if (stack_info->file == NULL)
    {
        printf("\nfopen error");
        printf("\ndump: %s  %s\n", __TIME__, __DATE__);
        printf("error code = %d\nstack pointer = %d\nposition = %d\ncapacity = %d\nstack ---> ", error_code, stack_info->stack, stack_info->position, stack_info->cap);
        stack_printf(stack_info);
    }
    else
    {
        fprintf(stack_info->file, "\ndump: %s  %s\n", __TIME__, __DATE__);
        fprintf(stack_info->file, "error code = %d\nposition = %d\ncapacity = %d\nstack ---> ", error_code, stack_info->stack, stack_info->position, stack_info->cap);
        stack_fprintf(stack_info);
        assert(fclose(stack_info->file) == 0);
    }
    
}

void verification(StackStruct *stack_info, size_t *error_code)
{

    if (stack_info == NULL)
    {
        *error_code += 100000000;
    }

    if (stack_info->stack == 0)
    {
        *error_code += 10000000;
    }

    if (stack_info->cap == 0)
    {
        *error_code += 1000000;
    }

    if (stack_info->stack[-1] != L_canary)
    {
        *error_code += 100000;
    }

    if (stack_info->stack[stack_info->cap] != R_canary)
    {
        *error_code += 10000;
    }

    if (stack_info->L_CANARY != L_canary)
    {
        *error_code += 1000;
    }

    if (stack_info->R_CANARY != R_canary)
    {
        *error_code += 100;
    }

    if (stack_info->struct_hash != struct_hash(stack_info))
    {
        *error_code += 10;
    }

    if (stack_info->stack_hash != stack_hash(stack_info))
    {
        *error_code += 1;
    }

}

void stack_assert(StackStruct *stack_info)
{

    size_t error_code = 0;

    verification(stack_info, &error_code);

    if (error_code != 0)
    {
        dump(stack_info, error_code);
        exit(0);
    }

}

unsigned long stack_hash(StackStruct *stack_info)
{

    unsigned long hash = DJB2;

    for (size_t c = 0; c < stack_info->position; c++)
    {
        hash = ((hash << 5) + hash) + stack_info->stack[c];
    }

    return hash;

}

unsigned long struct_hash(StackStruct *stack_info)
{
    
    unsigned long hash = DJB2;

    hash = ((hash << 5) + hash) + stack_info->position;
    hash = ((hash << 5) + hash) + stack_info->cap;
    hash = ((hash << 5) + hash) + stack_info->stack_hash;
    hash = ((hash << 5) + hash) + stack_info->L_CANARY;
    hash = ((hash << 5) + hash) + stack_info->R_CANARY;

}
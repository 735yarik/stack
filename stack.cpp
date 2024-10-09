#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG

#ifdef DEBUG
    #define STACK_ASSERT(stack_info)    size_t error_code = verification(stack_info);\
                                                                                     \
                                        if (error_code != 0)                         \
                                        {                                            \
                                        dump(stack_info, error_code);                \
                                        exit(0);                                     \
                                        }
#else
    #define STACK_ASSERT(stack_info)
#endif


typedef int stack_elem_t;

const size_t INIT_CAP = 2;
const unsigned long long STACK_DEFAULT_VALUE = 0xDEDEDEDE;
const int L_CANARY = 0xDEADF00D;
const int R_CANARY = 222;
const int DJB2 = 5381;

enum modes
{

    INCREASE,
    DECREASE,

};

enum error_code
{

    STACK_INFO_POINTER_ERR = 100000000,
    STACK_POINTER_ERR      = 10000000,
    CAPACITY_VALUE_ERR     = 1000000,
    STACK_L_CANARY_ERR     = 100000,
    STACK_R_CANARY_ERR     = 10000,
    STRUCT_L_CANARY_ERR    = 1000,
    STRUCT_R_CANARY_ERR    = 100,
    STRUCT_HASH_ERR        = 10,
    STACK_HASH_ERR         = 1,

};

struct StackStruct
{

    int l_canary = L_CANARY;

    FILE *file = NULL;

    stack_elem_t *stack = NULL;
    size_t position = 0;
    size_t cap = 0;

    unsigned long stack_hash = DJB2;
    unsigned long struct_hash = DJB2;

    int r_canary = R_CANARY;

};

void Ctor(StackStruct *stack_info, size_t init_cap);

void Dtor(StackStruct *stack_info);

void stack_push(StackStruct *stack_info, stack_elem_t new_elem);

stack_elem_t stack_pop(StackStruct *stack_info);

void stack_printf(StackStruct *stack_info);

void stack_resize(StackStruct *stack_info, int mode);

void dump(StackStruct *stack_info, size_t error_code);

size_t verification(StackStruct *stack_info);

unsigned long stack_hash(StackStruct *stack_info);

unsigned long struct_hash(StackStruct *stack_info);

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
    stack_info->stack = (stack_elem_t *) calloc(init_cap * sizeof(stack_elem_t) + 2 * sizeof(unsigned long long), sizeof(char));
    assert(stack_info->stack != NULL);

    stack_info->stack++;

    stack_info->stack[-1] = L_CANARY;
    stack_info->stack[init_cap] = R_CANARY;

    memset(stack_info->stack + 1, STACK_DEFAULT_VALUE, (init_cap - 1) * sizeof(stack_elem_t));

}

void Dtor(StackStruct *stack_info)
{

    STACK_ASSERT(stack_info);

    free(--stack_info->stack);
    stack_info->stack = NULL;
    stack_info->cap = 0;
    stack_info->position = 0;
    stack_info->file = NULL;
    stack_info->stack_hash = 0;
    stack_info->l_canary = 0;
    stack_info->r_canary = 0;

}

void stack_push(StackStruct *stack_info, stack_elem_t new_elem)
{

    STACK_ASSERT(stack_info);

    if (stack_info->position == stack_info->cap)
    {
        stack_resize(stack_info, INCREASE);
    }

    stack_info->stack[stack_info->position] = new_elem;
    stack_info->position++;

    stack_info->stack_hash = stack_hash(stack_info);

    STACK_ASSERT(stack_info);

}

stack_elem_t stack_pop(StackStruct *stack_info)
{

    STACK_ASSERT(stack_info);

    if (stack_info->position == stack_info->cap / 2 - 3)
    {
        stack_resize(stack_info, DECREASE);
    }

    stack_elem_t popped_elem = stack_info->stack[stack_info->position - 1];
    stack_info->stack[stack_info->position - 1] = STACK_DEFAULT_VALUE;
    stack_info->position--;

    stack_info->stack_hash = stack_hash(stack_info);

    STACK_ASSERT(stack_info);

    return popped_elem;

}

void stack_printf(StackStruct *stack_info)
{

    for (size_t c = 0; c <= stack_info->cap; c++)
    {
        printf("%d ", stack_info->stack[c]);
    }

    printf("\n");

    STACK_ASSERT(stack_info);

}

void stack_resize(StackStruct *stack_info, int mode)
{

    STACK_ASSERT(stack_info);
    assert(mode == INCREASE || mode == DECREASE);

    stack_info->cap = (mode == INCREASE) ? stack_info->cap * 2 : stack_info->cap / 2;

    stack_info->stack = (stack_elem_t *) realloc(stack_info->stack - 1, (stack_info->cap + 2) * sizeof(stack_elem_t));
    assert(stack_info->stack != NULL);

    stack_info->stack++;
    stack_info->stack[stack_info->cap] = R_CANARY;


    if (mode == INCREASE)
    {
        memset(&stack_info->stack[stack_info->cap / 2 + 1], STACK_DEFAULT_VALUE, (stack_info->cap - 2) * sizeof(stack_elem_t) / 2);
    }


    STACK_ASSERT(stack_info);

}

void dump(StackStruct *stack_info, size_t error_code)
{

    if (stack_info == NULL)
    {
        printf("struct pointer = NULL");
    }
    else if (stack_info->stack == NULL)
    {
        printf("stack pointer = NULL");
        fprintf(stack_info->file, "\ndump: %s  %s\n", __TIME__, __DATE__);
        fprintf(stack_info->file, "error code = %d\nstack pointer = NULL\nposition = %d\ncapacity = %d\nstack ---> ", error_code, stack_info->position, stack_info->cap);

    }
    else
    {
        stack_info->file = fopen("log.txt", "a");
        
        if (stack_info->file == NULL)
        {
            printf("\nfopen error");
        }
        else
        {
            fprintf(stack_info->file, "\ndump: %s  %s\n", __TIME__, __DATE__);
            fprintf(stack_info->file, "error code = %d\nstack pointer = %p\nposition = %d\ncapacity = %d\nstack ---> ", error_code, stack_info->stack, stack_info->position, stack_info->cap);
            
            size_t max_elem = (stack_info->cap == 0) ? stack_info->position : stack_info->cap;

            for (size_t i = 0; i < max_elem; i++)
            {
                fprintf(stack_info->file, "%d ", stack_info->stack[i]);
            }

            assert(fclose(stack_info->file) == 0);
        }
    }
    
}

size_t verification(StackStruct *stack_info)
{

    size_t error_code = 0;

    if (stack_info == NULL)
    {
        error_code += STACK_INFO_POINTER_ERR;
    }

    if (stack_info->stack == 0)
    {
        error_code += STACK_POINTER_ERR;
    }

    if (stack_info->cap == 0)
    {
        error_code += CAPACITY_VALUE_ERR;
    }

    if (stack_info->stack[-1] != L_CANARY)
    {
        error_code += STACK_L_CANARY_ERR;
    }

    if (stack_info->stack[stack_info->cap] != R_CANARY)
    {
        error_code += STACK_R_CANARY_ERR;
    }

    if (stack_info->l_canary != L_CANARY)
    {
        error_code += STRUCT_L_CANARY_ERR;
    }

    if (stack_info->r_canary != R_CANARY)
    {
        error_code += STRUCT_R_CANARY_ERR;
    }

    // if (stack_info->struct_hash != struct_hash(stack_info))
    // {
    //     error_code += STRUCT_HASH_ERR;
    // }

    if (stack_info->stack_hash != stack_hash(stack_info))
    {
        error_code += STACK_HASH_ERR;
    }

    return error_code;

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
    hash = ((hash << 5) + hash) + stack_info->l_canary;
    hash = ((hash << 5) + hash) + stack_info->r_canary;

}
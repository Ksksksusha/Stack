#include "stack.h"

const Elem_t TRASH_ELEM = 0xBA0BAB;
const size_t STACK_CAPACITY = 10;

enum stack_status {ALL_IS_OK = 0, 
                    NEGATIVE_SIZE = 1 << 0, 
                    NEGATIVE_POINTER = 1 << 1,
                    NEGATIVE_CAPACITY = 1 << 2,
                    SIZE_BIGGER_CAPACITY = 1 << 3, 
                    NEXT_ELEM_NOT_TRASH = 1 << 4};


static const char errors[5][50] = {"Stack has negative size\n", 
                            "Stack has negative pointer\n", 
                            "Stack has negative capacity\n", 
                            "Stack size bigger than capacity\n", 
                            "Elements in the end of stack is not trash\n"};
                            
int stack_ctor(stack* stk, const char* name, int line, const char* file, const char* func)
{
    if(stk == NULL)
    {
        printf("ERROR:\nStruct pointer = NULL\n");
        return -1;
    }

    if(stk->data)
    {
        STACK_DUMP(stk);
        return -2;
    }
    
    stk->data = (Elem_t*) calloc(STACK_CAPACITY, sizeof(Elem_t));

    stk->capacity = STACK_CAPACITY;
    stk->size = 0;
    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    if(stk->data == NULL)
    {
        printf("ERROR:\nstk->data == NULL\n");
        stk->status = NEGATIVE_POINTER;
        STACK_DUMP(stk);
        return stk->status;
    }

    for(long long stk_iter = 0; stk_iter < stk->capacity; stk_iter++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }

    stk->status = stack_ok(stk);
    return stk->status;
}


int stack_ok(stack* stk)
{
    int error = 0;

    if(stk->size < 0) error |= NEGATIVE_SIZE;
    
    if(stk->data == 0) error |= NEGATIVE_POINTER;

    if(stk->capacity < 0) error |= NEGATIVE_CAPACITY;

    if(stk->size > stk->capacity) error |= SIZE_BIGGER_CAPACITY;

    for(long long data_iter = stk->size; data_iter < stk->capacity; data_iter ++)
    {
        if(*(stk->data + data_iter) != TRASH_ELEM) 
        {
            error |= NEXT_ELEM_NOT_TRASH;
            return error;
        }
    }

    return error;
}

void print_stack_status(int error)
{
    printf("\n\nStack status:\n");
    
    if(error == 0) printf("Stack is OK\n");

    for(int error_iter = 0; error_iter < 5; error_iter++)
    {
        if((error &= (1 >> error_iter)) > 0) puts(errors[error_iter]);
    }
}

void stack_dump(stack* stk, int line, const char* file, const char* func)
{
    printf(" Stack %p ", stk);
    puts(stk->name);
    printf(" from ");
    puts(stk->func);
    printf(" (%d) ", stk->line);
    puts(stk->func);
    printf(" called from ");
    puts(file);
    printf(" (%d) ", line);
    puts(func);

    printf("{\n");

    printf("\tsize = %lld\n", stk->size);
    printf("\tcapacity = %lld\n", stk->capacity);
    printf("\tdata[%p]\n", stk->data);

    printf("\t{\n");

    for(long long stk_iter = 0; stk_iter < stk->capacity; stk_iter++)
    {
        if(stk_iter < stk->size)
        {
            printf("\t\t*[%lld] = %d;\n", stk_iter, *(stk->data + stk_iter));
        }else
        {
            printf("\t\t [%lld] = %d;\n", stk_iter, *(stk->data + stk_iter));
        }
    }

    printf("\t}\n");

    printf("}\n");
}

int stack_dtor(stack* stk)
{
    STK_STATUS(stk);

    for(long long stk_iter = 0; stk_iter < stk->size ; stk_iter ++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }
    
    free(stk->data);

    stk->capacity = -5;

    stk->size = -3;

    return 0;
}

int stack_push(stack* stk, Elem_t value)
{
    STK_STATUS(stk);

    if(stk->status) return stk->status;

    if(stk->size == stk->capacity)
    {
        stk->capacity *= 2;

        stk->data = (Elem_t *) realloc(stk->data, sizeof(Elem_t) * ((size_t) stk->capacity));

        if(stk->data == NULL)
        {
            printf("ERROR:\nstk->data == NULL\n");
            stk->status = NEGATIVE_POINTER;
            STACK_DUMP(stk);
            return stk->status;
        }

        for(long long stk_iter = stk->size; stk_iter < stk->capacity; stk_iter++)
        {
            *(stk->data + stk_iter) = TRASH_ELEM;
        }
    }

    *(stk->data + stk->size) = value;
    stk->size ++;

    stk->status = stack_ok(stk);
    return stk->status;
}

Elem_t stack_pop(stack* stk) //возвращаем верхний элемент и удаляем его
{
    STK_STATUS(stk);

    if(stk->status)
    {
        printf("\n\nERROR \nWe have problem with stack in stack_pop\n");
        print_stack_status(stk->status);
    }

    if(stk->size == 0)
    {
        printf("We want pop from empty stack\n");
        stk->size = -1;
        stk->status = stack_ok(stk);
        return TRASH_ELEM;
    }
    
    stk->size -= 1;
    Elem_t pop_elem = *(stk->data + stk->size);
    *(stk->data + stk->size) = TRASH_ELEM;

    if(stk->size < (stk->capacity)/2)
    {
        stk->capacity = (stk->capacity + 1)/2;

        stk->data = (Elem_t *) realloc(stk->data, sizeof(Elem_t) * ((size_t) stk->capacity));

        if(stk->data == NULL)
        {
            printf("ERROR:\nstk->data == NULL\n");
            stk->status = NEGATIVE_POINTER;
            STACK_DUMP(stk);
            return TRASH_ELEM;
        }
    }

    return pop_elem;
}

#include "stack.h"


const Elem_t TRASH_ELEM = 0xDEAD;
const Canary_t CANARY_ELEM = 0xBADC0FFEE;
const size_t STACK_CAPASITY = 10;




enum stack_status {ALL_IS_OK = 0, NEGATIVE_SIZE = 1, NEGATIVE_ITER = 2, NEGATIVE_CAPACITY = 4, 
                    SIZE_BIGGER_CAPACITY = 8, NEXT_ELEM_NOT_TRASH = 16};




int stack_ctor(stack* stk, const char* name, int line, const char* file, const char* func)
{
    stk->data = (Elem_t*) calloc(STACK_CAPASITY, sizeof(Elem_t));
    stk->capacity = STACK_CAPASITY;
    stk->size = 0;
    stk->name = strdup_(name);
    stk->line = line;
    stk->file = strdup_(file);
    stk->func = strdup_(func);
    stk->status = stack_ok(stk);

    for(long long stk_iter = 0; stk_iter < stk->capacity; stk_iter++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }
    return stk->status;
}


int stack_ok(stack* stk)
{
    int error = 0;

    if(stk->size < 0) error += NEGATIVE_SIZE;
    
    if(stk->data == 0) error += NEGATIVE_ITER;

    if(stk->capacity < 0) error += NEGATIVE_CAPACITY;

    if(stk->size > stk->capacity) error += SIZE_BIGGER_CAPACITY;

    for(long long data_iter = stk->size; data_iter < stk->capacity; data_iter ++)
    {
        if(*(stk->data + data_iter) != TRASH_ELEM) 
        {
            error += NEXT_ELEM_NOT_TRASH;
        }
    }

    return error;
}

void print_stack_status(int error)
{
    printf("\n\nStack status:\n");
    
    if(error == 0) printf("Stack is OK\n");

    if(error > NEXT_ELEM_NOT_TRASH) printf("%d elements in the end of stack is not trash\n", error/16);
    error %= NEXT_ELEM_NOT_TRASH;
    
    if(error > SIZE_BIGGER_CAPACITY) printf("Stack size bigger than capacity\n");
    error %= SIZE_BIGGER_CAPACITY;
    
    if(error > NEGATIVE_CAPACITY) printf("Stack have negative capacity\n");
    error %= NEGATIVE_CAPACITY;

    if(error > NEGATIVE_ITER) printf("Stack have negative iter\n");
    error %= NEGATIVE_ITER;

    if(error > NEGATIVE_SIZE) printf("Stack have negative size\n");
}

void stack_dump(stack* stk, int line, const char* file, const char* func)
{
    printf(" Stack %p ", stk);
    puts_(stk->name);
    printf(" from ");
    puts_(stk->func);
    printf(" (%d) ", stk->line);
    puts_(stk->func);
    printf(" called from ");
    puts_(file);
    printf(" (%d) ", line);
    puts_(func);

    printf("{\n");

    printf("\tsize = %lld\n", stk->size);
    printf("\tcapasity = %lld\n", stk->capacity);
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

int stack_dtor(stack* stk)// возвращает статус стека, если он < 0, то мы испортили стек :)
{
    for(long long stk_iter = 0; stk_iter < stk->size ; stk_iter ++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }
    
    free(stk->data);

    strcpy_(stk->file, "-");
    free(stk->file);

    strcpy_(stk->func, "-");
    free(stk->func);

    strcpy_(stk->name, "-");
    free(stk->name);

    stk->capacity = -5;

    stk->size = -3;

    stk->status = stack_ok(stk);

    return stk->status;
}

int stack_push(stack* stk, Elem_t value)
{
    stk->status = stack_ok(stk);
    if(stk->status)
    {
        printf("\n\nERROR \nWe have problem with stack in stack_push\n");
        print_stack_status(stk->status);
        STACK_DUMP(stk);
        return stk->status;
    }

    if(stk->size == stk->capacity)
    {
        stk->capacity *= 2;

        stk->data = (Elem_t *) realloc(stk->data, sizeof(Elem_t) * ((size_t) stk->capacity));

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
    stk->status = stack_ok(stk);
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
    }
    
    stk->size -= 1;
    Elem_t pop_elem = *(stk->data + stk->size);
    *(stk->data + stk->size) = TRASH_ELEM;

    if(stk->size < (stk->capacity)/2)
    {
        stk->capacity = (stk->capacity + 1)/2;

        stk->data = (Elem_t *) realloc(stk->data, sizeof(Elem_t) * ((size_t) stk->capacity));
    }

    return pop_elem;
}


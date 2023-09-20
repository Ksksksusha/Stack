#include <stack.h>

typedef int Elem_t;

struct stack
{
    Elem_t* data;
    size_t size;
    size_t capacity;
    size_t status;
};

const int TRASH_ELEM = 10000;

enum stack_status {ALL_IS_OK = 0, NEGATIVE_SIZE = 1, NEGATIVE_ITER = 2, NEGATIVE_CAPACITY = 4, 
                    SIZE_BIGGER_CAPACITY = 8, NEXT_ELEM_NOT_TRASH = 16};


size_t stack_ok(const *stack stk)
{
    int error = 0;

    if(stk.size < 0) error += NEGATIVE_SIZE;
    
    if(stk.data <= 0) error += NEGATIVE_ITER;

    if(stk.capacity < 0) error += NEGATIVE_CAPACITY;

    if(stk.size > stk.capacity) error += SIZE_BIGGER_CAPACITY;

    for(size_t data_iter = size; data_iter < capacity; data_iter ++)
    {
        if(*(data + data_iter) != TRASH_ELEM) 
        {
            error += NEXT_ELEM_NOT_TRASH;
        }
    }
}

void print_stack_status(size_t error)
{
    printf("\n\nStack status:\n")
    
    if(error == 0) printf("Stack is OK\n");

    if(error > NEXT_ELEM_NOT_TRASH) printf("%z elements in the end of stack is not trash\n", error/16);
    error %= NEXT_ELEM_NOT_TRASH;
    
    if(error > SIZE_BIGGER_CAPACITY) printf("Stack size bigger than capacity\n");
    error %= SIZE_BIGGER_CAPACITY;
    
    if(error > NEGATIVE_CAPACITY) printf("Stack have negative capacity\n");
    error %= NEGATIVE_CAPACITY;

    if(error > NEGATIVE_ITER) printf("Stack have negative iter\n");
    error %= NEGATIVE_ITER;

    if(error > NEGATIVE_SIZE) printf("Stack have negative size\n");
}

size_t stack_creator(stack* stk)
{
    
    return stack_ok(stk);
}

size_t stack_free(stack* stk)// возвращает статус стека, если он < 0, то мы испортили стек :)
{
    for(size_t stk_iter = 0, stk_iter < stk.size ; stk_iter ++)
    {
        *(stk + stk_iter) = TRASH_ELEM;
    }
    
    free(stk->data);

    stk->capacity = -5;

    stk->size = -3;

    stk->status = stack_ok(stk);

    return stk->status;
}

size_t stack_push(stack* stk, Elem_t value)
{
    stk->status = stack_ok(stk);
    if(stk->status)
    {
        printf("\n\nERROR \nWe have problem with stack in stack_push\n");
        print_stack_status(stk->status);
        return stk->status;
    }

    if(stk->size == stk->capacity)
    {
        stack_realloc(stk);     //  дописать!!!!!  делаем capacity в 2 раза больше
    }

    *(stk->data + stk->size) = value;
    stk->size ++;

    stk->status = stack_ok(stk);
    return stk->status;
}

size_t stack_pop(stack* stk) //возвращаем верхний эдемент и удаляем его
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
    size_t pop_elem = *(stk->data + stk->size);
    *(stk->data + stk->size) = TRASH_ELEM;

    //написать уменьшение величины стека в 2 раза
}


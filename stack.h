#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <locale>
#include <sys/stat.h>
#include <malloc.h>


#define STACK_DUMP(stk) stack_dump((stk), __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define STACK_CTOR(stk) stack_ctor((stk), #stk,  __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define  STK_STATUS(stk) stk->status = stack_ok(stk);                                               \
    if(stk->status)                                                                                 \
    {                                                                                               \
        printf("\n\nERROR \nWe have problem with stack in %s \n", __PRETTY_FUNCTION__);             \
        print_stack_status(stk->status);                                                            \
        STACK_DUMP(stk);                                                                            \
    }

typedef int Elem_t;
typedef unsigned long long Canary_t;

struct stack
{
    Elem_t* data;//указатель на начало массива, не на канарейку в случае стека с защитой
    long long size;
    long long capacity;
    int status;

    char* name;
    int line;
    char* file;
    char* func;
};


//stack_functions

int stack_ctor(stack* stk, const char* name, int line, const char* file, const char* func);

int stack_ok(stack* stk);

void print_stack_status(int error);

void stack_dump(stack* stk, int line, const char* file, const char* func);

int stack_dtor(stack* stk);

int stack_push(stack* stk, Elem_t value);

Elem_t stack_pop(stack* stk);



#endif //STACK_H
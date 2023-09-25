#ifndef SAFETY_STACK_H
#define SAFETY_STACK_H

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <locale>
#include <sys/stat.h>
#include <malloc.h>


#define STACK_DUMP_S(stk) stack_dump_s((stk), __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define STACK_CTOR_S(stk) stack_ctor_s((stk), #stk,  __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define  STK_STATUS_S(stk) stk->status = stack_ok_s(stk);                                               \
    if(stk->status)                                                                                 \
    {                                                                                               \
        printf("\n\nERROR \nWe have problem with stack in %s \n", __PRETTY_FUNCTION__);             \
        print_stack_status_s(stk->status);                                                            \
        STACK_DUMP_S(stk);                                                                            \
    }

typedef int Elem_t;
typedef unsigned long long Canary_t;

const Elem_t TRASH_ELEM = 0xDEAD;
const Canary_t CANARY_ELEM = 0xBADC0FFEE;
const size_t STACK_CAPACITY = 8;// STACK_CAPACITY % 8 = 0!!!!!!!! for canary

enum stack_status {ALL_IS_OK = 0,
                    NEGATIVE_SIZE = 1 << 0, 
                    NEGATIVE_ITER = 1 << 1, 
                    NEGATIVE_CAPACITY = 1 << 2,
                    SIZE_BIGGER_CAPACITY = 1 << 3, 
                    NEXT_ELEM_NOT_TRASH = 1 << 4, 
                    LEFT_CANARY = 1 << 5,
                    RIGHT_CANARY = 1 << 6, 
                    NEGATIVE_ITER_LEFT_CANARY = 1 << 7, 
                    NEGATIVE_ITER_RIGHT_CANARY = 1 << 8,
                    LEFT_STRUCT_CANARY = 1 << 9,
                    RIGHT_STRUCT_CANARY = 1 << 10};



struct safety_stack
{
    Canary_t canary_stack_left = CANARY_ELEM;

    Canary_t* canary_left;
    Canary_t* canary_right;

    Elem_t* data;//указатель на начало массива, не на канарейку в случае стека с защитой
    long long size;
    long long capacity;
    int status;

    char* name;
    int line;
    char* file;
    char* func;

    Canary_t canary_stack_right = CANARY_ELEM;
};

//safety_stack_functions

int stack_ctor_s(safety_stack* stk, const char* name, int line, const char* file, const char* func);

int stack_ok_s(safety_stack* stk);

void print_stack_status_s(int error);

void stack_dump_s(safety_stack* stk, int line, const char* file, const char* func);

int stack_dtor_s(safety_stack* stk);

int stack_push_s(safety_stack* stk, Elem_t value);

Elem_t stack_pop_s(safety_stack* stk);



#endif //SAFETY_STACK_H
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

typedef int Elem_t;
typedef unsigned long long Canary_t;

struct stack
{
    Elem_t* data;
    long long size;
    long long capacity;
    int status;

    char* name;
    int line;
    char* file;
    char* func;
};


//string functions

int puts_(const char *str);

size_t strlen_(const char *str);

char *strcpy_(char *str1, const char *str2);

char *strncpy_(char *str1, const char *str2, size_t count);

char *strcat_(char *str1, const char *str2);

char *strncat_(char *str1, const char *str2, size_t count);

char *strdup_(const char *str);

int strcmp_(const char *str1, const char *str2);

//stack_functions

int stack_ctor(stack* stk, const char* name, int line, const char* file, const char* func);

int stack_ok(stack* stk);

void print_stack_status(int error);

void stack_dump(stack* stk, int line, const char* file, const char* func);

int stack_dtor(stack* stk);

int stack_push(stack* stk, Elem_t value);

Elem_t stack_pop(stack* stk);


#endif //STACK_H
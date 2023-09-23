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

struct stack;


//string functions

int puts_(const char *str);

size_t strlen_(const char *str);

char *strchr_(const char *str, int ch);

char *strcpy_(char *str1, const char *str2);

char *strncpy_(char *str1, const char *str2, size_t count);

char *strcat_(char *str1, const char *str2);

char *strncat_(char *str1, const char *str2, size_t count);

char *fgets_(char *str, int num, FILE *stream);

char *strdup_(const char *str);

int strcmp_(const char *str1, const char *str2);

//stack_functions

size_t stack_ctor(stack* stk, const char* name, int line, const char* file, const char* func);

size_t stack_ok(stack* stk);

void print_stack_status(size_t error);

void stack_dump(stack* stk, int line, const char* file, const char* func);

size_t stack_dtor(stack* stk);

size_t stack_push(stack* stk, Elem_t value);

size_t stack_pop(stack* stk);


#endif //STACK_H
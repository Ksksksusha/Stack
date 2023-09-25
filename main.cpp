#include "stack.h"

int main()
{
    struct stack stk;

    STACK_CTOR(&stk);

    printf("We create stack");

    STACK_DUMP(&stk);

    stack_push(&stk, 5);
    STACK_DUMP(&stk);

    stack_push(&stk, 6);
    STACK_DUMP(&stk);

    stack_push(&stk, 7);
    STACK_DUMP(&stk);

    stack_push(&stk, 8);
    STACK_DUMP(&stk);

    printf("%d\n", stack_pop(&stk));
    STACK_DUMP(&stk);

    printf("%d\n", stack_pop(&stk));
    STACK_DUMP(&stk);

    printf("%d\n", stack_pop(&stk));
    STACK_DUMP(&stk);

    printf("%d\n", stack_pop(&stk));
    STACK_DUMP(&stk);

    return 0;
}
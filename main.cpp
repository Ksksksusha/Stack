#include "stack.h"
#include "safety_stack.h"

int main()
{
    //check stack

    struct stack stk1;

    STACK_CTOR(&stk1);
    STACK_DUMP(&stk1);

    stack_push(&stk1, 5);
    STACK_DUMP(&stk1);

    stack_push(&stk1, 6);
    STACK_DUMP(&stk1);

    stack_push(&stk1, 7);
    STACK_DUMP(&stk1);

    stack_push(&stk1, 8);
    STACK_DUMP(&stk1);

    printf("%d\n", stack_pop(&stk1));
    STACK_DUMP(&stk1);

    printf("%d\n", stack_pop(&stk1));
    STACK_DUMP(&stk1);

    printf("%d\n", stack_pop(&stk1));
    STACK_DUMP(&stk1);

    printf("%d\n", stack_pop(&stk1));
    STACK_DUMP(&stk1);

    stack_dtor(&stk1);
    

    // check safety_stack

    struct safety_stack stk2;

    STACK_CTOR_S(&stk2);
    STACK_DUMP_S(&stk2);

    stack_push_s(&stk2, 5);
    STACK_DUMP_S(&stk2);

    stack_push_s(&stk2, 4);
    STACK_DUMP_S(&stk2);

    stack_push_s(&stk2, 3);
    STACK_DUMP_S(&stk2);

    stack_push_s(&stk2, 2);
    STACK_DUMP_S(&stk2);

    stack_push_s(&stk2, 1);
    STACK_DUMP_S(&stk2);

    printf("%d\n", stack_pop_s(&stk2));
    STACK_DUMP_S(&stk2);

    printf("%d\n", stack_pop_s(&stk2));
    STACK_DUMP_S(&stk2);

    stack_dtor_s(&stk2);
    return 0;
}
//#include "stack.h"
#include "safety_stack.h"

int main()
{
    //check stack

    /*struct stack stk;

    STACK_CTOR(&stk);

    printf("We create stack!\n");

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
    */

    // check safety_stack

    struct safety_stack stk2;

    STACK_CTOR_S(&stk2);

    printf("We create safety_stack!\n");

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


    return 0;
}
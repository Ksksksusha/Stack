#include "safety_stack.h"

static char errors[11][50] = {"Stack have negative size\n",
                                "Stack have negative iter\n",
                                "Stack have negative capacity\n", 
                                "Stack size bigger than capacity\n",
                                "Elements in the end of stack is not trash\n",
                                "Left canary catch error in data\n", 
                                "Right canary catch error in data\n",
                                "Left canary have negative iter\n", 
                                "Right canary have negative iter\n",
                                "Left canary catch error in struct\n", 
                                "Right canary catch error in struct\n"};

int stack_ctor_s(safety_stack* stk, const char* name, int line, const char* file, const char* func)//
{
    void* addr = malloc(sizeof(Canary_t)*2 + sizeof(Elem_t)*STACK_CAPACITY);

    stk->canary_left = (Canary_t*) addr;
    stk->data = (Elem_t*) (stk->canary_left + 1);
    stk->canary_right = (Canary_t*) (stk->data + STACK_CAPACITY);

    stk->capacity = STACK_CAPACITY;
    stk->size = 0;
    stk->name = strdup(name);
    stk->line = line;
    stk->file = strdup(file);
    stk->func = strdup(func);
    stk->status = stack_ok_s(stk);

    *(stk->canary_left) = CANARY_ELEM;
    *(stk->canary_right) = CANARY_ELEM;

    for(long long stk_iter = 0; stk_iter < stk->capacity; stk_iter++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }
    return stk->status;
}


int stack_ok_s(safety_stack* stk)
{
    int error = 0;

    if(stk->size < 0) error |= NEGATIVE_SIZE;
    
    if(stk->data == 0) error |= NEGATIVE_ITER;

    if(stk->capacity < 0) error |= NEGATIVE_CAPACITY;

    if(stk->size > stk->capacity) error |= SIZE_BIGGER_CAPACITY;

    if(*(stk->canary_left) != CANARY_ELEM) error |= LEFT_CANARY;

    if(*(stk->canary_right) != CANARY_ELEM) error |= RIGHT_CANARY;

    if(stk->canary_left == 0) error |= NEGATIVE_ITER_LEFT_CANARY;

    if(stk->canary_right == 0) error |= NEGATIVE_ITER_RIGHT_CANARY;    

    if(stk->canary_stack_left != CANARY_ELEM) error |= LEFT_STRUCT_CANARY;

    if(stk->canary_stack_right != CANARY_ELEM) error |= RIGHT_STRUCT_CANARY;

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

void print_stack_status_s(int error)
{
    printf("\n\nStack status:\n");
    
    if(error == 0) printf("Stack is OK\n");

    for(int error_iter = 0; error_iter < 11; error_iter++)
    {
        if((error &= (1 >> error_iter)) > 0) puts(errors[error_iter]);
    }
}

void stack_dump_s(safety_stack* stk, int line, const char* file, const char* func)
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
    printf("\tdata[%p]\n\n", stk->data);

    printf("Left canary = %lld\n", *(stk->canary_left));
    printf("Right canary = %lld\n\n", *(stk->canary_right));

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

int stack_dtor_s(safety_stack* stk)// возвращает статус стека, если он < 0, то мы испортили стек :)
{
    for(long long stk_iter = 0; stk_iter < stk->size ; stk_iter ++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }
    
    free(stk->canary_left);//тк именно на ней начинается заказанный блок памяти

    strcpy(stk->file, "-");
    free(stk->file);

    strcpy(stk->func, "-");
    free(stk->func);

    strcpy(stk->name, "-");
    free(stk->name);

    stk->capacity = -5;

    stk->size = -3;

    stk->status = stack_ok_s(stk);

    return stk->status;
}

int stack_push_s(safety_stack* stk, Elem_t value)
{
    STK_STATUS_S(stk);

    if(stk->status) return stk->status;

    if(stk->size == stk->capacity)
    {
        stk->capacity *= 2;//уже кратна 8, поэтому мы за это не переживаем

        void* addr = realloc(stk->canary_left, sizeof(Canary_t)*2 + sizeof(Elem_t)*((size_t) stk->capacity));

        stk->canary_left = (Canary_t*) addr;
        stk->data = (Elem_t*) (stk->canary_left + 1);
        stk->canary_right = (Canary_t*) (stk->data + stk->capacity);

        for(long long stk_iter = 0; stk_iter < stk->capacity; stk_iter++)
        {
            *(stk->data + stk_iter) = TRASH_ELEM;
        }
    }

    *(stk->data + stk->size) = value;
    stk->size ++;

    stk->status = stack_ok_s(stk);
    return stk->status;
}

Elem_t stack_pop_s(safety_stack* stk) //возвращаем верхний элемент и удаляем его
{
    STK_STATUS_S(stk);

    if(stk->status)
    {
        printf("\n\nERROR \nWe have problem with stack in stack_pop\n");
        print_stack_status_s(stk->status);
    }

    if(stk->size == 0)
    {
        printf("We want pop from empty stack\n");
        stk->size = -1;
        stk->status = stack_ok_s(stk);
        return TRASH_ELEM;
    }
    
    stk->size -= 1;
    Elem_t pop_elem = *(stk->data + stk->size);
    *(stk->data + stk->size) = TRASH_ELEM;

    if(stk->size < (stk->capacity)/2 && stk->size >= 8)
    {
        stk->capacity = (stk->capacity)/2; // и так была кратна 8, из условия что она больше 8, при делении так же кратна 8

        void* addr = realloc(stk->canary_left, sizeof(Canary_t)*2 + sizeof(Elem_t)*((size_t) stk->capacity));

        stk->canary_left = (Canary_t*) addr;
        stk->data = (Elem_t*) (stk->canary_left + 1);
        stk->canary_right = (Canary_t*) (stk->data + stk->capacity);
    }

    return pop_elem;
}


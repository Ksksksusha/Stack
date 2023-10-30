#include "safety_stack.h"

static const char errors[13][50] = {
    "Stack has negative size\n",
    "Stack has negative pointer\n",
    "Stack has negative capacity\n", 
    "Stack size bigger than capacity\n",
    "Elements in the end of stack is not trash\n",
    "Left canary catch error in data\n", 
    "Right canary catch error in data\n",
    "Left canary has negative iter\n", 
    "Right canary has negative iter\n",
    "Left canary catch error in struct\n", 
    "Right canary catch error in struct\n", 
    "Hash doesn't match\n",
    "Stack struct has null pointer\n"
};

int stack_ctor_s(safety_stack* stk, const char* name, int line, const char* file, const char* func)
{

    if(stk == NULL)
    {
        printf("ERROR:\nStruct pointer = NULL\n");
        return -1;
    }

    void* addr = malloc(sizeof(Canary_t)*2 + sizeof(Elem_t)*STACK_CAPACITY);

    if(addr == nullptr)
    {
        printf("ERROR:\nstk->data == NULL\n");
        stk->status = NEGATIVE_POINTER;
        STACK_DUMP_S(stk);
        return stk->status;
    }
   
    stk->canary_left = (Canary_t*) addr;
    stk->data = (Elem_t*) (stk->canary_left + 1);
    stk->canary_right = (Canary_t*) (stk->data + STACK_CAPACITY);

    stk->capacity = STACK_CAPACITY;
    stk->size = 0;
    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    *(stk->canary_left)  = CANARY_ELEM;
    *(stk->canary_right) = CANARY_ELEM;


    for(long long stk_iter = 0; stk_iter < stk->capacity; stk_iter++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }

    stk->hash = murmur_hash(stk);

    stk->status = stack_ok_s(stk);

    return stk->status;
}


static int stack_ok_s(safety_stack* stk)
{
    int error = 0;

    if(stk == nullptr) 
    {
        printf("Stk is nullptr!\n");
        error |= NULL_POINTER_STRUCT_STK;
        return error;
    }

    if(stk->size < 0) error |= NEGATIVE_SIZE;
    
    if(stk->data == 0) error |= NEGATIVE_POINTER;

    if(stk->capacity < 0) error |= NEGATIVE_CAPACITY;

    if(stk->size > stk->capacity) error |= SIZE_BIGGER_CAPACITY;

    if(*(stk->canary_left) != CANARY_ELEM) error |= LEFT_CANARY;

    if(*(stk->canary_right) != CANARY_ELEM) error |= RIGHT_CANARY;

    if(stk->canary_left == 0) error |= NEGATIVE_ITER_LEFT_CANARY;

    if(stk->canary_right == 0) error |= NEGATIVE_ITER_RIGHT_CANARY;    

    if(stk->canary_stack_left != CANARY_ELEM) error |= LEFT_STRUCT_CANARY;

    if(stk->canary_stack_right != CANARY_ELEM) error |= RIGHT_STRUCT_CANARY;

    if(stk->hash != murmur_hash(stk)) error |= HASH_ERROR;

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

static void print_stack_status_s(int error)
{
    printf("\n\nStack status:\n");
    
    if(error == 0) printf("Stack is OK\n");

    for(int error_iter = 0; error_iter < 12; error_iter++)
    {
        if((error &= (1 << error_iter)) > 0)
        {
            puts(errors[error_iter]);
            printf("\n");
        } 
    }
}

void stack_dump_s(safety_stack* stk, int line, const char* file, const char* func)
{
    printf(" Safety stack %p ", stk);
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

    printf("Left canary = %llu\n", *(stk->canary_left));
    printf("Right canary = %llu\n\n", *(stk->canary_right));

    printf("\t{\n");

    for(long long stk_iter = 0; stk_iter < stk->capacity; stk_iter++)
    {
        if(stk_iter < stk->size)
        {
            printf("\t\t*[%lld] = %lg;\n", stk_iter, *(stk->data + stk_iter));
        }else
        {
            printf("\t\t [%lld] = %lg;\n", stk_iter, *(stk->data + stk_iter));
        }
    }

    printf("\t}\n");

    printf("}\n");
}

int stack_dtor_s(safety_stack* stk) // возвращает статус стека, если он < 0, то мы испортили стек :)
{
    STK_STATUS_S(stk);
    
    for(long long stk_iter = 0; stk_iter < stk->size ; stk_iter ++)
    {
        *(stk->data + stk_iter) = TRASH_ELEM;
    }

    stk->capacity = -5;

    stk->size = -3;

    free(stk->canary_left); // тк именно на ней начинается заказанный блок памяти

    return 0;
}

int stack_push_s(safety_stack* stk, Elem_t value)
{
    STK_STATUS_S(stk);

    if(stk->status) return stk->status;

    if(stk->size == stk->capacity)
    {
        stk->capacity *= 2; // уже кратна 8, поэтому мы за это не переживаем

        void* addr = realloc(stk->canary_left, sizeof(Canary_t)*2 + sizeof(Elem_t)*((size_t) stk->capacity));

        if(addr == NULL)
        {
            printf("ERROR:\nstk->data == NULL\n");
            stk->status = NEGATIVE_POINTER;
            STACK_DUMP_S(stk);
            return stk->status;
        }

        stk->canary_left = (Canary_t*) addr;
        stk->data = (Elem_t*) (stk->canary_left + 1);
        stk->canary_right = (Canary_t*) (stk->data + stk->capacity);

        for(long long stk_iter = stk->size; stk_iter < stk->capacity; stk_iter++)
        {
            *(stk->data + stk_iter) = TRASH_ELEM;
        }
    }

    *(stk->data + stk->size) = value;
    stk->size ++;

    stk->hash = murmur_hash(stk);

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

        if(addr == NULL)
        {
            printf("ERROR:\nstk->data == NULL\n");
            stk->status = NEGATIVE_POINTER;
            STACK_DUMP_S(stk);
            return stk->status;
        }

        stk->canary_left = (Canary_t*) addr;
        stk->data = (Elem_t*) (stk->canary_left + 1);
        stk->canary_right = (Canary_t*) (stk->data + stk->capacity);
    }

    stk->hash = murmur_hash(stk);

    return pop_elem;
}


static unsigned int murmur_hash(safety_stack* stk)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const unsigned int r = 24;

    unsigned int len = (unsigned int) stk->size;

    unsigned int h = seed ^ len;

    const unsigned int * data = (const unsigned int *)stk->data;
    unsigned int k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8u;
        k |= data[2] << 16u;
        k |= data[3] << 24u;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3: 
        h ^= data[2] << 16u;
        [[fallthrough]];
    case 2: 
        h ^= data[1] << 8u;
        [[fallthrough]];
    case 1:
        h ^= data[0];
        h *= m;
        [[fallthrough]];
    default: 
        break;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
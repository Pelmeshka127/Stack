#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>

#include "stack_library.h"
#include "stackerrors.h"

Stack_Errno Stack_Ctor(Stack_Info * const MyStack)
{
    assert(MyStack);

    MyStack->capacity = Def_Capacity;

    MyStack->size = 0;

    MyStack->data = (int *)calloc (MyStack->capacity + 2, sizeof(int));
    
    if (MyStack->data == nullptr)
    {
        Return_Err();

        Stack_Dtor(MyStack);

        return Stack_Errno::Stack_Alloc_Err;
    }

    return Stack_Errno::Stack_No_Err;
}


Stack_Errno Stack_Push(Stack_Info * const MyStack, const int value)
{
    assert(MyStack);

    if (MyStack->size >= MyStack->capacity)
        if (Stack_Upsize(MyStack) == Stack_Errno::Stack_Alloc_Err)
            return Stack_Errno::Stack_Alloc_Err;
    
    MyStack->data[MyStack->size++] = value; 

    return Stack_Errno::Stack_No_Err;
}


Stack_Errno Stack_Pop(Stack_Info * const MyStack)
{
    assert(MyStack);

    if (Is_Empty(MyStack))
    {
        Return_Err();
        printf("Stack is underflow\n");
        return Stack_Errno::Stack_Underflow;
    }

    else if (2.5 * MyStack->size <= MyStack->capacity)
        if (Stack_Lowsize(MyStack) == Stack_Errno::Stack_Alloc_Err)
            return Stack_Errno::Stack_Alloc_Err;

    MyStack->data[MyStack->size--];

    return Stack_Errno::Stack_No_Err;
}


Stack_Errno Stack_Upsize(Stack_Info * MyStack)
{
    assert(MyStack);

    printf("Need to increase the capacity: The vlaue of capacity is %d\n", MyStack->capacity);
    
    MyStack->capacity *= Multiplier;

    int * Alloc_Mem = (int *)realloc (MyStack->data, MyStack->capacity * sizeof(int));

    if (Alloc_Mem == nullptr)
    {
        printf("Failed allocation of memory\n");

        free(MyStack->data);

        Return_Err();
        
        return Stack_Errno::Stack_Alloc_Err;
    }

    MyStack->data = Alloc_Mem;

    printf("The capacity after increasing is %d\n", MyStack->capacity);

    return Stack_Errno::Stack_No_Err;
}


Stack_Errno Stack_Lowsize(Stack_Info * MyStack)
{
    assert(MyStack);

    printf("Need to decrease the capacity: The vlaue of capacity is %d\n", MyStack->capacity);

    MyStack->capacity = MyStack->capacity / Multiplier + 1; //think bout it

    int * Alloc_Mem = (int *)realloc (MyStack->data , MyStack->capacity * sizeof(int));

    if(Alloc_Mem == nullptr)
    {
        printf("Failed allocation of memory\n");

        free(MyStack->data);
            
        Return_Err();
            
        return Stack_Errno::Stack_Alloc_Err;
    }

    MyStack->data = Alloc_Mem;

    printf("The capacity after decreasing is %d\n", MyStack->capacity);

    return Stack_Errno::Stack_No_Err;
}


int Stack_Peep(Stack_Info * MyStack)
{
    assert(MyStack);

    if (Is_Empty(MyStack))
    {
        Return_Err();
        return (int) Stack_Errno::Stack_Underflow;
    }

    return MyStack->data[MyStack->size - 1];
}


void Stack_Dtor(Stack_Info * MyStack)
{
    assert(MyStack);

    free(MyStack->data);
}


void Stack_Dump(const Stack_Info * MyStack)
{
    assert(MyStack);

    printf("The capacity of stack is %d\n"
           "The size of stack is %d\n",
           MyStack->capacity, MyStack->size);
}
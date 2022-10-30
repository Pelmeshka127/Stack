#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>

#include "stackfunctions.h"

//---------------------------------------------------------------------//

Stack_Errno Stack_Verificator(Stack_Info * const MyStack)
{
    if (MyStack->capacity <= 0)
    {
        fprintf(stderr, "Incorrect value of capacity: %d\n", MyStack->capacity);
        return Stack_Errno::Stack_Undef_Capacity;
    }
    
    if (MyStack->size < 0)
    {
        fprintf(stderr, "The value of size of Stack is %d\n", MyStack->size);
        return Stack_Errno::Stack_Underflow;
    }

    if (MyStack->size > MyStack->capacity)
    {
        fprintf(stderr, "The value of size %d is bigger than value of capacity %d\n", MyStack->size, MyStack->capacity);
        return Stack_Errno::Stack_Overflow;
    }

    if (MyStack->data == nullptr)
    {
        fprintf(stderr, "Memory allocaion was failed\n");
        return Stack_Errno::Stack_Alloc_Err;
    }

    if (MyStack->Canary_Left != CANARY_VALUE)
    {
        fprintf(stderr, "The value of Canary_Left in MyStack was changed: %d\n", MyStack->Canary_Left);
        return Stack_Errno::Stack_Canary_Left_Failed;
    }

    if (MyStack->Canary_Right != CANARY_VALUE)
    { 
        fprintf(stderr, "The value of Canary_Right in MyStack was changed: %d\n", MyStack->Canary_Right);
        return Stack_Errno::Stack_Canary_Right_Failed;
    }

    if (MyStack->data[0] != CANARY_VALUE)
    {
        fprintf(stderr, "The value of Canary in the start of data was changed: %d\n", MyStack->data[0]);
        return Stack_Errno::Stack_Canary_Data_Left_Failed;
    }

    if (MyStack->data[MyStack->capacity + 1] != CANARY_VALUE)
    {
        fprintf(stderr, "The valueo of Canary in the end of data was changed: %d\n", MyStack->data[MyStack->capacity + 1]);
        return Stack_Errno::Stack_Canary_Data_Right_Failed;
    }

    if (MyStack->logfile == nullptr)
    {
        fprintf(stderr, "Openning of logfile was failed");
        return Stack_Errno::Stack_Logfile_Error;
    }

    return Stack_Errno::Stack_No_Err;
}

//---------------------------------------------------------------------//

void Error_Meaning(Stack_Info * MyStack, Stack_Errno Error)
{
    switch (Error)
    {
        case Stack_Errno::Stack_Undef_Capacity:
            fprintf(MyStack->logfile, "Incorrect value of capacity: %d\n", MyStack->capacity);
            break;
        
        case Stack_Errno::Stack_Underflow:
            fprintf(MyStack->logfile, "The value of size of Stack is %d\n", MyStack->size);
            break;

        case Stack_Errno::Stack_Overflow:
            fprintf(MyStack->logfile, "The value of size %d is bigger than value of capacity %d\n", 
            MyStack->size, MyStack->capacity);
            break;
        
        case Stack_Errno::Stack_Alloc_Err:
            fprintf(MyStack->logfile, "Memory allocaion was failed\n");
            break;
        
        case Stack_Errno::Stack_Canary_Left_Failed:
            fprintf(MyStack->logfile, "The value of Canary_Left in MyStack was changed from %u to %d\n", 
            CANARY_VALUE, MyStack->Canary_Left);
            break;
        
        case Stack_Errno::Stack_Canary_Right_Failed:
            fprintf(MyStack->logfile, "The value of Canary_RIght in MyStack was changed from %u to: %d\n", 
            CANARY_VALUE, MyStack->Canary_Right);
            break;

        case Stack_Errno::Stack_Canary_Data_Left_Failed:
            fprintf(MyStack->logfile, "The value of Canary in the start of data was changed from %u to: %d\n", 
            CANARY_VALUE, MyStack->data[0]);
            break;

        case Stack_Errno::Stack_Canary_Data_Right_Failed:
            fprintf(MyStack->logfile, "The value of Canary in the end of data was changed from %u to: %d\n", 
            CANARY_VALUE, MyStack->data[MyStack->capacity + 1]);
            break;

        case Stack_Errno::Stack_Logfile_Error:
            fprintf(MyStack->logfile, "Openning of logfile was failed\n");
            break;

        default:
            fprintf(MyStack->logfile, "There's no errors. Stack is OK\n");
    }
}

//---------------------------------------------------------------------//

Stack_Errno Stack_Ctor(Stack_Info * const MyStack)
{
    assert(MyStack);
    
    Stack_Errno Error = Stack_Errno::Stack_No_Err;

    MyStack->logfile = fopen("logfile.txt", "w");

    if (MyStack->logfile == nullptr)
    {
        fprintf(stderr, "%s: Openning of logfile was failed\n", strerror(errno));
        return Stack_Errno::Stack_Logfile_Error;
    }

    MyStack->capacity = DEF_CAPACITY;

    if (MyStack->capacity <= 0)
    {
        STACK_DUMP(MyStack, Stack_Errno::Stack_Undef_Capacity);
        fprintf(stderr, "Incorrect value of capacity: %d\n", MyStack->capacity);
        return Stack_Errno::Stack_Undef_Capacity;
    }

    MyStack->size = 0;

    int * Alloc_Mem = (int *)calloc (MyStack->capacity + 2, sizeof(int));

    if (Alloc_Mem == nullptr)
    {
        STACK_DUMP(MyStack, Stack_Errno::Stack_Alloc_Err);
        fprintf(stderr, "Memory allocaion was failed\n");
        return Stack_Errno::Stack_Alloc_Err;
    }
    
    MyStack->data = Alloc_Mem;

    MyStack->data[0] = (int) MyStack->Canary_Left;

    MyStack->data[MyStack->capacity + 1] = (int) MyStack->Canary_Right;

    for (int i = 0; i <= MyStack->capacity; i++)
        MyStack->data[i] = POISON;
    
    Error = Stack_Verificator(MyStack);

    if (Error != Stack_Errno::Stack_No_Err)
        Return_Err();

    STACK_DUMP(MyStack, Error);
    
    return (Stack_Errno) Error;
}

//---------------------------------------------------------------------//

Stack_Errno Stack_Push(Stack_Info * const MyStack, const int value)
{
    assert(MyStack);

    if (MyStack->size >= MyStack->capacity)
        if (Stack_Upsize(MyStack) == Stack_Errno::Stack_Alloc_Err)
            return Stack_Errno::Stack_Alloc_Err;
    
    MyStack->data[MyStack->size++] = value; 

    return Stack_Errno::Stack_No_Err;
}

//---------------------------------------------------------------------//

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

//---------------------------------------------------------------------//

Stack_Errno Stack_Upsize(Stack_Info * MyStack)
{
    assert(MyStack);

    printf("Need to increase the capacity: The vlaue of capacity is %d\n", MyStack->capacity);
    
    MyStack->capacity *= UP_MULTIPLIER;

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

//---------------------------------------------------------------------//

Stack_Errno Stack_Lowsize(Stack_Info * MyStack)
{
    assert(MyStack);

    printf("Need to decrease the capacity: The vlaue of capacity is %d\n", MyStack->capacity);

    MyStack->capacity = MyStack->capacity / UP_MULTIPLIER + 1; //think bout it

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

//---------------------------------------------------------------------//


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

//---------------------------------------------------------------------//

int Is_Empty(Stack_Info * const MyStack)
{
    return MyStack->size == 0;
}

//---------------------------------------------------------------------//

void Stack_Dtor(Stack_Info * MyStack)
{
    assert(MyStack);

    free(MyStack->data);

    MyStack->data = nullptr;

    fclose(MyStack->logfile); // 
}

//---------------------------------------------------------------------//

void Stack_Dump(Stack_Info * const MyStack, Stack_Errno Error, const char * File, const char * Function, int Line)
{
    assert(MyStack);

    fprintf(MyStack->logfile, "//---------------------------------------------------------------------//\n");

    fprintf(MyStack->logfile, "Stack_Dump was called in file %s\nIn function %s\nIn line %d\n\n", File, Function, Line);

    if (Error == Stack_Errno::Stack_No_Err)
    {
        fprintf(MyStack->logfile, "No errors: the status of stack is OK\nThe value of left Canary is %u\n", MyStack->Canary_Left);

        fprintf(MyStack->logfile, "The capacity of stack is %d\nThe size of stack is %d\n", MyStack->capacity, MyStack->size);

        for (int i = 0; i <= MyStack->capacity + 1; i++)
        {
            fprintf(MyStack->logfile, "The %d elem of data is %d\n", i, MyStack->data[i]);
        }

        fprintf(MyStack->logfile, "The value of right Canary is %u\n", MyStack->Canary_Right);
    }

    else
    {
        Error_Meaning(MyStack, Error);
    }

    fprintf(MyStack->logfile, "//---------------------------------------------------------------------//\n\n");

}

//---------------------------------------------------------------------//
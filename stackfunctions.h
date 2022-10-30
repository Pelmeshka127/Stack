#ifndef STACK_LIBRARY_H_
#define STACK_LIBRARY_H_

#include "stackerrors.h"

// typedef int elem_t 
// murmur 

// hash - struct
// hash - data

const unsigned int CANARY_VALUE = 0xDEADBEEF;

const int DEF_CAPACITY = 5; // Def_capacity

const int UP_MULTIPLIER = 2; // 

const int LOW_MULTIPLIER = 3;

const int POISON = 0;

#define STACK_DUMP(MyStack, Error) \
Stack_Dump(MyStack, Error, __FILE__, __PRETTY_FUNCTION__, __LINE__)

struct Stack_Info // Stack
{
    unsigned int Canary_Left = CANARY_VALUE; // canary_left

    int capacity = 0;
    int size = 0;
    int * data = (int *) CANARY_VALUE;

    #ifdef STACK_DEBUG_INFO
        // Stack_debug_info stack_debug_info{};
    #endif 

    unsigned int Canary_Right = CANARY_VALUE;

    FILE * logfile = nullptr;
};

/// @brief 
/// @param MyStack 
/// @return 
Stack_Errno Stack_Verificator(Stack_Info * const MyStack);

/// @brief Function initializes Stack
/// @param MyStack is ptr on struct Stack_Info
/// @param capacity is the value that will be the capacity of Stack
/// @return Alloc_Err if calloc fell down or No_Error if it's OK
Stack_Errno Stack_Ctor(Stack_Info * const MyStack); // (..., const size_t capacity = DEFAULT_CAPACITY)

/// @brief Function pushes an element into the stack
/// @param MyStack is ptr on struct Stack_Info
/// @param n is an element that will be pushed into the stack
/// @return Alloc_Err if calloc fell down or No_Error if it's OK
Stack_Errno Stack_Push(Stack_Info * const MyStack, const int value);

/// @brief Function reduces the size of stack
/// @param MyStack is ptr on struct Stack_Info
/// @return Stack_Underflow if size = 0 or the last elem from stack if it's OK
Stack_Errno Stack_Pop(Stack_Info * const MyStack);

/// @brief Function increases the capacity of stack
/// @param MyStack is ptr on struct Stack_Info
/// @return Alloc_Err if realloc falls or 0 if it's OK
Stack_Errno Stack_Upsize(Stack_Info  * const MyStack);

/// @brief Function decreases the capacity of stack
/// @param MyStack is ptr on struct Stack_Info
/// @return Alloc_Err if realloc falls or 0 if it's OK
Stack_Errno Stack_Lowsize(Stack_Info * const MyStack);

/// @brief Returns the last elem of stack
/// @param MyStack is ptr on struct Stack_Info
/// @return Returns the last elem of stack
int Stack_Peep(Stack_Info * const MyStack);

/// @brief 
/// @param MyStack 
/// @return 
int Is_Empty(Stack_Info * const MyStack);

/// @brief Free allocated memory
/// @param MyStack is ptr on struct Stack_Info
void Stack_Dtor(Stack_Info * MyStack);

/// @brief Displays the status of stack
/// @param MyStack is ptr on struct Stack_Info
void Stack_Dump(Stack_Info * const MyStack, Stack_Errno Error, const char * File, const char * Function, int Line);

#endif
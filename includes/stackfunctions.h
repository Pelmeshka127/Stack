#ifndef STACK_LIBRARY_H_
#define STACK_LIBRARY_H_

// murmur 

// hash - struct
// hash - data

typedef int elem_t;

const size_t DEF_CAPACITY = 3;

const size_t MULTIPLIER = 2; 

const int UP_MODE = 1;

const int LOW_MODE = -1;

const elem_t POISON = 0;

#define HASH_PROTECT

#define CANARY_PROTECT

//---------------------------------------------------------------------------------------------//

#ifdef CANARY_PROTECT
    typedef unsigned long canary_t;
    const canary_t Canary_Value = 0xDEADBEEF;
#endif

//---------------------------------------------------------------------------------------------//

struct Stack
{
    #ifdef CANARY_PROTECT
        canary_t canary_left = Canary_Value;
    #endif

    size_t capacity = 0;
    size_t size = 0;

    #ifdef CANARY_PROTECT
        canary_t * canary_left_data = nullptr;
    #endif

    elem_t * data = nullptr;

    #ifdef CANARY_PROTECT
        canary_t * canary_right_data = nullptr;
    #endif

    #ifdef CANARY_PROTECT
        canary_t canary_right = Canary_Value;
    #endif
};

//---------------------------------------------------------------------------------------------//

/// @brief Function initializes Stack
/// @param My_Stack is ptr on struct Stack
/// @param capacity is the value that will be the capacity of Stack
/// @return Alloc_Err if calloc fell down or No_Error if it's OK
int Stack_Ctor(Stack * const My_Stack); // (..., const size_t capacity = DEFAULT_CAPACITY)

//---------------------------------------------------------------------------------------------//

/// @brief Function pushes an element into the stack
/// @param My_Stack is ptr on struct Stack
/// @param n is an element that will be pushed into the stack
/// @return Alloc_Err if calloc fell down or No_Error if it's OK
int Stack_Push(Stack * const My_Stack, const int value);

//---------------------------------------------------------------------------------------------//

/// @brief Function reduces the size of stack
/// @param My_Stack is ptr on struct Stack
/// @return Stack_Underflow if size = 0 or the last elem from stack if it's OK
int Stack_Pop(Stack * const My_Stack);

//---------------------------------------------------------------------------------------------//

/// @brief Free allocated memory
/// @param My_Stack is ptr on struct Stack
void Stack_Dtor(Stack * My_Stack);

#endif
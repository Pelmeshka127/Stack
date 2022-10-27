#ifndef STACK_LIBRARY_H_
#define STACK_LIBRARY_H_

struct Stack_Info
{
    int * data; //сделать места слева и справа для канареек
    int size;
    int capacity;
};

enum class Stack_Errno {
    Stack_No_Err = 0,
    Stack_Undef_Capacity = 1,
    Stack_Alloc_Err = 2,
    Stack_Underflow = 3,
    Stack_Canary_Failed = 4,
};

const int Def_Capacity = 5;

const int Multiplier = 2;

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

/// @brief Free allocated memory
/// @param MyStack is ptr on struct Stack_Info
void Stack_Dtor(Stack_Info * MyStack);

/// @brief Displays the status of stack
/// @param MyStack is ptr on struct Stack_Info
void Stack_Dump(const Stack_Info * MyStack);

#endif
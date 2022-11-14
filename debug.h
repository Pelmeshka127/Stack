#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#include "stack.h"

#define DUMP_DEBUG

/// @brief includes the codes of errors
enum Err_Codes {
    No_Err = 0xabcd0,
    
    Undef_Capacity = 0xabcd1,
    
    Alloc_Err = 0xabcd2,
    
    Underflow = 0xabcd3,
    
    Overflow = 0xabcd4,
    
    Canary_Left_Failed = 0xabcd5,
    
    Canary_Right_Failed = 0xabcd6,

	Canary_Data_Left_Failed = 0xabcd7,

	Canary_Data_Right_Failed = 0xabcd8,

	Logfile_Error = 0xabcd9,

    Stack_Hash_Err = 0xabcd10,

    Data_Hash_Err = 0xabcd11,
};

//---------------------------------------------------------------------------------------------//

/// @brief Funcrion verifies the errors in stack
/// @param My_Stack is ptr on struct Stack
/// @param Err_Code the code of error
/// @param File the pointer on file
/// @param Function the pointer on function
/// @param Line the pointer on line
void Stack_Verify(Stack * const My_Stack, const char * const File, const char * const Function, const int Line);

//---------------------------------------------------------------------------------------------//

/// @brief Function prints the description of error to stderr (or to the logfile if it includes)
/// @param My_Stack is ptr on struct Stack
/// @param Err_Code the code of error
/// @param File the pointer on file
/// @param Function the pointer of function
/// @param Line the pointer of line
void Stack_Print_Err(Stack * const My_Stack, const int Err_Code, const char * const File, const char * const Function, const int Line);

//---------------------------------------------------------------------------------------------//

/// @brief Function prints the status of stack
/// @param My_Stack is ptr on struct Stack
/// @param File the pointer on file
/// @param Function the pointer of function
/// @param Line the pointer of line
void Stack_Dump(Stack * const My_Stack, const char * const File, const char * const Function, const int Line);

//---------------------------------------------------------------------------------------------//

#define STACK_VERIFY(My_Stack) \
Stack_Verify(My_Stack, __FILE__, __PRETTY_FUNCTION__, __LINE__);

//---------------------------------------------------------------------------------------------//

#endif
#ifndef STACKERRORS_H_
#define STACKERRORS_H_

#include <stdio.h>

#include "stackfunctions.h"

const char RED[]   = "\x1b[31m";
const char GREEN[] = "\x1b[32m";
const char RESET[] = "\x1b[0m";

/// @brief Includes the codes of errors
enum Err_Codes {
    No_Err = 0,
    
    Undef_Capacity = 1,
    
    Alloc_Err = 2,
    
    Underflow = 3,
    
    Overflow = 4,
    
    Canary_Left_Failed = 5,
    
    Canary_Right_Failed = 6,

	Canary_Data_Left_Failed = 7,

	Canary_Data_Right_Failed = 8,

	Logfile_Error = 9,
};

#define DUMP_DEBUG

#define CANARY_DEBUG

#define HASH_DEBUG

//---------------------------------------------------------------------------------------------//

#define RETURN_ERR()                                                      \
fprintf(logfile, "Error in file %s\nin function %s\nin line %d\n",        \
        __FILE__, __PRETTY_FUNCTION__, __LINE__)  

//---------------------------------------------------------------------------------------------//

#ifdef DUMP_DEBUG
    #define STACK_DUMP(My_Stack)                                                                                                    \
    assert(My_Stack);                                                                                                               \
                                                                                                                                    \
    fprintf(logfile, "//---------------------------------------------------------------------------------------------//\n\n");      \
                                                                                                                                    \
    fprintf(logfile, "Stack_Dump was called in file %s\nIn function %s\nIn line %d\n\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);  \
                                                                                                                                    \
    fprintf(logfile, "Stack has an adress %p. It's adrees in decimal system is %d\n\n", My_Stack, My_Stack);                        \
                                                                                                                                    \
    fprintf(logfile, "The value of left Canary is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",                 \
    My_Stack->canary_left, &My_Stack->canary_left, &My_Stack->canary_left);                                                         \
                                                                                                                                    \
    fprintf(logfile, "The capacity of stack is %zu\nThe size of stack is %zu\n\n",                                                  \
    My_Stack->capacity, My_Stack->size);                                                                                            \
                                                                                                                                    \
    fprintf(logfile, "The value of canary_left_data is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",            \
    *My_Stack->canary_left_data, My_Stack->canary_left_data, My_Stack->canary_left_data);                                           \
                                                                                                                                    \
    for (int i = 0; i < (elem_t) My_Stack->capacity; i++)                                                                           \
    {                                                                                                                               \
        fprintf(logfile, "The [%d] elem of data is %d. It's adress is %p. It's adrees in decimal system is %d\n",                   \
        i, My_Stack->data[i], &My_Stack->data[i], &My_Stack->data[i]);                                                              \
    }                                                                                                                               \
                                                                                                                                    \
    fprintf(logfile, "\n");                                                                                                         \
                                                                                                                                    \
    fprintf(logfile, "The value of canary_right_data is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",           \
    *(My_Stack->canary_right_data), My_Stack->canary_right_data, My_Stack->canary_right_data);                                      \
                                                                                                                                    \
    fprintf(logfile, "The value of right Canary is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",                \
    My_Stack->canary_right, &My_Stack->canary_right_data, &My_Stack->canary_right_data);                                            \
                                                                                                                                    \
    Stack_Verificator(My_Stack);                                                                                                   
#endif

//---------------------------------------------------------------------------------------------//

#ifdef CANARY_DEBUG
    #define CANARY_CHECK(My_Stack)                                                                                                  \
    assert(My_Stack);                                                                                                               \
                                                                                                                                    \
    int Error = No_Err;                                                                                                             \
                                                                                                                                    \
    if (My_Stack->canary_left != Canary_Value)                                                                                      \
    {                                                                                                                               \
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n",                                                               \
        __FILE__, __PRETTY_FUNCTION__, __LINE__);                                                                                   \
        Error = Canary_Left_Failed;                                                                                                 \
    }                                                                                                                               \
    if (My_Stack->canary_right != Canary_Value)                                                                                     \
    {                                                                                                                               \
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n",                                                               \
        __FILE__, __PRETTY_FUNCTION__, __LINE__);                                                                                   \
        Error = Canary_Right_Failed;                                                                                                \
    }                                                                                                                               \
    if (*My_Stack->canary_left_data != Canary_Value)                                                                                \
    {                                                                                                                               \
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n",                                                               \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, Canary_Value);                                                                     \
        Error = Canary_Data_Left_Failed;                                                                                            \
    }                                                                                                                               \
    if (*(My_Stack->canary_right_data) != Canary_Value)                                                                             \
    {                                                                                                                               \
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n",                                                               \
        __FILE__, __PRETTY_FUNCTION__, __LINE__);                                                                                   \
        Error = Canary_Data_Right_Failed;                                                                                           \
    }                                                                                                                               \
                                                                                                                                    \
    assert(Error == No_Err);                                                                                                        \
                                                                                                                                    \
    fprintf(logfile, "CANARY_CHECK IS OK\n\n");                                                                                     \
                                                                                                                                    \
    fprintf(logfile, "//---------------------------------------------------------------------------------------------//\n\n"); 
                                                                                                                                
#endif

//---------------------------------------------------------------------------------------------//

/// @brief Function checks the status of Stack
/// @param My_Stack is ptr on struct Stack
void Stack_Verificator(Stack * const My_Stack);

//---------------------------------------------------------------------------------------------//

/// @brief Function make error strings colorful
/// @param str is a string that will become colorful
/// @param color is a kind of color
void Coloured_Print(FILE * file, const char* str, const char* color);

//---------------------------------------------------------------------------------------------//

/// @brief Function write to the logfile the code of error
/// @param My_Stack is ptr on struct Stack
/// @param Error error
void Error_Meaning(Stack * My_Stack, int Error, FILE * logfile);

//---------------------------------------------------------------------------------------------//

#endif



/*//---------------------------------------------------------------------------------------------//

/// @brief Displays the status of stack
/// @param My_Stack is ptr on struct
void Stack_Dump(Stack * const My_Stack, const char * File, const char * Function, int Line, FILE * logfile);

//---------------------------------------------------------------------------------------------//

#define STACK_DUMP(My_Stack, File) \
Stack_Dump(My_Stack, __FILE__, __PRETTY_FUNCTION__, __LINE__, File);*

//---------------------------------------------------------------------------------------------//*/




/*/// @brief 
/// @param My_Stack 
/// @param logfile 
/// @return 
int Canary_Check(Stack * My_Stack, const char * File, const char * Function, int Line, FILE * logfile);

//---------------------------------------------------------------------------------------------//

#define CANARY_CHECK(My_Stack, File) \
assert(Canary_Check(My_Stack, __FILE__, __PRETTY_FUNCTION__, __LINE__, File) == No_Err);*/
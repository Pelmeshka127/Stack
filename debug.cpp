#include <stdio.h>
#include <assert.h>

#include "debug.h"
#include "log.h"
#include "hash.h"
#include "stack.h"

void Stack_Verify(Stack * const My_Stack, const char * const File, const char * const Function, const int Line)
{
    assert(My_Stack);

#ifdef DUMP_DEBUG
    Stack_Dump(My_Stack, File, Function, Line);
#endif

    if (My_Stack->size < 0)
        Stack_Print_Err(My_Stack, Underflow, File, Function, Line);

    if (My_Stack->size > My_Stack->capacity)
        Stack_Print_Err(My_Stack, Overflow,  File, Function, Line);

    if (My_Stack->data == nullptr)
        Stack_Print_Err(My_Stack, Alloc_Err, File, Function, Line);

#ifdef CANARY_PROTECT
    if (My_Stack->canary_left  != Canary_Value)
        Stack_Print_Err(My_Stack, Canary_Left_Failed,  File, Function, Line);
    
    if (My_Stack->canary_right != Canary_Value)
        Stack_Print_Err(My_Stack, Canary_Right_Failed, File, Function, Line);
    
    if (*My_Stack->canary_left_data  != Canary_Value)
        Stack_Print_Err(My_Stack, Canary_Data_Left_Failed,  File, Function, Line);
    
    if (*My_Stack->canary_right_data != Canary_Value)
        Stack_Print_Err(My_Stack, Canary_Data_Right_Failed, File, Function, Line);
#endif
}

//---------------------------------------------------------------------------------------------//

void Stack_Print_Err(Stack * const My_Stack, const int Err_Code, const char * const File, const char * const Function, const int Line)
{
    assert(My_Stack);

    if (Err_Code == Underflow)
        fprintf(stderr, "INCORRECT VALUE OF CAPACITY : %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                My_Stack->capacity, File, Function, Line);

    if (Err_Code == Overflow)
        fprintf(stderr, "THE VALUE OF SIZE %zu IS BIGGER THAN VALUE OF CAPACITY %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n",
                My_Stack->size, My_Stack->capacity, File, Function, Line);
    
    if (Err_Code == Alloc_Err)
        fprintf(stderr, "MEMEORY ALLOCATION WAS FAILED IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n",
                File, Function, Line);

#ifdef CANARY_PROTECT
    if (Err_Code == Canary_Left_Failed)
        fprintf(stderr, "THE VALUE OF LEFT CANARY IN My_Stack WAS CHANGED FROM %zu TO %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, My_Stack->canary_left, File, Function, Line);

    if (Err_Code == Canary_Right_Failed) 
        fprintf(stderr, "THE VALUE OF RIGHT CANARY IN My_Stack WAS CHANGED FROM %zu TO: %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, My_Stack->canary_right, File, Function, Line);

    if (Err_Code == Canary_Data_Left_Failed)
        fprintf(stderr, "THE VALUE OF CANARY IN THE START OF DATA WAS CHANGED FROM %zu TO: %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, *My_Stack->canary_left_data, File, Function, Line);

    if (Err_Code == Canary_Data_Right_Failed)
        fprintf(stderr, "THE VALUE OF CANARY IN THE END OF DATA WAS CHANGED FROM %zu TO: %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, *My_Stack->canary_right_data, File, Function, Line);
#endif

#ifdef USE_LOGS
    Logfile_Stack_Print_Err(My_Stack, Err_Code, File, Function, Line);
#endif
}

//---------------------------------------------------------------------------------------------//

void Stack_Dump(Stack * const My_Stack, const char * const File, const char * const Function, const int Line)
{
    assert(My_Stack);

#ifdef USE_LOGS
    Logfile_Stack_Dump(My_Stack, File, Function, Line);
#else
    printf("The capasity of stack is %zu. The size of stack is %zu\n", My_Stack->capacity, My_Stack->size);
    
    for (int i = 0; i < My_Stack->capacity; i++)
        printf("The [%d] elem of data is %d.\n", i, My_Stack->data[i]);
#endif
}

//---------------------------------------------------------------------------------------------////---------------------------------------------------------------------------------------------//

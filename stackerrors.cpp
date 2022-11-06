#include <stdio.h>
#include <assert.h>

#include "stackerrors.h"
#include "log.h"


//---------------------------------------------------------------------------------------------//

void Coloured_Print(FILE * file, const char * str, const char * color) 
{
    fprintf(file, "%s", color);
    fprintf(file, "%s", str);
    fprintf(file, "%s", RESET);
}

//---------------------------------------------------------------------------------------------//

void Stack_Verificator(Stack * const My_Stack)
{
    assert(My_Stack);

    if (My_Stack->capacity <= 0)
        fprintf(logfile, "INCORRECT VALUE OF CAPACITY: %zu\n\n", My_Stack->capacity);
    
    if (My_Stack->size < 0)
        fprintf(logfile, "THE VALUE OF SIZE OF STACK IS %zu\n\n", My_Stack->size);

    if (My_Stack->size > My_Stack->capacity)
        fprintf(logfile, "THE VALUE OF SIZE %zu IS BIGGER THAN VALUE OF CAPACITY %zu\n\n", My_Stack->size, My_Stack->capacity);

    if (My_Stack->data == nullptr)
        fprintf(logfile, "MEMEORY ALLOCATION WAS FAILED\n\n");

    if (My_Stack->canary_left != Canary_Value)
    {
        fprintf(logfile, "THE VALUE OF LEFT CANARY IN My_Stack WAS CHANGED FROM %zu TO %zu\n\n", 
        Canary_Value, My_Stack->canary_left);
    }

    if (My_Stack->canary_right != Canary_Value) 
    {
        fprintf(logfile, "THE VALUE OF RIGHT CANARY IN My_Stack WAS CHANGED FROM %zu TO: %zu\n\n", 
        Canary_Value, My_Stack->canary_right);
    }

    if (*My_Stack->canary_left_data != Canary_Value)
    {
        fprintf(logfile, "THE VALUE OF CANARY IN THE START OF DATA WAS CHANGED FROM %zu TO: %zu\n\n", 
        Canary_Value, *My_Stack->canary_left_data);
    }

    if (*(My_Stack->canary_right_data) != Canary_Value)
    {
        fprintf(logfile, "THE VALUE OF CANARY IN THE END OF DATA WAS CHANGED FROM %zu TO: %zu \n\n", 
        Canary_Value, *My_Stack->canary_right_data);
    }

    else
        fprintf(logfile, "STACK AFTER VERIFICATION OK\n\n");
}

//---------------------------------------------------------------------------------------------//

void Error_Meaning(Stack * My_Stack, int Error, FILE * logfile)
{
    assert(My_Stack);

    switch (Error)
    {
        case Undef_Capacity:
            fprintf(logfile, "INCORRECT VALUE OF CAPACITY: %zu\n", My_Stack->capacity);
            break;
        
        case Underflow:
            fprintf(logfile, "THE VALUE OF SIZE OF STACK IS %zu\n", My_Stack->size);
            break;

        case Overflow:
            fprintf(logfile, "THE VALUE OF SIZE %zu IS BIGGER THAN VALUE OF CAPACITY %zu\n", 
            My_Stack->size, My_Stack->capacity);
            break;
        
        case Alloc_Err:
            fprintf(logfile, "MEMEORY ALLOCATION WAS FAILED\n");
            break;
    
        case Canary_Left_Failed:
            fprintf(logfile, "THE VALUE OF LEFT CANARY IN My_Stack WAS CHANGED FROM %zu TO %zu\n", 
            Canary_Value, My_Stack->canary_left);
            break;
        
        case Canary_Right_Failed:
            fprintf(logfile, "THE VALUE OF RIGHT CANARY IN My_Stack WAS CHANGED FROM %zu TO %zu\n", 
            Canary_Value, My_Stack->canary_right);
            break;

        case Canary_Data_Left_Failed:
            fprintf(logfile, "THE VALUE OF CANARY IN THE START OF DATA WAS CHANGED FROM %zu TO: %zu\n", 
            Canary_Value, *My_Stack->canary_left_data);
            break;

        case Canary_Data_Right_Failed:
            fprintf(logfile, "THE VALUE OF CANARY IN THE END OF DATA WAS CHANGED FROM %zu TO: %zu\n", 
            Canary_Value, *My_Stack->canary_right_data);
            break;

        case Logfile_Error:
            fprintf(logfile, "OPPENING OF LOGFILE WAS FAILED\n");
            break;

        default:
            fprintf(logfile, "THERE'S NO ERRORS. STACK IS OK\n");
    }
}

//---------------------------------------------------------------------------------------------//










/*
int Canary_Check(Stack * My_Stack,  const char * File, const char * Function, int Line, FILE * logfile)
{
    assert(My_Stack);

    if (My_Stack->canary_left != Canary_Value)
    {
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n", 
        File, Function, Line);
        return Canary_Left_Failed;
    }
    if (My_Stack->canary_right != Canary_Value)
    { 
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n", 
        File, Function, Line);
        return Canary_Right_Failed;
    }
    if (*My_Stack->canary_left_data != Canary_Value)
    {
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n", 
        File, Function, Line, Canary_Value);
        return Canary_Data_Left_Failed;
    }
    if (*(My_Stack->canary_right_data) != Canary_Value)
    {
        fprintf(logfile, "IN FILE %s. IN FUNCTION %s. IN LINE %d.\n", 
        File, Function, Line);
        return Canary_Data_Right_Failed;
    }

    fprintf(logfile, "CANARY_CHECK IS OK\n\n");

    fprintf(logfile, "//---------------------------------------------------------------------------------------------//\n\n");

    return No_Err; 
}




void Stack_Dump(Stack * const My_Stack, const char * File, const char * Function, int Line, FILE * logfile)
{ 
    assert(My_Stack);

    fprintf(logfile, "//---------------------------------------------------------------------------------------------//\n\n");

    fprintf(logfile, "Stack_Dump was called in file %s\nIn function %s\nIn line %d\n\n", File, Function, Line);

    fprintf(logfile, "Stack has an adress %p. It's adrees in decimal system is %d\n\n", My_Stack, My_Stack);

    fprintf(logfile, "The value of left Canary is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",
    My_Stack->canary_left, &My_Stack->canary_left, &My_Stack->canary_left);

    fprintf(logfile, "The capacity of stack is %zu\nThe size of stack is %zu\n\n",
    My_Stack->capacity, My_Stack->size);

    fprintf(logfile, "The value of canary_left_data is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",
    *My_Stack->canary_left_data, My_Stack->canary_left_data, My_Stack->canary_left_data);

    for (int i = 0; i < (elem_t) My_Stack->capacity; i++)
    {
        fprintf(logfile, "The [%d] elem of data is %d. It's adress is %p. It's adrees in decimal system is %d\n",
        i, My_Stack->data[i], &My_Stack->data[i], &My_Stack->data[i]);
    }

    fprintf(logfile, "\n");

    fprintf(logfile, "The value of canary_right_data is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",
    *(My_Stack->canary_right_data), My_Stack->canary_right_data, My_Stack->canary_right_data);

    fprintf(logfile, "The value of right Canary is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",
    My_Stack->canary_right, &My_Stack->canary_right_data, &My_Stack->canary_right_data);

    Stack_Verificator(My_Stack);
}*/


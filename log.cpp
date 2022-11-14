#include <stdio.h>
#include <assert.h>

#include "debug.h"
#include "log.h"
#include "stack.h"

static FILE * logfile = nullptr;

static const char * const LOGFILE_NAME = "logfile.txt";

//---------------------------------------------------------------------------------------------//

int Logfile_Ctor()
{
    logfile = fopen(LOGFILE_NAME, "a");

    if (logfile == nullptr)
        return Logfile_Error;

    return No_Err;
}

//---------------------------------------------------------------------------------------------//

int Logfile_Dtor()
{
    if (fclose(logfile) == 0)
        return Logfile_Error;

    return No_Err;
}

//---------------------------------------------------------------------------------------------//

void Logfile_Stack_Print_Err(Stack * const My_Stack, const int Err_Code, const char * const File, const char * const Function, const int Line)
{
    assert(My_Stack);
    assert(logfile);

    if (Err_Code == Underflow)
        fprintf(logfile, "INCORRECT VALUE OF CAPACITY : %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                My_Stack->capacity, File, Function, Line);

    if (Err_Code == Overflow)
        fprintf(logfile, "THE VALUE OF SIZE %zu IS BIGGER THAN VALUE OF CAPACITY %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n",
                My_Stack->size, My_Stack->capacity, File, Function, Line);
    
    if (Err_Code == Alloc_Err)
        fprintf(logfile,  "MEMEORY ALLOCATION WAS FAILED IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n",
                File, Function, Line);

#ifdef CANARY_PROTECT
    if (Err_Code == Canary_Left_Failed) 
        fprintf(logfile, "THE VALUE OF LEFT CANARY IN My_Stack WAS CHANGED FROM %zu TO %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, My_Stack->canary_left, File, Function, Line);

    if (Err_Code == Canary_Right_Failed) 
        fprintf(logfile, "THE VALUE OF RIGHT CANARY IN My_Stack WAS CHANGED FROM %zu TO: %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, My_Stack->canary_right, File, Function, Line);

    if (Err_Code == Canary_Data_Left_Failed)
        fprintf(logfile, "THE VALUE OF CANARY IN THE START OF DATA WAS CHANGED FROM %zu TO: %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, *My_Stack->canary_left_data, File, Function, Line);

    if (Err_Code == Canary_Data_Right_Failed)
        fprintf(logfile, "THE VALUE OF CANARY IN THE END OF DATA WAS CHANGED FROM %zu TO: %zu IN FILE %s; IN FUNCTION %s; IN LINE %d\n\n", 
                Canary_Value, *My_Stack->canary_right_data, File, Function, Line);
#endif
}

//---------------------------------------------------------------------------------------------//

void Logfile_Hash_Err(Stack * const My_Stack, hash_t Cur_Hash_Value, const int Error, const char * const File, const char * const Function, const int Line)
{
    if (Error == Stack_Hash_Err)
        fprintf(logfile, "ERROR IN FUNCION %s, IN FILE %s, IN LINE %d.\nTHE VALUE OF STACK_HASH WAS CHANGED FROM %zu TO %zu\n\n",
                        Function, File, Line, Cur_Hash_Value, My_Stack->stack_hash);
    
    if (Error == Data_Hash_Err)
        fprintf(logfile, "ERROR IN FUNCION %s, IN FILE %s, IN LINE %d.\nTHE VALUE OF DATA_HASH WAS CHANGED FROM %zu TO %zu\n\n",
                        Function, File, Line, Cur_Hash_Value, My_Stack->data_hash);
}


//---------------------------------------------------------------------------------------------//

void Logfile_Stack_Dump(Stack * const My_Stack, const char * const File, const char * const Function, const int Line)
{
    assert(My_Stack);
    assert(logfile);

    fprintf(logfile, "//---------------------------------------------------------------------------------------------//\n\n");
                                                                                                                                    
    fprintf(logfile, "Stack_Dump was called at %s\nAt %s\nIn file %s\nIn function %s\nIn line %d\n\n", __TIME__, __DATE__, File, Function, Line);  
                                                                                                                                    
    fprintf(logfile, "Stack has an adress %p. It's adrees in decimal system is %d\n\n", My_Stack, My_Stack);                        

#ifdef CANARY_PROTECT                                                                                                            
    fprintf(logfile, "The value of left Canary is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",           
            My_Stack->canary_left, &My_Stack->canary_left, &My_Stack->canary_left);                                                   
#endif

    fprintf(logfile, "The capacity of stack is %zu\nThe size of stack is %zu\n\n",                                            
            My_Stack->capacity, My_Stack->size);                                                                                      

#ifdef CANARY_PROTECT                                                                                                                  
    fprintf(logfile, "The value of canary_left_data is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",      
            *My_Stack->canary_left_data, My_Stack->canary_left_data, My_Stack->canary_left_data);                                     
#endif
                                                                                                             
    for (int i = 0; i < (elem_t) My_Stack->capacity; i++)                                                                     
    {                                                                                                                         
        fprintf(logfile, "The [%d] elem of data is %d. It's adress is %p. It's adrees in decimal system is %d\n",             
                i, My_Stack->data[i], &My_Stack->data[i], &My_Stack->data[i]);                                                        
    }                                                                                                                         
                                                                                                                        
    fprintf(logfile, "\n");                                                                                                   

#ifdef CANARY_PROTECT                                                                                                                      
    fprintf(logfile, "The value of canary_right_data is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",     
            *(My_Stack->canary_right_data), My_Stack->canary_right_data, My_Stack->canary_right_data    );                                
                                                                                                                        
    fprintf(logfile, "The value of right Canary is %zu. It's adress is %p. It's adrees in decimal system is %d\n\n",          
            My_Stack->canary_right, &My_Stack->canary_right_data, &My_Stack->canary_right_data);     
#endif

#ifdef HASH_PROTECT
    fprintf(logfile, "The value of stack_hash is %zu. It's adress is %p. It's adress in decimal system is %d\n\n",
            My_Stack->stack_hash, &My_Stack->stack_hash, &My_Stack->stack_hash);
    
    fprintf(logfile, "The value of data_hash is %zu. It's adress is %p. It's adress in decimal system is %d\n\n",
            My_Stack->data_hash, &My_Stack->data_hash, &My_Stack->data_hash);
#endif
}

//---------------------------------------------------------------------------------------------//
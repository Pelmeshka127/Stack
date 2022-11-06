#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "stackfunctions.h"
#include "stackerrors.h"
#include "log.h"

//---------------------------------------------------------------------------------------------//

/// @brief Function reallocate dynamic memory of stack
/// @param My_Stack is ptr on struct Stack
/// @return Alloc_Err if reallocation failed an No_Err if it's OK
static int Stack_Resize(Stack * const My_Stack, int mode);

//---------------------------------------------------------------------------------------------//

/// @brief Function checks is empty Stack
/// @param My_Stack is ptr on struct Stack
/// @return If Stack is overflow - 1, else - 0
static int Is_Empty(Stack * const My_Stack);

//---------------------------------------------------------------------------------------------//


int Stack_Ctor(Stack * const My_Stack)
{
    assert(My_Stack);

    if (Logfile_Ctor() == Logfile_Error)
        return Logfile_Error;

    My_Stack->capacity = DEF_CAPACITY;

    if (My_Stack->capacity <= 0)
    {
        RETURN_ERR();

        Error_Meaning(My_Stack, Undef_Capacity, logfile);
        
        Coloured_Print(stderr, "INCORRECT VALUE OF CAPACITY\n", RED);
        
        fprintf(stderr, "%zu\n", My_Stack->capacity);
        
        return Undef_Capacity;
    }

    My_Stack->size = 0;

    size_t Mem_Value = 2 * sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t) + 
    (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t);

    char * Alloc_Mem = (char *)calloc (Mem_Value, sizeof(char));

    if (Alloc_Mem == nullptr)
    {
        RETURN_ERR();

        Error_Meaning(My_Stack, Alloc_Err, logfile);
        
        Coloured_Print(stderr, "MEMEORY ALLOCATION WAS FAILED\n", RED);
        
        return Alloc_Err;
    }
    
    My_Stack->canary_left_data = (canary_t *) (Alloc_Mem);

    My_Stack->data = (elem_t *) (Alloc_Mem + sizeof(canary_t));

    if (My_Stack->capacity % sizeof(canary_t) == 0)
        My_Stack->canary_right_data = (canary_t *) (Alloc_Mem + sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t));
    else
        My_Stack->canary_right_data = (canary_t *) (Alloc_Mem + sizeof(canary_t) +
        My_Stack->capacity * sizeof(elem_t) + (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t));

    *(My_Stack->canary_left_data) = Canary_Value;

    *(My_Stack->canary_right_data) = Canary_Value;

    for (int i = 0; i < (elem_t) My_Stack->capacity; i++)
        My_Stack->data[i] = POISON;

#ifdef DUMP_DEBUG
    STACK_DUMP(My_Stack);
#endif

#ifdef CANARY_DEBUG
    CANARY_CHECK(My_Stack)
#endif

    return No_Err;
}

//---------------------------------------------------------------------------------------------//

int Stack_Push(Stack * const My_Stack, const int value)
{
    assert(My_Stack);

    if (My_Stack->size == My_Stack->capacity)
        if (Stack_Resize(My_Stack, UP_MODE) == Alloc_Err)
        {
            Coloured_Print(stderr, "MEMEORY ALLOCATION WAS FAILED\n", RED);
            
            Error_Meaning(My_Stack, Alloc_Err, logfile);

            free(My_Stack->canary_left_data);

            RETURN_ERR();
            
            return Alloc_Err;
        }
            
    My_Stack->data[My_Stack->size++] = value; 

#ifdef DUMP_DEBUG
    STACK_DUMP(My_Stack);
#endif

#ifdef CANARY_DEBUG
    CANARY_CHECK(My_Stack)
#endif

    return No_Err;
}

//---------------------------------------------------------------------------------------------//

int Stack_Pop(Stack * const My_Stack)
{
    assert(My_Stack);

    if (Is_Empty(My_Stack))
    {
        Coloured_Print(stderr, "STACK IS UNDERFLOW\n", RED);

        Error_Meaning(My_Stack, Underflow, logfile);

        free(My_Stack->canary_left_data);

        RETURN_ERR();

        return Underflow;
    }

    if (4 * My_Stack->size <= My_Stack->capacity)
        if (Stack_Resize(My_Stack, LOW_MODE) == Alloc_Err)
        {
            Coloured_Print(stderr, "MEMEORY ALLOCATION WAS FAILED\n", RED);
            
            Error_Meaning(My_Stack, Alloc_Err, logfile);
            
            free(My_Stack->canary_left_data);

            RETURN_ERR();
            
            return Alloc_Err;
        }

    My_Stack->data[--My_Stack->size] = POISON;

#ifdef DUMP_DEBUG
    STACK_DUMP(My_Stack);
#endif

#ifdef CANARY_DEBUG
    CANARY_CHECK(My_Stack)
#endif

    return No_Err;
}

//---------------------------------------------------------------------------------------------//

int Stack_Resize(Stack * My_Stack, int mode)
{
    assert(My_Stack);

    if (mode == UP_MODE)
    {
        My_Stack->capacity *= MULTIPLIER;
        fprintf(logfile, "Increasing the capacity of Stack to %zu\n\n", My_Stack->capacity);
    }
    
    else if (mode == LOW_MODE)
    {
        My_Stack->capacity /= MULTIPLIER;
        fprintf(logfile, "Decreasing the capacity of Stack to %zu \n\n", My_Stack->capacity);
    }

    else
    {
        fprintf(logfile, "INCORRECT VALUE OF MODE %d. STACK CAN'T BE RESIZED\n", mode);

        RETURN_ERR();

        return Alloc_Err;
    }

    size_t Add_Mem_Value = 2 * sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t) +
    (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t);

    char * Add_Alloc_Mem = (char *)realloc (My_Stack->canary_left_data, Add_Mem_Value * sizeof(canary_t));

    if (Add_Alloc_Mem == nullptr)
    {
        Coloured_Print(stderr, "FAILED ALLOCATION OF MEMORY\n", RED);

        Error_Meaning(My_Stack, Alloc_Err, logfile);

        free(My_Stack->canary_left_data);

        RETURN_ERR();

        return Alloc_Err;
    }

    My_Stack->canary_left_data = (canary_t *) (Add_Alloc_Mem);

    My_Stack->data = (elem_t *) (Add_Alloc_Mem + sizeof(canary_t));

    if (My_Stack->capacity % sizeof(canary_t) == 0)
        My_Stack->canary_right_data = (canary_t *) (Add_Alloc_Mem + My_Stack->capacity * sizeof(elem_t) + sizeof(canary_t));
    else
        My_Stack->canary_right_data = (canary_t *) (Add_Alloc_Mem + My_Stack->capacity * sizeof(elem_t) + sizeof(canary_t) +
        (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t));

    *(My_Stack->canary_right_data) = Canary_Value;

    if (mode == UP_MODE)
        for (int i = My_Stack->capacity / MULTIPLIER; i < My_Stack->capacity; i++)
            My_Stack->data[i] = POISON;
            
#ifdef DUMP_DEBUG
    STACK_DUMP(My_Stack);
#endif

#ifdef CANARY_DEBUG
    CANARY_CHECK(My_Stack)
#endif

    return No_Err;

}

//---------------------------------------------------------------------------------------------//

int Is_Empty(Stack * const My_Stack)
{
    return My_Stack->size <= 0;
}

//---------------------------------------------------------------------------------------------//

void Stack_Dtor(Stack * My_Stack)
{
    assert(My_Stack);

    free(My_Stack->canary_left_data);

    My_Stack->data = nullptr;

    Logdile_Dtor();
}
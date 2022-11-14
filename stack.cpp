#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "stack.h"
#include "hash.h"
#include "debug.h"
#include "log.h"

//---------------------------------------------------------------------------------------------//

/// @brief Function resizes the data of Stack
/// @param My_Stack is ptr on struct Stack
/// @return Alloc_Err if resizing failed an No_Err if it's OK
static int Stack_Resize(Stack * const My_Stack, const int mode);

//---------------------------------------------------------------------------------------------//

/// @brief Function reallocates the memory for data
/// @param My_Stack is ptr on struct Stack
/// @return Alloc_errif reallocation failed an No_Err if it's OK
static char * Stack_Recalloc(Stack * const My_Stack);

//---------------------------------------------------------------------------------------------//

/// @brief Function sets canaries after the allocation of memeory
/// @param My_Stack is ptr on struct Stack
static void Canary_Recalloc(Stack * const My_Stack, const char * const Add_Alloc_Mem);

//---------------------------------------------------------------------------------------------//

/// @brief Function checks is empty Stack
/// @param My_Stack is ptr on struct Stack
/// @return If Stack is overflow - 1, else - 0
static int Is_Empty(Stack * const My_Stack);

//---------------------------------------------------------------------------------------------//


int Stack_Ctor(Stack * const My_Stack)
{
    assert(My_Stack);

    My_Stack->capacity = DEF_CAPACITY;

    My_Stack->size = 0;

#ifdef CANARY_PROTECT
    size_t Mem_Value = 2 * sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t) + 
    (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t);
#else
    size_t Mem_Value = My_Stack->capacity * sizeof(elem_t);
#endif

    char * Alloc_Mem = (char *)calloc (Mem_Value, sizeof(char));

    if (Alloc_Mem == nullptr)
    {
        fprintf(stderr, "THE INITIALIZATION OF MEMORY FAILED\n");
        return Alloc_Err;
    }

#ifdef CANARY_PROTECT
    My_Stack->canary_left_data = (canary_t *) (Alloc_Mem);

    My_Stack->data = (elem_t *) (Alloc_Mem + sizeof(canary_t));

    if (My_Stack->capacity % sizeof(canary_t) == 0)
        My_Stack->canary_right_data = (canary_t *) (Alloc_Mem + sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t));
    else
        My_Stack->canary_right_data = (canary_t *) (Alloc_Mem + sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t) + 
        (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t));

    *(My_Stack->canary_left_data) = Canary_Value;

    *(My_Stack->canary_right_data) = Canary_Value;
#else
    My_Stack->data = (elem_t *) (Alloc_Mem);
#endif

    for (int i = 0; i < (elem_t) My_Stack->capacity; i++)
        My_Stack->data[i] = POISON;

#ifdef HASH_PROTECT
    My_Stack->stack_hash = Calculate_Hash(My_Stack, sizeof(My_Stack));
#ifdef CANARY_PROTECT
    My_Stack->data_hash = Calculate_Hash(My_Stack->canary_left_data, 2 * sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t));
#else
    My_Stack->data_hash = Calculate_Hash(My_Stack->data, sizeof(elem_t));
#endif
#endif

    STACK_VERIFY(My_Stack);

#ifdef HASH_PROTECT
    CHECK_HASH(My_Stack);
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
#ifdef CANARY_PROTECT
            free(My_Stack->canary_left_data);
#else
            free(My_Stack->data);
#endif
            return Alloc_Err;
        }
            
    My_Stack->data[My_Stack->size++] = value; 

    STACK_VERIFY(My_Stack);

#ifdef HASH_PROTECT
    CHECK_HASH(My_Stack);
#endif

    return No_Err;
}

//---------------------------------------------------------------------------------------------//

int Stack_Pop(Stack * const My_Stack)
{
    assert(My_Stack);

    if (Is_Empty(My_Stack))
    {
#ifdef CANARY_PROTECT
        free(My_Stack->canary_left_data);
#else
        free(My_Stack->data);
#endif
        return Underflow;
    }

    if (MIN_SIZE * My_Stack->size <= My_Stack->capacity)
        if (Stack_Resize(My_Stack, LOW_MODE) == Alloc_Err)
        {
#ifdef CANARY_PROTECT
            free(My_Stack->canary_left_data);
#else 
            free(My_Stack->data);
#endif
            return Alloc_Err;
        }

    elem_t Popping_Elem = My_Stack->data[My_Stack->size - 1];

    My_Stack->data[--My_Stack->size] = POISON;

    STACK_VERIFY(My_Stack);

#ifdef HASH_PROTECT
    CHECK_HASH(My_Stack);
#endif

    return Popping_Elem;
}

//---------------------------------------------------------------------------------------------//

static int Stack_Resize(Stack * My_Stack, int mode)
{
    assert(My_Stack);

    if (mode == UP_MODE)
        My_Stack->capacity *= MULTIPLIER;
    
    else if (mode == LOW_MODE)
        My_Stack->capacity /= MULTIPLIER;

    else
    {
        fprintf(stderr, "INCORRECT VALUE OF MODE %d. STACK CAN'T BE RESIZED\n", mode);
        return Alloc_Err;
    }

    char * Add_Alloc_Mem = Stack_Recalloc(My_Stack);
    
    if (Add_Alloc_Mem == nullptr)
        return Alloc_Err;

#ifdef CANARY_PROTECT
    Canary_Recalloc(My_Stack, Add_Alloc_Mem);
#else
    My_Stack->data = (elem_t *) (Add_Alloc_Mem);
#endif

    if (mode == UP_MODE)
        for (int i = My_Stack->capacity / MULTIPLIER; i < My_Stack->capacity; i++)
            My_Stack->data[i] = POISON;
            
    STACK_VERIFY(My_Stack);

#ifdef HASH_PROTECT
    CHECK_HASH(My_Stack);
#endif

    return No_Err;
}

//---------------------------------------------------------------------------------------------//

static char * Stack_Recalloc(Stack * const My_Stack)
{
    assert(My_Stack);

#ifdef CANARY_PROTECT
    size_t Add_Mem_Value = 2 * sizeof(canary_t) + My_Stack->capacity * sizeof(elem_t) +
    (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t);
#else
    size_t Add_Mem_Value = My_Stack->capacity * sizeof(elem_t);
#endif

#ifdef CANARY_PROTECT
    char * Add_Alloc_Mem = (char *)realloc (My_Stack->canary_left_data, Add_Mem_Value * sizeof(canary_t));
#else
    char * Add_Alloc_Mem = (char *)realloc (My_Stack->data, Add_Mem_Value * sizeof(elem_t));
#endif

    if (Add_Alloc_Mem == nullptr)
    {
        Stack_Print_Err(My_Stack, Alloc_Err, __FILE__, __PRETTY_FUNCTION__, __LINE__);
#ifdef USE_LOGS
        Logfile_Stack_Print_Err(My_Stack, Alloc_Err,  __FILE__, __PRETTY_FUNCTION__, __LINE__);
#endif
        return nullptr;
    }

    return Add_Alloc_Mem;
}

//---------------------------------------------------------------------------------------------//

#ifdef CANARY_PROTECT
static void Canary_Recalloc(Stack * const My_Stack, const char * const Add_Alloc_Mem)
{
    assert(Add_Alloc_Mem);

    My_Stack->canary_left_data = (canary_t *) (Add_Alloc_Mem);

    My_Stack->data = (elem_t *) (Add_Alloc_Mem + sizeof(canary_t));

    if (My_Stack->capacity % sizeof(canary_t) == 0)
        My_Stack->canary_right_data = (canary_t *) (Add_Alloc_Mem + My_Stack->capacity * sizeof(elem_t) + sizeof(canary_t));
    else
        My_Stack->canary_right_data = (canary_t *) (Add_Alloc_Mem + My_Stack->capacity * sizeof(elem_t) + sizeof(canary_t) +
        (sizeof(canary_t) - (My_Stack->capacity % sizeof(canary_t))) * sizeof(elem_t));

    *(My_Stack->canary_right_data) = Canary_Value;    
}
#endif

//---------------------------------------------------------------------------------------------//

static int Is_Empty(Stack * const My_Stack)
{
    return My_Stack->size == 0;
}

//---------------------------------------------------------------------------------------------//

void Stack_Dtor(Stack * const My_Stack)
{
    assert(My_Stack);

#ifdef CANARY_PROTECT
    free(My_Stack->canary_left_data);

    My_Stack->canary_left_data = nullptr;

    My_Stack->canary_right_data = nullptr;
#else
    free(My_Stack->data);
#endif
    My_Stack->capacity = 0;

    My_Stack->size = 0;

    My_Stack->data = nullptr;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "stackfunctions.h"
#include "stackerrors.h"
#include "log.h"

int main(void)
{
	Stack My_Stack = {};

    if (Stack_Ctor(&My_Stack) != No_Err)
	{
		Coloured_Print(stderr, "FAILED THE INITIALIZATION OF STACK\n", RED);
		return Alloc_Err;
	}

	for (int i = 0; i < 16; i++)
	{
		if (Stack_Push(&My_Stack, i) != No_Err)
		    return Alloc_Err;
	}

    for (int i = 0; i < 16; i++)
	    if (Stack_Pop(&My_Stack) != No_Err)
		    return Alloc_Err;
	
    Stack_Dtor(&My_Stack);

	return 0;
}
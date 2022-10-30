#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "stackfunctions.h"
#include "stackerrors.h"

int main(void)
{
	Stack_Info MyStack = {}; // Stack

    if (Stack_Ctor(&MyStack) != Stack_Errno::Stack_No_Err)
	{
		fprintf(stderr, "Failed initialization of stack\n");
		return (int) Stack_Errno::Stack_Alloc_Err;
	}

    for (int i = 0; i < MyStack.capacity + 2; i++)
	    printf("%d\n", MyStack.data[i]);
	
    Stack_Dtor(&MyStack);

	return 0;
}
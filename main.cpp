#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "stack_library.h"
#include "stackerrors.h"

int main(void)
{
	Stack_Info MyStack = {};

	if(Def_Capacity <= 0)
	{
		fprintf(stderr, "Incorrect value of capacity: %d\n", Def_Capacity);
		return (int) Stack_Errno::Stack_Undef_Capacity;
	}

    if(Stack_Ctor(&MyStack) != Stack_Errno::Stack_No_Err)
	{
		fprintf(stderr, "Failed initialization of stack\n");
		return (int) Stack_Errno::Stack_Alloc_Err;
	}

	for (int i = 1; i <= 20; i++)
	{
		if (Stack_Push(&MyStack, i) == Stack_Errno::Stack_Alloc_Err)
		    return (int) Stack_Errno::Stack_Alloc_Err;
        Stack_Dump(&MyStack);
	    printf("The last elem of stack is %d\n", Stack_Peep(&MyStack));
	}

	for (int i = 1; i <= 20; i++)
	{
		if (Stack_Pop(&MyStack) == Stack_Errno::Stack_Underflow)
		    return (int) Stack_Errno::Stack_Alloc_Err;
	    Stack_Dump(&MyStack);
	    printf("The last elem of stack is %d\n", Stack_Peep(&MyStack));
	}

    Stack_Dtor(&MyStack);

	return 0;
}
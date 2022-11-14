#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "stack.h"
#include "debug.h"
#include "log.h"

int main(void)
{
	Stack My_Stack = {};

#ifdef USE_LOGS
	if (Logfile_Ctor() == Logfile_Error)
        return Logfile_Error;
#endif

    if (Stack_Ctor(&My_Stack) != No_Err)
	{
		fprintf(stderr, "FAILED THE INITIALIZATION OF STACK\n");
		return Alloc_Err;
	}

	Stack_Push(&My_Stack, 1);
	Stack_Push(&My_Stack, 2);
	Stack_Push(&My_Stack, 3);
	Stack_Push(&My_Stack, 2);

	Stack_Pop(&My_Stack);
	Stack_Pop(&My_Stack);
	Stack_Pop(&My_Stack);

#ifdef USE_LOGS
	Logfile_Dtor();
#endif

    Stack_Dtor(&My_Stack);

	return 0;
}
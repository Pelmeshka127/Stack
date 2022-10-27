#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <assert.h>

#include "stack_library.h"
#include "stackerrors.h"

int Is_Empty(Stack_Info * const MyStack)
{
    return MyStack->size == 0;
}

void Skip_Line()
{
    while (getchar() != '\n');
}

int Is_Eq_Double(double first, double second)
{
    return fabs(first - second) < 0.005; 
}

int Is_Zero(double num)
{
    return Is_Eq_Double(num, 0);
}
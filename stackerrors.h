#ifndef STACKERRORS_H_
#define STACKERRORS_H_

#include "stack_library.h"

#define Return_Err()                       \
fprintf(stderr, "Error in file %s\n"       \
        "in function%s\nin line %d\n%s\n", \
        __FILE__, __PRETTY_FUNCTION__,     \
        __LINE__, strerror(errno));

int Is_Empty(Stack_Info * const MyStack);

const double Eps = 0.005;

void Skip_Line();

int Is_Eq_Double(double first, double second);

int Is_Zero(double num);

#endif
#ifndef STACKCONFIG_H_
#define STACKCONFIG_H_

#include <stdio.h>

static const size_t DEF_CAPACITY = 3;

static const size_t MULTIPLIER = 2;

static const size_t MIN_SIZE = 4;

static const int UP_MODE = 1;

static const int LOW_MODE = -1;

static const unsigned long POISON = 0xBABADEAD;

typedef int elem_t;

#endif
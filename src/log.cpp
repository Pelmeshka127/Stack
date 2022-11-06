#include <stdio.h>

#include "stackerrors.h"
#include "log.h"

FILE * logfile = nullptr;

char * LOGFILE_NAME = "logfile.txt";

int Logfile_Ctor()
{
    logfile = fopen(LOGFILE_NAME, "w");

    if (logfile == nullptr)
        return Logfile_Error;

    return No_Err;
}

int Logdile_Dtor()
{
    if (fclose(logfile) == 0)
        return Logfile_Error;

    return No_Err;
}

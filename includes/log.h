#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

extern char * LOGFILE_NAME;

extern FILE * logfile;

/// @brief 
/// @return 
int Logfile_Ctor();

/// @brief 
/// @return 
int Logdile_Dtor();

#endif

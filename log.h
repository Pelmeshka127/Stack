#ifndef LOG_H_
#define LOG_H_

#define USE_LOGS

//---------------------------------------------------------------------------------------------//

/// @brief Opens logfile 
/// @return Logfile_Err if openning failed and No_Err if it's ok
int Logfile_Ctor();

//---------------------------------------------------------------------------------------------//

/// @brief Close logfile
/// @return Logfile_Err if openning failed and No_Err if it's ok
int Logfile_Dtor();

//---------------------------------------------------------------------------------------------//

/// @brief Function prints the code of error to the logfile
/// @param My_Stack is ptr on struct Stack
/// @param Err_Code the code of error
/// @param File 
/// @param Function 
/// @param Line 
void Logfile_Stack_Print_Err(Stack * const My_Stack, const int Err_Code, const char * const File, const char * const Function, const int Line);

//---------------------------------------------------------------------------------------------//

#ifdef HASH_PROTECT
/// @brief 
/// @param My_Stack 
/// @param Cur_Hash_Value 
/// @param File 
/// @param Function 
/// @param Line 
void Logfile_Hash_Err(Stack * const My_Stack, hash_t Cur_Hash_Value, const int Error, const char * const File, const char * const Function, const int Line);
#endif

//---------------------------------------------------------------------------------------------//

/// @brief 
/// @param My_Stack 
/// @param File 
/// @param Function 
/// @param Line 
void Logfile_Stack_Dump(Stack * const My_Stack, const char * const File, const char * const Function, const int Line);

//---------------------------------------------------------------------------------------------//


#endif
#ifndef STACKERRORS_H_
#define STACKERRORS_H_

#define Return_Err()                       \
fprintf(stderr, "Error in file %s\n"       \
        "in function%s\nin line %d\n", \
        __FILE__, __PRETTY_FUNCTION__,     \
        __LINE__)



/// @brief Includes the codes of errors
enum class Stack_Errno {
    Stack_No_Err = 0,
    
    Stack_Undef_Capacity = 1,
    
    Stack_Alloc_Err = 2,
    
    Stack_Underflow = 3,
    
    Stack_Overflow = 4,
    
    Stack_Canary_Left_Failed = 5,
    
    Stack_Canary_Right_Failed = 6,

	Stack_Canary_Data_Left_Failed = 7,

	Stack_Canary_Data_Right_Failed = 8,

	Stack_Logfile_Error = 9,
};

#endif STACKERRORS_H_
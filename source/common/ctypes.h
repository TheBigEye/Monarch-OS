#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H 1

#define NULL ((void *)0)

/**
 * @brief Sometimes we want to keep parameters to a function for later use
 * and this is a solution to avoid the 'unused parameter' compiler warning
 */
#define UNUSED(x) (void)(x)

#define FOREVER for(;;)

/* Definition of common types used in the codebase */

typedef unsigned int    dword; // unsigned 32 bit value (0 to 4,294,967,295)
typedef unsigned short   word; // unsigned 16 bit value (ot to 65,535)
typedef unsigned char    byte; // unsigned 8 bit value (0 to 255)

#endif /* _COMMON_TYPES_H */

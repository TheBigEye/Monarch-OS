#ifndef CSTRING_H_
#define CSTRING_H_

#include "cstddef.h"

char* concatenateStrings(char* str1, const char* str2); // strcat
char* copyString(char* str1, const char* str2); // strcpy
bool stringsEqual(const char* str1, const char* str2); // strcmp

size_t stringLength(const char* s);

char* strrev(const char* s);

char* to_string(int n);
char* strncat(char* dest, const char* src, int n);
char* strncpy(char* dest, const char* src, int n);

#endif /* CSTRING_H_ */

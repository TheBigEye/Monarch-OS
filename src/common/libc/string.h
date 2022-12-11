#ifndef STRING_H_
#define STRING_H_

#include "stdint.h"

char* concatenateStrings(char* str1, const char* str2); // strcat
char* copyString(char* str1, const char* str2); // strcpy
bool stringsEqual(const char* str1, const char* str2); // strcmp

size_t stringLength(const char* s);

char* strrev(const char* s);

char* to_string(int n);
char* strncat(char* dest, const char* src, int n);
char* strncpy(char* dest, const char* src, int n);

#endif /* STRING_H_ */

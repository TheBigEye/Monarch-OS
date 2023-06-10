#ifndef _COMMON_STRING_H
#define _COMMON_STRING_H 1

#include <stdbool.h>
#include <stdint.h>

/*
int strlen(const char* string);
int strcmp(char *a, char *b);

char *strcpy(char *a, const char *b);
char *strcat(char *a, const char *b);

void *memcpy(void* dstptr, const void* srcptr, int size);
void *memset(void* bufptr, int value, int size);
*/

char* itoa(int n);
char* htoa(int n);
int atoi(char str[]);

int strlen(char str[]);
void reverse(char str[]);

bool backspace(char s[]);
void append(char str[], char num);

int strcmp(char *a, char *b);
bool stringStartsWith(char *a, char *b);

int sum_decode(char s[], int pos);
int mul_decode(char s[], int pos);

#endif /* _COMMON_STRING_H */

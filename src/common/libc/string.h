#ifndef STRING_H_
#define STRING_H_

typedef char* string;           // String of chars

string strcat(string s1, string s2);
string strcpy(string s1, string s2);
int strcmp(string s1, string s2);

unsigned long long strlen(string s);
unsigned long long strlen(const char *s);

string strrev(string s);

string to_string(int n);
string strncat(string s1, string s2, int n);
string strncpy(string s1, string s2, int n);

#endif /* STRING_H_ */

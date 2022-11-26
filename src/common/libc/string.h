#ifndef STRING_H_
#define STRING_H_

typedef char* string;

string strcat(string s1, string s2);
string strcpy(string s1, string s2);
int strcmp(string s1, string s2);
int strlen(string s);

string strrev(string s);

string to_string(int n);
string strncat(string s1, string s2, int n);
string strncpy(string s1, string s2, int n);

#endif /* STRING_H_ */

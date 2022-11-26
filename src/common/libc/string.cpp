#include "string.h"


string strcat(string s1, string s2) {
    int i, j;
    for (i = 0; s1[i] != '\0'; i++);
    for (j = 0; s2[j] != '\0'; j++, i++) {
        s1[i] = s2[j];
    }
    s1[i] = '\0';
    return s1;
}

string strcpy(string s1, string s2) {
    int i;
    for (i = 0; s2[i] != '\0'; i++) {
        s1[i] = s2[i];
    }
    s1[i] = '\0';
    return s1;
}

int strcmp(string s1, string s2) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') { return 0; }
    }
    return s1[i] - s2[i];
}

int strlen(string s) {
    int i;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}

string strrev(string s) {
    int i, j;
    string s1;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        s1[i] = s[j];
        s1[j] = s[i];
    }
    s1[i] = '\0';
    return s1;
}

string to_string(int n) {
    string s = "";
    int i = 0;
    while (n != 0) {
        s[i++] = n % 10 + '0';
        n /= 10;
    }
    s[i] = '\0';
    return strrev(s);
}

string to_string(unsigned int n) {
    string s = "";
    int i = 0;
    while (n != 0) {
        s[i++] = n % 10 + '0';
        n /= 10;
    }
    s[i] = '\0';
    return strrev(s);
}

string to_string(unsigned char n) {
    string s = "";
    int i = 0;
    while (n != 0) {
        s[i++] = n % 10 + '0';
        n /= 10;
    }
    s[i] = '\0';
    return strrev(s);
}

string strncat(string s1, string s2, int n) {
    int i, j;
    for (i = 0; s1[i] != '\0'; i++);
    for (j = 0; s2[j] != '\0' && j < n; j++, i++) {
        s1[i] = s2[j];
    }
    s1[i] = '\0';
    return s1;
}

string strncpy(string s1, string s2, int n) {
    int i;
    for (i = 0; s2[i] != '\0' && i < n; i++) {
        s1[i] = s2[i];
    }
    s1[i] = '\0';
    return s1;
}

#include "cstring.h"
#include "cstddef.h"

// STRCAT() equivalent
char* concatenateStrings(char* str1, const char* str2) {
    // Find the end of the first string
    int index1;
    for (index1 = 0; str1[index1] != '\0'; index1++);

    // Copy the characters from the second const char *to the end of the first
    // Initialize index1 to the value of index1 after the first loop
    for (int index2 = 0; str2[index2] != '\0'; index2++, index1++) {
        str1[index1] = str2[index2];
    }

    // Append the null character to the end of the first string
    str1[index1] = '\0';
    return str1;
}

// STRCPY() equivalent
char* copyString(char* str1, const char* str2) {
    // Copy the characters from str2 to str1
    for (int index = 0; str2[index] != '\0'; index++) {
        str1[index] = str2[index];

        // Append the null character to the end of the first string
        if (str2[index] == '\0') {
            str1[index] = '\0';
        }
    }
    return str1;
}

// STRCMP() equivalent
bool stringsEqual(const char* str1, const char* str2) {
    // Compare the characters in str1 and str2 one by one
    for (int index = 0; str1[index] == str2[index]; index++) {
        // If we reach the end of the const char *(indicated by the null character '\0'), return true
        if (str1[index] == '\0') {
            return true;
        }
    }

    // If any of the characters are different or the end of the const char *is not reached, return false
    return false;
}

size_t stringLength(const char* str) {
    // Initialize the length to 0
    size_t length = 0;

    // Count the number of characters in the string
    // Stop when we reach the end of the const char *(indicated by the null character '\0')
    while (str[length] != '\0') {
        length++;
    }

    // Return the length of the string
    return length;
}

char* strrev(const char* s) {
    int i, j;
    char *s1;
    for (i = 0, j = stringLength(s) - 1; i < j; i++, j--) {
        s1[i] = s[j];
        s1[j] = s[i];
    }
    s1[i] = '\0';
    return s1;
}

char* to_string(int n) {
    char* s = "";
    int i = 0;
    while (n != 0) {
        s[i++] = n % 10 + '0';
        n /= 10;
    }
    s[i] = '\0';
    return strrev(s);
}

char* strncat(char* dest, const char* src, int n) {
    // Initialize the index variables
    int destIndex = 0;
    int srcIndex = 0;

    // Find the end of the dest string
    while (dest[destIndex] != '\0') {
        destIndex++;
    }

    // Append up to n characters from the src string
    // to the end of the dest string
    while (src[srcIndex] != '\0' && srcIndex < n) {
        dest[destIndex] = src[srcIndex];
        destIndex++;
        srcIndex++;
    }

    // Append the null character to the end of the dest string
    dest[destIndex] = '\0';

    // Return a pointer to the dest string
    return dest;
}

char* strncpy(char* dest, const char* src, int n) {
    // Initialize the index variables
    int destIndex = 0;
    int srcIndex = 0;

    // Copy up to n characters from the src string
    // to the dest string
    while (src[srcIndex] != '\0' && srcIndex < n) {
        dest[destIndex] = src[srcIndex];
        destIndex++;
        srcIndex++;
    }

    // Append the null character to the end of the dest string
    dest[destIndex] = '\0';

    // Return a pointer to the dest string
    return dest;
}

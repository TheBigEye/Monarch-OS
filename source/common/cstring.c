#include "cstring.h"
#include "ctypes.h"

/**
int strlen(const char* string) {
	int result = 0;
	while (string[result++]);
	return result;
}

int strcmp(char *a, char *b) {
	for (; *a, *b; a++, b++) {
		if (*a != *b) {
			return -1;
        }
    }

	if (*a != '\0') return 1;
	if (*b != '\0') return -1;

	return 0;
}

char *strcpy(char *a, const char *b) {
    char *temp = a;
	while(*a++ = *b++);
	return temp;
}

char *strcat(char *a, const char *b) {
    for (; *a; a++);
	for (; *b; a++, b++) *a = *b;
	*a = 0;

    return a;
}
**/

/* C COMMON STANDARD */

/**
 * Convert an integer to a string.
 *
 * @param n The integer to convert.
 * @return The converted string.
 */
char* itoa(int n) {
    static char str[12];
    int i, sign;

    if (n < 0) {
        sign = -1;
        n = -n;
    } else {
        sign = 1;
    }

    i = 0;
    do {
        str[i++] = (char)(n % 10 + '0');
    } while ((n /= 10) > 0);

    if (sign == -1) {
        str[i++] = '-';
    }

    str[i] = '\0';
    reverse(str);
    return str;
}


/**
 * Convert an integer to a hexadecimal string.
 *
 * @param n The integer to convert.
 * @return The converted hexadecimal string.
 */
char* htoa(int n) {
    static char str[12];
    int index = 0;
    str[index++] = '0';
    str[index++] = 'x';
    char zeros = 0;

    int32_t tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) {
            continue;
        }
        zeros = 1;
        if (tmp >= 0xA) {
            str[index++] = (char)(tmp - 0xA + 'A');
        } else {
            str[index++] = (char)(tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) {
        str[index++] = (char)(tmp - 0xA + 'A');
    } else {
        str[index++] = (char)(tmp + '0');
    }

    str[index] = '\0';
    return str;
}


/**
 * Convert a string to an integer.
 *
 * @param str The input string.
 * @return The converted integer.
 */
int atoi(char str[]) {
    int num = 0;
    while('0' <= *str && *str <= '9') {
        num = num * 10 + *str++ - '0';
    }
    return num;
}


/**
 * Calculate the length of a string.
 *
 * @param str The input string.
 * @return The length of the string.
 */
int strlen(char str[]) {
    int len = 0;
    while (str[len] != '\0') ++len;
    return len;
}


/**
 * Reverse a string.
 *
 * @param str The input string.
 */
void reverse(char str[]) {
    char tmp;
    int len, i;

    len = strlen(str);
    for (i = 0; i < len / 2; i++) {
        tmp = str[len - i - 1];
        str[len - i - 1] = str[i];
        str[i] = tmp;
    }
}


/**
 * Append a character to a string.
 *
 * @param str The input string.
 * @param n The character to append.
 */
void append(char str[], char num) {
    int length = strlen(str);
    str[length] = num;
    str[length + 1] = '\0';
}

/**
 * Remove the last character from a string (backspace).
 *
 * @param str The input string.
 * @return True if a character was removed, false otherwise.
 */
bool backspace(char str[]) {
    int length = strlen(str);
    if (length > 0) {
        str[length - 1] = '\0';
        return true;
    }
    return false;
}

/**
 * Compare two strings lexicographically.
 *
 * @param a The first string.
 * @param b The second string.
 * @return 0 if the strings are equal, a negative value if the first string is less than the second,
 *         or a positive value if the first string is greater than the second.
 */
int strcmp(char *a, char *b) {
    while (*a && *a == *b) {
        a++, b++;
    }
    return (byte)*a - (byte)*b;
}

/**
 * Check if a string starts with another string.
 *
 * @param a The string to check.
 * @param b The prefix to compare.
 * @return True if the string starts with the prefix, false otherwise.
 */
bool stringStartsWith(char *a, char *b) {
    int length_a = strlen(a); int i = 0;
    int length_b = strlen(b); int j = 0;

    for (i = 0; i <= length_b - length_a; i++) { // Slide the pattern 'a' one by one in 'b'
        for (j = 0; j < length_a; j++) { // Check for pattern match at the current index 'i'
            if (b[i + j] != a[j]) {
                break; // If characters don't match, break out of the inner loop
            }
        }

        if (j == length_a) { // If the inner loop completed without breaking, it means 'a' is found at index 'i' in 'b'
            return true; // 'a' is a prefix of 'b', return true
        }
    }
    return false; // 'a' is not a prefix of 'b', return false
}


/**
 * Decode and calculate the sum of numbers in a comma-separated string.
 *
 * @param s The input string.
 * @param pos The starting position in the string.
 * @return The sum of the numbers.
 */
int sum_decode(char s[], int pos){
    int str_len = strlen(s);
    char extracted_str[100] = "";
    int j = 0, i = 0;

    for (i = pos; i < str_len; i++) {
        extracted_str[j++] = s[i];
    }

    extracted_str[j]='0';

    int extracted_str_len = strlen(extracted_str);
    int sum = 0;
    int num = 0;
    for (i = 0; i < extracted_str_len-1; i++) {
        if (extracted_str[i] != ',') {
            num *= 10;
            num += (extracted_str[i] - '0');
        } else {
            sum += num;
            num = 0;
        }
    }
    sum += num;
    return sum;
}


/**
 * Decode and calculate the product of numbers in a comma-separated string.
 *
 * @param s The input string.
 * @param pos The starting position in the string.
 * @return The product of the numbers.
 */
int mul_decode(char s[], int pos){
    int str_len = strlen(s);
    char extracted_str[100] = "";
    int j = 0, i = pos;

    for (i = pos; i < str_len; i++) {
        extracted_str[j++] = s[i];
    }

    extracted_str[j]= '0';

    int extracted_str_len = strlen(extracted_str);
    int mul = 1;
    int num = 0;
    for (i = 0; i < extracted_str_len - 1; i++) {
        if (extracted_str[i] != ',') {
            num *= 10;
            num += (extracted_str[i] - '0');
        } else {
            mul *= num;
            num = 0;
        }
    }
    mul *= num;
    return mul;
}

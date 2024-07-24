#include "sysutils.h"

/*
 * Get the square root from an exponent
 */
double sqrt(int n) {
    double a = (double) n;
    double x = 1;
    for (int i = 0; i < n; i++) {
        x = 0.5 * (x + a / x);
    }
    return x;
}

double factorial(int n) {
    double fact = 1;
    while (n > 1) fact *= n--;
    return fact;
}


double pow(double base, int exp) {
    double result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}


int atoi(const char *string) {
    int sign = 1;
    int base = 0;

    while (*string == ' ') string++;

    if (*string == '-' || *string == '+') {
        sign = 1 - 2 * (*string++ == '-');
    }

    while (*string >= '0' && *string <= '9') {
        if ((base > INT32_MAX / 10) || (base == INT32_MAX / 10 && *string - '0' > 7)) {
            return (sign == 1) ? INT32_MAX : INT32_MIN;
        }
        base = 10 * base + (*string++ - '0');
    }

    return base * sign;
}

/**
 * Convert an integer to a string.
 *
 * @param integer The integer to convert.
 * @return The converted string.
 */
char *itoa(int integer) {
    static char string[16]; // Static buffer to store the converted string
    int index = 0;
    bool sign = (integer < 0);

    if (integer < 0) integer = -integer; // Make the number positive for conversion

    do {
        string[index++] = (char)(integer % 10 + '0'); // Convert the least significant digit to character and store it in the buffer
        integer /= 10; // Divide the number by 10 to move to the next digit
    } while (integer > 0);

    if (sign) string[index++] = '-'; // If the number was negative, add a minus sign to the buffer

    string[index] = '\0'; // Add null-terminator to mark the end of the string

    // Reverse the string
    for (int i = 0, j = index - 1; i < j; ++i, --j) {
        char temp = string[i];
        string[i] = string[j];
        string[j] = temp;
    }

    return string; // Return the converted string
}


int normalize(double *val) {
    int exponent = 0;
    double value = *val;

    while (value >= 1.0) {
        value /= 10.0;
        ++exponent;
    }

    while (value < 0.1) {
        value *= 10.0;
        --exponent;
    }
    *val = value;
    return exponent;
}


/**
 * Convert a float or double to a string.
 *
 * @param value The value to convert.
 * @return The converted string.
 */
char *ftoa(double value) {
    static char buffer[32]; // Buffer to store the resulting string
    char* pointer = buffer; // Pointer for traversing the buffer

    int exponent = 0; // Exponent for normalizing the value
    int places = 0; // Decimal places counter
    static const int width = 6; // Desired width for the decimal part

    // Handle special cases: zero and extreme values
    if (ABS(value) < EPSILON) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    if (value >= DOUBLE_MAX || value <= DOUBLE_MIN) {
        stringCopy(buffer, (value > 0) ? "INFINITE" : "-INFINITE");
        return buffer;
    }

    // Adjust the sign if negative
    if (value < 0.0) {
        *pointer++ = '-';
        value = -value;
    }

    // Normalize the value and calculate the exponent
    exponent = normalize(&value);

    // Convert integer part
    while (exponent > 0) {
        int digit = value * 10;
        *pointer++ = digit + '0';
        value = value * 10 - digit;
        ++places;
        --exponent;
    }

    // Add leading zero if no integer part
    if (places == 0) {
        *pointer++ = '0';
    }

    // Add decimal point if needed
    if (places < width) {
        *pointer++ = '.';
    }

    // Convert fractional part
    while (exponent < 0 && places < width) {
        *pointer++ = '0';
        --exponent;
        ++places;
    }

    // Fill remaining places with digits
    while (places < width) {
        int digit = value * 10.0;
        *pointer++ = digit + '0';
        value = value * 10.0 - digit;
        ++places;
    }

    *pointer = '\0'; // Null-terminate the string

    return buffer;
}


/**
 * Convert an integer to a hexadecimal string.
 *
 * @param integer The integer to convert.
 * @return The converted hexadecimal string.
 */
char *htoa(int integer) {
    int index = 0;
    static char buffer[16]; // Static buffer to store the converted hexadecimal string
    buffer[index++] = '0'; // Add the '0' character to the buffer
    buffer[index++] = 'x'; // Add the 'x' character to the buffer

    // Convert each nibble (4 bits) of the integer to a hexadecimal digit
    for (int i = 28; i >= 0; i -= 4) {
        int temp = (integer >> i) & 0xF; // Extract the current nibble
        if (temp >= 0xA) {
            buffer[index++] = (char)(temp - 0xA + 'A'); // Convert and add the hexadecimal digit (A-F) to the buffer
        } else {
            buffer[index++] = (char)(temp + '0'); // Convert and add the hexadecimal digit (0-9) to the buffer
        }
    }

    buffer[index] = '\0'; // Add null-terminator to mark the end of the string
    return buffer; // Return the converted hexadecimal string
}

/**
 * Calculate the length of a string (strlen).
 */
int stringLength(const char *string) {
    const char *pointer = string;
    while (*(pointer++));
    return pointer - string - 1;
}

/**
 * Reverse a string (strrev).
 */
void stringReverse(char *string) {
    // Check for a null pointer or an empty string
    if (string == NULL || *string == '\0') {
        return;
    }

    int length = stringLength(string);

    for (int index = 0; index < length / 2; index++) {
        char temp = string[index];
        string[index] = string[length - index - 1];
        string[length - index - 1] = temp;
    }
}

/**
 * Concatenate two strings (strcat).
 */
void stringCombine(char *destination, char *source) {
    char *end = destination + stringLength(destination);

    while (*source != '\0') {
        *end++ = *source++;
    }
    *end = '\0';
}

/**
 * Copy a null-terminated string from source to destinatio (strcpy).
 *
 * @param destination Pointer to the destination string buffer.
 * @param source Pointer to the source string to be copied.
 */
char *stringCopy(char *destination, const char *source) {
    // Copy characters until reaching the null terminator
    while ((*destination++ = *source++));

    return destination;
}

// strncpy
char *stringCopyTo(char *destination, const char *source, unsigned int nchars) {
    unsigned int i;

    for (i = 0; (i < (nchars - 1)) && (source[i] != '\0'); i++) {
        destination[i] = source[i];
    }

    destination[i] = '\0';

    return destination;
}


/**
 * Append a character to a string (append).
 *
 * @param string The input string.
 * @param num The character to append.
 */
void appendChar(char string[], char num) {
    int length = stringLength(string);
    string[length] = num;
    string[length + 1] = '\0';
}

/** String to uppercase (strlwr) */
void toLowercase(char *string) {
    do {
        *string = toLower(*string);
    } while (*(string++));
}

/** String to uppercase (strupr) */
void toUppercase(char *string) {
    do {
        *string = toUpper(*string);
    } while (*(string++));
}


/**
 * Converts an integer to a string representation in the specified base.
 *
 * @param integer The integer value to convert.
 * @param buffer The buffer where the resulting string will be stored.
 * @param base The base to use for the conversion (e.g., 10 for decimal, 16 for hexadecimal).
 */
void toString(int integer, char *buffer, int base) {
    // Static array for digit conversion in bases up to 16
    static const char digits[17] = "0123456789ABCDEF";

    // Pointer to navigate through the buffer
    char *pointer = buffer;

    // Determine if the number is negative in base 10
    bool sign = (integer < 0 && base == 10);

    // Convert negative integer to positive if base 10 and negative
    if (sign) integer = -integer;

    do {
        *pointer++ = digits[integer % base]; // Store the least significant digit in the buffer
        integer /= base; // Reduce integer by base for next digit conversion
    } while (integer);

    if (sign) *pointer++ = '-'; // Append '-' sign if originally negative in base 10
    *pointer-- = '\0'; // Null-terminate the string

    // Reverse the string in place
    for (char *start = buffer; start < pointer; ++start, --pointer) {
        char temp = *start; // Temporarily store character from start
        *start = *pointer; // Swap character at start with character at pointer
        *pointer = temp; // Store original character from start at pointer
    }
}

/**
 * Compare two strings lexicographically (strcmp).
 *
 * @param a The first string.
 * @param b The second string.
 * @return 0 if the strings are equal, a negative value if the first string is less than the second,
 *         or a positive value if the first string is greater than the second.
 */
int stringCompare(const char *a, const char *b) {
    // Protection against NULL pointers (segfault prevention)
    if (!a || !b) return a ? 1 : b ? -1 : 0;

    // Compare characters until a difference is found or end of strings
    while (*a && *a == *b) a++, b++;

    // Return the difference in ASCII values
    return (unsigned char) *a - (unsigned char) *b;
}

// strncmp
int stringCompareTo(const char *a, const char *b, unsigned int nchars) {
    while (nchars && *a && (*a == *b)) {
        ++a; ++b; --nchars;
    }

    if (nchars == 0) {
        return 0;
    } else {
        return (*(unsigned char *)a - *(unsigned char *)b);
    }
}


/**
 * Compare two strings, determining how many characters match before the first non-matching character (match).
 *
 * @param a The first string.
 * @param b The second string.
 * @return The index of the first non-matching character. If the strings are identical, returns -2.
 */
int stringMatch(char *a, char *b) {
    // Stores the index of the last matching character.
    int threshold = 0;

    // Loop to compare the characters of both strings one by one.
    for (int i = 0; *(a + i) == *(b + i); i++) {
        // If one of the current characters is the null terminator '\0',
        // it means we have reached the end of one or both strings.
        if (*(a + i) == '\0' || *(b + i) == '\0') {
            // Return the index of the last matching character.
            return i - 1;
        }
        // Update the current index.
        threshold = i;
    }

    // If the strings are identical up to the end, an additional comparison is performed.
    if (stringCompare(a, b) == 0) {
        // If the strings are identical, return -2.
        return -2;
    }

    // If none of the above conditions are met,
    // we return the index of the last matching character.
    return threshold;
}


/**
 * Check if a string starts with another string (substrcmp).
 *
 * @param a The string to check.
 * @param b The prefix to compare.
 * @return True if the string starts with the prefix, false otherwise.
 */
bool stringStarts(const char *a, const char *b) {
    int length_a = stringLength(a); int i = 0;
    int length_b = stringLength(b); int j = 0;

    for (i = 0; i <= (length_b - length_a); i++) { // Slide the pattern 'a' one by one in 'b'
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
 * Search the first occurrence of a character in a string (strchr)
 *
 * @param string   The string to search within.
 * @param character The character to find.
 * @return A pointer to the first occurrence of the character in the string,
 *         or NULL if the character is not found.
 */
char *stringFindChar(const char *string, int character) {
    while (*string != '\0') {
        if (*string == character) {
            return ((char *) string);
        }
        string++;
    }
    return NULL;
}

char *stringFindLastChar(const char *string, int character) {
    const char *last = NULL;
    while (*string != '\0') {
        if (*string == character) {
            last = string;
        }
        string++;
    }
    return ((char *) last);
}

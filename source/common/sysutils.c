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
    double fact = 1.0;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

double pow(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

double sin(double x) {
    x = x * (PI / 180.0);
    double sum = 0.0;

    for (int i = 0; i < 128; i++) {
        double term = pow(-1, i) * pow(x, 2 * i + 1) / factorial(2 * i + 1);
        sum += term;
    }

    return sum;
}

double cos(double x) {
    x = x * (PI / 180.0);
    double sum = 0.0;

    for (int i = 0; i < 128; i++) {
        double term = pow(-1, i) * pow(x, 2 * i) / factorial(2 * i);
        sum += term;
    }

    return sum;
}

double tan(double x) {
    return sin(x) / cos(x);
}

double ceil(double x) {
    int i = (int) x;
    return (double)(x > i ? i + 1 : i);
}

double floor(double x) {
    int i = (int) x;
    return (double)(x < i ? i - 1 : i);
}


int atoi(const char *string) {
    int sign = 1, base = 0, i = 0;

    // if whitespaces then ignore.
    while (string[i] == ' ') {
        i++;
    }

    // sign of number
    if (string[i] == '-' || string[i] == '+') {
        sign = 1 - 2 * (string[i++] == '-');
    }

    // checking for valid input
    while (string[i] >= '0' && string[i] <= '9') {
        // handling overflow test case
        if ((base > INT32_MAX / 10) || (base == INT32_MAX / 10 && string[i] - '0' > 7)) {
            if (sign == 1) {
                return INT32_MAX;
            } else {
                return INT32_MIN;
            }
        }
        base = 10 * base + (string[i++] - '0');
    }
    return base * sign;
}


/**
 * Convert an integer to a string.
 *
 * @param integer The integer to convert.
 * @return The converted string.
 */
char* itoa(int integer) {
    static char string[16]; // Static buffer to store the converted string
    int index = 0;
    int sign = (integer < 0) ? -1 : 1;

    if (integer < 0) integer = -integer; // Make the number positive for conversion

    do {
        string[index++] = (char)(integer % 10 + '0'); // Convert the least significant digit to character and store it in the buffer
        integer /= 10; // Divide the number by 10 to move to the next digit
    } while (integer > 0);

    if (sign == -1) string[index++] = '-'; // If the number was negative, add a minus sign to the buffer

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
char* ftoa(double value) {
    static char buffer[32]; // Buffer to store the resulting string
    char* p = buffer; // Pointer for traversing the buffer

    int exponent = 0; // Exponent for normalizing the value
    int places = 0; // Decimal places counter
    static const int width = 4; // Desired width for the decimal part

    // Handle special cases: zero and extreme values
    if (ABS(value) < EPSILON) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    if (value >= DOUBLE_MAX || value <= DOUBLE_MIN) {
        if (value > 0) {
            copyString(buffer, "INF");
        } else {
            copyString(buffer, "-INF");
        }
        return buffer;
    }

    // Adjust the sign if negative
    if (value < 0.0) {
        *p++ = '-';
        value = -value;
    }

    // Normalize the value and calculate the exponent
    exponent = normalize(&value);

    // Convert integer part
    while (exponent > 0) {
        int digit = value * 10;
        *p++ = digit + '0';
        value = value * 10 - digit;
        ++places;
        --exponent;
    }

    // Add leading zero if no integer part
    if (places == 0)
        *p++ = '0';

    // Add decimal point if needed
    if (places < width)
        *p++ = ',';

    // Convert fractional part
    while (exponent < 0 && places < width) {
        *p++ = '0';
        --exponent;
        ++places;
    }

    // Fill remaining places with digits
    while (places < width) {
        int digit = value * 10.0;
        *p++ = digit + '0';
        value = value * 10.0 - digit;
        ++places;
    }

    *p = '\0'; // Null-terminate the string

    return buffer;
}


/**
 * Convert an integer to a hexadecimal string.
 *
 * @param integer The integer to convert.
 * @return The converted hexadecimal string.
 */
char* htoa(int integer) {
    int index = 0;
    static char buffer[16]; // Static buffer to store the converted hexadecimal string
    buffer[index++] = '0'; // Add the '0' character to the buffer
    buffer[index++] = 'x'; // Add the 'x' character to the buffer

    // Convert each nibble (4 bits) of the integer to a hexadecimal digit
    for (int i = 28; i >= 0; i -= 4) {
        int tmp = (integer >> i) & 0xF; // Extract the current nibble
        if (tmp >= 0xA) {
            buffer[index++] = (char)(tmp - 0xA + 'A'); // Convert and add the hexadecimal digit (A-F) to the buffer
        } else {
            buffer[index++] = (char)(tmp + '0'); // Convert and add the hexadecimal digit (0-9) to the buffer
        }
    }

    buffer[index] = '\0'; // Add null-terminator to mark the end of the string
    return buffer; // Return the converted hexadecimal string
}


/**
 * Calculate the length of a string (strlen).
 */
int lengthString(const char *string) {
    int length = 0;

    // Iterate through the string until null terminator is reached
    while (string[length] != '\0') {
        length++;
    }

    // Return the calculated length
    return length;
}


/**
 * Reverse a string (strrev).
 */
void reverseString(char *string) {
    // Check for a null pointer or an empty string
    if (string == NULL || *string == '\0') {
        return;
    }

    int length = lengthString(string);

    for (int index = 0; index < length / 2; index++) {
        char temp = string[index];
        string[index] = string[length - index - 1];
        string[length - index - 1] = temp;
    }
}


/**
 * Concatenate two strings (strcat).
 */
void combineString(char *dest, char *source) {
    char *end = dest + lengthString(dest);

    while (*source != '\0') *end++ = *source++;
    *end = '\0';
}


/**
 * Copy an string into a another string.
 */
void copyString(char *dest, const char *source) {
    int i = 0;
    while (1) {
        dest[i] = source[i];

        if (dest[i] == '\0') {
            break;
        }

        i++;
    }
}


/**
 * Append a character to a string (append).
 *
 * @param string The input string.
 * @param num The character to append.
 */
void appendChar(char string[], char num) {
    int length = lengthString(string);
    string[length] = num;
    string[length + 1] = '\0';
}


/**
 * Remove the last character from a string (backspace).
 * @return True if a character was removed, false otherwise.
 */
bool backspace(char string[]) {
    int len = lengthString(string);
    if (len > 0) {
        string[len - 1] = '\0';
        return true;
    }
    return false;
}


/** Character to lowercase */
int toLower(char character) {
	return isUpper(character) ? character - 'A' + 'a' : character;
}

/** Character to uppercase */
int toUpper(char character) {
	return isLower(character) ? character - 'a' + 'A' : character;
}


/** String to lowercase */
void toLowercase(char *string) {
    int length = lengthString(string);
    for (int i = 0; i < length; i++) {
        string[i] = toLower(string[i]);
    }
}

/** String to uppercase */
void toUppercase(char *string) {
    int length = lengthString(string);
    for (int i = 0; i < length; i++) {
        string[i] = toUpper(string[i]);
    }
}

/**
 * Convert a integer value into an string
 */
void toString(int integer, char *buffer, int base) {
    static char digits[17] = "0123456789ABCDEF";
    int index = 0;
    bool sign = false;

    if (integer < 0 && base == 10) {
        sign = true;
        integer = -integer;
    }

    do {
        buffer[index++] = digits[integer % base];
        integer /= base;
    } while (integer != 0);

    if (sign) buffer[index++] = '-';

    // Reverse the string
    for (int i = 0, j = index - 1; i < j; ++i, --j) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    buffer[index] = '\0';
}


/**
 * Compare two strings lexicographically (strcmp).
 *
 * @param a The first string.
 * @param b The second string.
 * @return 0 if the strings are equal, a negative value if the first string is less than the second,
 *         or a positive value if the first string is greater than the second.
 */
int equalsWith(const char* a, const char* b) {
    // Protection/segmentation fault check
    if (a == NULL || b == NULL) {
        return (a == NULL) ? ((b == NULL) ? 0 : -1) : 1;
    }

    if (a == b) return 0;

    while (*a && (*a == *b)) {
        a++; b++;
    }
    return (unsigned char) *a - (unsigned char) *b;
}


/**
 * Compare two strings, how many characters match, before the first unmatching character (match).
 *
 * @param a The first string.
 * @param b The second string.
 * @return The index of the first unmatching character. If the strings are identical, returns -2.
 */
int matchWith(char *a, char *b) {
    int threshold = 0;

    for (int i = 0; *(a + i) == *(b + i); i++) {
        if (*(a + i) == '\0' || *(b + i) == '\0') return i - 1;
        threshold = i;
    }

    if (equalsWith(a, b) == 0) {
        return -2;
    }

    return threshold;
}


/**
 * Check if a string starts with another string (substrcmp).
 *
 * @param a The string to check.
 * @param b The prefix to compare.
 * @return True if the string starts with the prefix, false otherwise.
 */
bool startsWith(const char *a, const char *b) {
    int length_a = lengthString(a); int i = 0;
    int length_b = lengthString(b); int j = 0;

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

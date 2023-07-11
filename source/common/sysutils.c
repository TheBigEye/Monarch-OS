#include "sysutils.h"


static unsigned int rand_state = 32;

void setRandom(unsigned int seed) {
    rand_state = seed;
}

int getSignedRandom() {
    rand_state = (rand_state * 1103515245U + 12345U) & 0x7fffffff;
    int result = (int) rand_state;
    if (result > 0x3fffffff) {
        result -= 0x7fffffff;
    }
    return result;
}

unsigned int getUnsignedRandom() {
    rand_state = (rand_state * 1103515245U + 12345U) & 0xffffffffU;
    return rand_state & 0x7fffffff;
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


/**
 * Convert an integer to a hexadecimal string.
 *
 * @param integer The integer to convert.
 * @return The converted hexadecimal string.
 */
char* htoa(int integer) {
    static char string[16]; // Static buffer to store the converted hexadecimal string
    int index = 0;
    string[index++] = '0'; // Add the '0' character to the buffer
    string[index++] = 'x'; // Add the 'x' character to the buffer

    // Convert each nibble (4 bits) of the integer to a hexadecimal digit
    for (int i = 28; i >= 0; i -= 4) {
        int tmp = (integer >> i) & 0xF; // Extract the current nibble
        if (tmp >= 0xA) {
            string[index++] = (char)(tmp - 0xA + 'A'); // Convert and add the hexadecimal digit (A-F) to the buffer
        } else {
            string[index++] = (char)(tmp + '0'); // Convert and add the hexadecimal digit (0-9) to the buffer
        }
    }

    string[index] = '\0'; // Add null-terminator to mark the end of the string
    return string; // Return the converted hexadecimal string
}


/**
 * Calculate the length of a string (strlen).
 */
int lengthString(char *string) {
    int length = 0;

    while (*string != '\0') {
        length++;
        string++;
    }
    return length;
}

/**
 * Reverse a string (strrev).
 */
void reverseString(char *string) {
    int length = lengthString(string);

    for (int index = 0; index < length / 2; index++) {
        char temp = string[index];
        string[index] = string[length - index - 1];
        string[length - index - 1] = temp;
    }
}


/**
 * Append a character to a string (append).
 *
 * @param str The input string.
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


// Character to lowercase
int toLower(char character) {
	return isUpper(character) ? character - 'A' + 'a' : character;
}

// Character to uppercase
int toUpper(char character) {
	return isLower(character) ? character - 'a' + 'A' : character;
}


// String to lowercase
void toLowercase(char *string) {
    int length = lengthString(string);
    for (int i = 0; i < length; i++) {
        string[i] = toLower(string[i]);
    }
}

// String to uppercase
void toUppercase(char *string) {
    int length = lengthString(string);
    for (int i = 0; i < length; i++) {
        string[i] = toUpper(string[i]);
    }
}

void toString(int integer, char *buffer, int base) {
    static char digits[] = "0123456789ABCDEF";
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
    while (*a && (*a == *b)) {
        a++; b++;
    }
    return (unsigned char) *a - (unsigned char) *b;
}


/**
 * Check if a string starts with another string (substrcmp).
 *
 * @param a The string to check.
 * @param b The prefix to compare.
 * @return True if the string starts with the prefix, false otherwise.
 */
bool startsWith(char *a, char *b) {
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


/**
 * Decode and calculate the sum of numbers in a comma-separated string.
 *
 * @param input The input string.
 * @param pos The starting position in the input.
 * @return The sum result of the numbers.
 */
int getInputSum(char input[], int pos) {
    char string[127] = "";
    int index = 0;
    int sum = 0;
    int num = 0;

    int line = lengthString(input);

    for (int i = pos; i < line; i++) {
        string[index++] = input[i];
    }

    string[index] = '0';

    int len = lengthString(string);
    for (int i = 0; i < (len - 1); i++) {
        if (string[i] != ',') {
            num *= 10;
            num += (string[i] - '0');
        } else {
            sum += num;
            num = 0;
        }
    }
    sum += num;
    return sum;
}

/**
 * Decode and calculate the subtraction of numbers in a comma-separated string.
 *
 * @param input The input string.
 * @param pos The starting position in the input.
 * @return The subtraction result of the numbers.
 */
int getInputSub(char input[], int pos) {
    char string[127] = "";
    int index = 0;
    int sub = 0;
    int num = 0;
    int line = lengthString(input);

    for (int i = pos; i < line; i++) {
        string[index++] = input[i];
    }

    string[index] = '0';

    int len = lengthString(string);
    for (int i = 0; i < (len - 1); i++) {
        if (string[i] != ',') {
            num *= 10;
            num += (string[i] - '0');
        } else {
            if (sub == 0) {
                sub = num;
            } else {
                sub -= num;
            }
            num = 0;
        }
    }
    sub -= num;
    return sub;
}


/**
 * Decode and calculate the product of numbers in a comma-separated string.
 *
 * @param input The input string.
 * @param pos The starting position in the string.
 * @return The product of the numbers.
 */
int getInputMul(char input[], int pos){
    char string[127] = "";
    int index = 0;
    int mul = 1;
    int num = 0;

    int line = lengthString(input);

    for (int i = pos; i < line; i++) {
        string[index++] = input[i];
    }

    string[index]= '0';

    int len = lengthString(string);

    for (int i = 0; i < (len - 1); i++) {
        if (string[i] != ',') {
            num *= 10;
            num += (string[i] - '0');
        } else {
            mul *= num;
            num = 0;
        }
    }
    mul *= num;
    return mul;
}

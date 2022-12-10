#include "stdio.h"
#include "stdarg.h"

#include "../../kernel/drivers/display.h"

const char hex_chars[] = "0123456789abcdef";

void printf_unsigned(unsigned long long number, int radix) {
    char buffer[32];
    int pos = 0;

    // convert number to ASCII
    do {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = hex_chars[rem];
    } while (number > 0);

    // print number in reverse order
    while (--pos >= 0) {
        display::print(buffer[pos]);
    }
}

void printf_signed(long long number, int radix) {
    if (number < 0) {
        display::print('-');
        printf_unsigned(-number, radix);
    } else {
        printf_unsigned(number, radix);
    }
}

/**
 * Prints a formatted string to the display.
 *
 * @param format The format string.
 * @param ... The arguments to the format string.
 * @example printf("Hello %s!", "world"); // prints "Hello world!" to the display.
*/
void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;
    int radix = 10;
    bool sign = false;
    bool number = false;

    while (*format) {
        switch (state) {
            case PRINTF_STATE_NORMAL:
                switch (*format) {
                    case '%':   state = PRINTF_STATE_LENGTH; break;
                    default:    display::print(*format); break;
                }
                break;

            case PRINTF_STATE_LENGTH:
                switch (*format)
                {
                    case 'h':
                        length = PRINTF_LENGTH_SHORT;
                        state = PRINTF_STATE_LENGTH_SHORT;
                        break;
                    case 'l':
                        length = PRINTF_LENGTH_LONG;
                        state = PRINTF_STATE_LENGTH_LONG;
                        break;
                    default:
                        goto PRINTF_STATE_SPEC_;
                }
                break;

            case PRINTF_STATE_LENGTH_SHORT:
                if (*format == 'h') {
                    length = PRINTF_LENGTH_SHORT_SHORT;
                    state = PRINTF_STATE_SPEC;
                } else {
                    goto PRINTF_STATE_SPEC_;
                }
                break;

            case PRINTF_STATE_LENGTH_LONG:
                if (*format == 'l') {
                    length = PRINTF_LENGTH_LONG_LONG;
                    state = PRINTF_STATE_SPEC;
                } else {
                    goto PRINTF_STATE_SPEC_;
                }
                break;

            case PRINTF_STATE_SPEC:
            PRINTF_STATE_SPEC_:
                switch (*format)  {
                    // writes a single character
                    case 'c':   display::print((char)va_arg(args, int)); break;

                    // writes a character string
                    case 's':   display::print(va_arg(args, const char*)); break;

                    case '%':   display::print('%'); break;

                    // converts a signed integer to decimal representation
                    case 'd':
                    case 'i':   radix = 10; sign = true; number = true; break;

                    // converts an unsigned integer to decimal representation
                    case 'u':   radix = 10; sign = false; number = true; break;

                    case 'X':
                    case 'x':
                    case 'p':   radix = 16; sign = false; number = true; break;

                    // converts an unsigned integer to octal representation
                    case 'o':   radix = 8; sign = false; number = true; break;

                    // ignore invalid spec
                    default:    break;
                }

                if (number) {
                    if (sign) {
                        switch (length) {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_signed(va_arg(args, int), radix); break;
                            case PRINTF_LENGTH_LONG:        printf_signed(va_arg(args, long), radix); break;
                            case PRINTF_LENGTH_LONG_LONG:   printf_signed(va_arg(args, long long), radix); break;
                        }
                    } else {
                        switch (length) {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_unsigned(va_arg(args, unsigned int), radix); break;
                            case PRINTF_LENGTH_LONG:        printf_unsigned(va_arg(args, unsigned long), radix); break;
                            case PRINTF_LENGTH_LONG_LONG:   printf_unsigned(va_arg(args, unsigned long long), radix); break;
                        }
                    }
                }

                // reset state
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                radix = 10;
                sign = false;
                number = false;
                break;
        }
        format++;
    }
    va_end(args);
}

/**
 * Put a character to the output stream.
 * @param c Character to put.
*/
void putc(int c) {
    printf("%c", c);
}

/**
 * Put a string to the output stream.
 * @param s String to put
*/
void puts(const char *s) {
    printf("%s\n", s);
}

#ifndef LOGGING_H_
#define LOGGING_H_

#include "types.h"

class logger {
    public:
        static void log(const char* decorator, uint_8 dec_color, const char* message, uint_8 msg_color);
        static void success(const char* msg);
        static void error(const char* msg);
        static void warning(const char* msg);
        static void info(const char* msg);
        static void process(const char* msg);
};

#endif /* LOGGING_H_ */

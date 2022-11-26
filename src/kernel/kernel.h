#ifndef KERNEL_H
#define KERNEL_H

#include "../common/libc/string.h"
#include "../common/typedefs.h"

class kernel {
    public:
        static void initialize();
        static void finalize();
        static void panic(string msg, uint_32 rebootTime);
};

#endif /* KERNEL_H */

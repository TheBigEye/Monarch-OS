#ifndef KERNEL_H
#define KERNEL_H

#include "../common/monarch.h"
#include "../common/stdlib.h"

class kernel {
    public:
        static void initialize();
        static void finalize();
};

#endif /* KERNEL_H */

#ifndef SHELL_H_
#define SHELL_H_

#include "../../common/monarch.h"

class shell {
    public:
        static void initialize();
};

class draw {
    public:
        static void filled_box(char chr, uint_8 color);
        static void unfilled_box(char chr, uint_8 color);
};

#endif /* SHELL_H_ */

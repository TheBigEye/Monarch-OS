#ifndef POWER_H_
#define POWER_H_

#include "../../common/monarch.h"

class power {
    public:
        static void reboot(uint_32 time = 2000);
        static void shutdown(uint_32 time = 2000);
};

#endif /* POWER_H_ */

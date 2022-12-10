#include "kernel.h"

#include "shell/shell.h"

extern "C" void _start() {
    kernel::initialize(); // Initialize kernel modules
    shell::initialize(); // Initialize shell
    return;
}

extern "C" void _end() {
    return;
}

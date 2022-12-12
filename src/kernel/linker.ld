OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT (
    /* bootloader */
    ./build/main.o
    ./build/binaries.o

    /* libc */
    ./build/cmath.o
    ./build/logging.o
    ./build/cstdio.o
    ./build/cstring.o

    /* kernel */
    ./build/IDT.o
    ./build/IO.o
    ./build/CPU.o
    ./build/map.o
    ./build/heap.o
    ./build/memory.o
    ./build/keyboard.o
    ./build/display.o
    ./build/power.o
    ./build/command.o
    ./build/shell.o
    ./build/kernel.o
    ./build/kmain.o
)

OUTPUT (./build/kernel.bin)

/* Valid definitions... 0x8000 = 1M | 0x1000 = 4K, 4096 */

SECTIONS {
    /* Kernel */
    . = 0x8000;

    .text : ALIGN(0x1000) {
        *(.text)
    }

    .idt BLOCK(0x1000) : ALIGN(0x1000) {
        _idt = .;
        . = . + 0x1000;
    }

    /* Read only data */
    .rodata : ALIGN(0x1000) {
        *(.rodata)
    }

    /* Read-write data */
    .data : ALIGN(0x1000) {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss : ALIGN(0x1000) {
        *(COMMON)
        *(.bss)
    }
}

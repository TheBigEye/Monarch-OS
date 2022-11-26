OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT (
    /* bootloader */
    ./build/main.o
    ./build/binaries.o

    /* libc */
    ./build/math.o
    ./build/logging.o
    ./build/stdio.o
    ./build/string.o

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

OUTPUT (
    ./build/kernel.bin
)

SECTIONS {
    . = 0x8000;

    .text : ALIGN(0x1000) {
        *(.text)
    }

    .idt BLOCK(0x1000) : ALIGN(0x1000) {
        _idt = .;
        . = . + 0x1000;
    }

    /* Read-write data (initialized) */
    .data : ALIGN(0x1000) {
        *(.data)
    }

    /* Read-only data. */
    .rodata : ALIGN(0x1000) {
        *(.rodata)
    }

    /* Read-write data (uninitialized) and stack */
    .bss : ALIGN(0x1000) {
        *(COMMON)
        *(.bss)
    }
}

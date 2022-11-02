OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT (
    ./build/main.o
    ./build/binaries.o
    ./build/kernel.o
    ./build/IDT.o
    ./build/IO.o
    ./build/map.o
    ./build/heap.o
    ./build/memory.o
    ./build/keyboard.o
    ./build/display.o
    ./build/power.o
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

    .data : ALIGN(0x1000) {
        *(.data)
    }

    .rodata : ALIGN(0x1000) {
        *(.rodata)
    }

    .bss : ALIGN(0x1000) {
        *(COMMON)
        *(.bss)
    }
}

OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT (
    ./build/ExtendedProgram.o
    ./build/Kernel.o
    ./build/IDT.o
    ./build/IO.o
    ./build/MemoryMap.o
    ./build/Heap.o
    ./build/Memory.o
    ./build/Keyboard.o
    ./build/Screen.o
    ./build/binaries.o
)

OUTPUT (
    ./build/Kernel.bin
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

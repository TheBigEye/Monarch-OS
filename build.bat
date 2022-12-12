::
:: This script is TOO important, compile all the code and make the OS floppy image
::

@echo off

:: Why batch? Cmake sucks, ok no

:: Build settings (TOOLS PATHS)
:: NOTE: If you use a cross compiler under WSL:
:: - Create a new environment variable called "WSLENV"
:: - In your distro's terminal, type "explorer.exe ." that will open the windows file explorer into the location of your distro files
:: - in the environment variable you put the location of the compiler binaries, example: /usr/bin/local/x86_64elfgcc/bin
:: - and here, in GCC you put, for example: wsl $WSLENV/x86_64-elf-gcc

set CFLAGS= -ffunction-sections -fdata-sections -w -Os -Wl,--gc-sections
set LDFLAGS= -nostdlib -z max-page-size=0x1000 -Ttext 0x8000
set ARCH= -m64

set ASM= nasm
set GCC= x86_64-elf-gcc
set LD= x86_64-elf-ld

:: --------------------------------------------------------------------------------------------------------------------------------

cls

:: WARNING: this remove all from inside of build folder
echo [-] Pre-cleaning build folder ...
if not exist build      (mkdir build)
if exist build          (del /s /q build\*.*)

:: --------------------------------------------------------------------------------------------------------------------------------

echo [-] Assembling bootloader ...
%ASM% src/boot/bootloader.asm -f bin                                                             -o build/bootloader.bin
%ASM% src/boot/zeroes.asm -f bin                                                                 -o build/zeroes.bin
%ASM% src/boot/main.asm -f elf64                                                                 -o build/main.o
%ASM% src/boot/disk/binaries.asm -f elf64                                                        -o build/binaries.o

echo [-] Compiling system kernel ...
:: NOTE: change or add objects require modify also INPUT on linker.ld

:: 0x8000 specifies where the code expects to find itself
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/cpu/IDT.cpp"                -o "build/IDT.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/cpu/IO.cpp"                 -o "build/IO.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/cpu/CPU.cpp"                -o "build/CPU.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/memory/map.cpp"             -o "build/map.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/memory/heap.cpp"            -o "build/heap.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/memory/memory.cpp"          -o "build/memory.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/drivers/keyboard.cpp"       -o "build/keyboard.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/drivers/display.cpp"        -o "build/display.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/power/power.cpp"            -o "build/power.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/shell/command.cpp"          -o "build/command.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/shell/shell.cpp"            -o "build/shell.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/kernel.cpp"                 -o "build/kernel.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/kmain.cpp"                  -o "build/kmain.o"

:: Standard monarch (common) Library
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/sys/logging.cpp"            -o "build/logging.o"

:: libc
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/libc/cmath.cpp"              -o "build/cmath.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/libc/cstdio.cpp"             -o "build/cstdio.o"
%GCC% %CFLAGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/libc/cstring.cpp"            -o "build/cstring.o"

:: --------------------------------------------------------------------------------------------------------------------------------

echo [-] Linking kernel ...
%LD% %LDFLAGS% -T "src/kernel/linker.ld"

cd ./build
echo:

:: TODO: make an valid floppy image for Virtual Box :(
echo [-] Making OS floppy image ...
copy /b bootloader.bin+kernel.bin+zeroes.bin Monarch-OS.img
echo [o] Output file: Monarch-OS.img
echo:
cd ..

:: --------------------------------------------------------------------------------------------------------------------------------

@echo off

:: Why batch? Cmake sucks, ok no

:: Build settings (TOOLS PATHS)
:: NOTE: If you use a cross compiler under WSL:
:: - Create a new environment variable called "WSLENV"
:: - In your distro's terminal, type "explorer.exe ." that will open the file explorer into the location of your distro files
:: - in the environment variable you put the location of the compiler binaries, example: /usr/bin/local/x86_64elfgcc/bin
:: - and here, in GCC you put, for example: wsl $WSLENV/x86_64-elf-gcc

set ARGS= -w -Os
set ARCH= -m64

set ASM= nasm
set GCC= x86_64-elf-gcc
set LD= x86_64-elf-ld

:: ----------------------------------------------------------------------------------------------------------------------------

echo [-] Pre-cleaning build folder ...
if not exist build      (mkdir build)
if exist build\*.bin    (del /s /q build\*.bin)
if exist build\*.img    (del /s /q build\*.img)
if exist build\*.o      (del /s /q build\*.o)
if exist build\*.tmp    (del /s /q build\*.tmp)

:: ----------------------------------------------------------------------------------------------------------------------------

echo [-] Assembling bootloader ...
%ASM% src/boot/bootloader.asm -f bin                                                             -o build/bootloader.bin
%ASM% src/boot/main.asm -f elf64                                                                 -o build/main.o
%ASM% src/boot/disk/binaries.asm -f elf64                                                        -o build/binaries.o

echo [-] Compiling system kernel ...
:: NOTE: change or add objects require modify also INPUT on linker.ld

%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/cpu/IDT.cpp"                -o "build/IDT.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/cpu/IO.cpp"                 -o "build/IO.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/cpu/CPU.cpp"                -o "build/CPU.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/memory/map.cpp"             -o "build/map.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/memory/heap.cpp"            -o "build/heap.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/memory/memory.cpp"          -o "build/memory.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/drivers/keyboard.cpp"       -o "build/keyboard.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/drivers/display.cpp"        -o "build/display.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/power/power.cpp"            -o "build/power.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/shell/command.cpp"          -o "build/command.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/shell/shell.cpp"            -o "build/shell.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/kernel.cpp"                 -o "build/kernel.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/kernel/kmain.cpp"                  -o "build/kmain.o"

:: Standard (common) Library
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/math.cpp"                   -o "build/math.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/logging.cpp"                -o "build/logging.o"

:: libc
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/libc/stdio.cpp"             -o "build/stdio.o"
%GCC% %ARGS% -Ttext 0x8000 -ffreestanding -mno-red-zone %ARCH% -c "src/common/libc/string.cpp"            -o "build/string.o"

:: ----------------------------------------------------------------------------------------------------------------------------

echo [-] Linking kernel ...
%LD% -T "src/kernel/linker.ld"

cd ./build

:: TODO: make an valid floppy image for Virtual Box :(

echo [-] Making floppy image ...
echo:
echo [o] Output:
copy /b bootloader.bin+kernel.bin Monarch-OS.img
cd ..
echo copied into Monarch-OS.img

:: ----------------------------------------------------------------------------------------------------------------------------

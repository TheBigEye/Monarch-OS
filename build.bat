@echo off

:: Why batch? Cmake sucks, ok no

:: Build settings (TOOLS PATHS)
:: NOTE: If you use a cross compiler under WSL:
:: - Create a new environment variable called "WSLENV"
:: - In your distro's terminal, type "explorer.exe ." that will open the file explorer into the location of your distro files
:: - in the environment variable you put the location of the compiler binaries, example: /usr/bin/local/x86_64elfgcc/bin
:: - and here, in GCC you put, for example: wsl $WSLENV/x86_64-elf-gcc

set ASM= nasm
set GCC= x86_64-elf-gcc -w -Os
set LD= x86_64-elf-ld

:: ----------------------------------------------------------------------------------------------------------------------------

echo [-] Pre-cleaning build folder ...
if not exist build      (mkdir build)
if exist build\*.bin    (del /s /q build\*.bin)
if exist build\*.img    (del /s /q build\*.img)
if exist build\*.o      (del /s /q build\*.o)
if exist build\*.tmp    (del /s /q build\*.tmp)

:: ----------------------------------------------------------------------------------------------------------------------------

echo [-] Assembling project code ...
%ASM% src/boot/bootloader.asm -f bin                                                -o build/bootloader.bin
%ASM% src/boot/main.asm -f elf64                                                    -o build/main.o
%ASM% src/boot/disk/binaries.asm -f elf64                                           -o build/binaries.o

echo [-] Compiling project code ...
:: NOTE: change or add objects require modify also INPUT on Kernel.ld
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/kernel.cpp"           -o "build/kernel.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/IDT.cpp"              -o "build/IDT.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/IO.cpp"               -o "build/IO.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/CPU.cpp"              -o "build/CPU.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/memory/map.cpp"       -o "build/map.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/memory/heap.cpp"      -o "build/heap.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/memory/memory.cpp"    -o "build/memory.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/drivers/keyboard.cpp" -o "build/keyboard.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/drivers/display.cpp"  -o "build/display.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/power/power.cpp"      -o "build/power.o"

:: Standard (common) Library
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/common/math.cpp"      -o "build/math.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/common/logging.cpp"   -o "build/logging.o"

:: ----------------------------------------------------------------------------------------------------------------------------

echo [-] Linking kernel ...
%LD% -T "src/kernel.ld"

cd ./build

:: TODO: make an valid floppy image for Virtual Box :(

echo [-] Making floppy image ...
echo:
echo [o] Output:
copy /b bootloader.bin+kernel.bin Monarch-OS.img
cd ..
echo copied into Monarch-OS.img

:: ----------------------------------------------------------------------------------------------------------------------------

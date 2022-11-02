@echo off

:: Why batch? Cmake sucks, ok no

:: Build settings (TOOLS PATHS)
:: NOTE: If you use a cross compiler under WSL:
:: - Create a new environment variable called "WSLENV"
:: - In your distro's terminal, type "explorer.exe ." that will open the file explorer into the location of your distro files
:: - in the environment variable you put the location of the compiler binaries, example: /usr/bin/local/x86_64elfgcc/bin
:: - and here, in GCC you put, for example: wsl $WSLENV/x86_64-elf-gcc
set ASM= nasm
set GCC= wsl $WSLENV/x86_64-elf-gcc
set LD= wsl $WSLENV/x86_64-elf-ld

echo [-] Pre-cleaning build folder ...
if not exist build      (mkdir build)
if exist build\*.bin    (del /s /q build\*.bin)
if exist build\*.img    (del /s /q build\*.img)
if exist build\*.o      (del /s /q build\*.o)
if exist build\*.tmp    (del /s /q build\*.tmp)

echo [-] Assembling project code ...
%ASM% src/boot/bootloader.asm -f bin -o build/bootloader.bin
%ASM% src/core/ExtendedProgram.asm -f elf64 -o build/ExtendedProgram.o
%ASM% src/boot/binaries.asm -f elf64 -o build/binaries.o

echo [-] Compiling project code ...
:: NOTE: change or add objects require modify also INPUT on Kernel.ld
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/Kernel.cpp" -o "build/Kernel.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/IDT.cpp" -o "build/IDT.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/IO.cpp" -o "build/IO.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/MemoryMap.cpp" -o "build/MemoryMap.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/Heap.cpp" -o "build/Heap.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/Memory.cpp" -o "build/Memory.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/drivers/Keyboard.cpp" -o "build/Keyboard.o"
%GCC% -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "src/drivers/Screen.cpp" -o "build/Screen.o"

echo [-] Linking kernel ...
%LD% -T"Kernel.ld"

cd ./build

:: TODO: make an real floppy image :(

echo [-] Making floppy image ...
echo:
echo [o] Output:
copy /b bootloader.bin+kernel.bin MonarchOS.img
cd ..
echo copied into MonarchOS.img
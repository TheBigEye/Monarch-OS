::
:: This script is used for simplify the OS run task
::
:: Requeriments:
:: - Have QEMU installed
:: - Have a brain

@echo off

:: Set the floppy image path
set IMG=./build/Monarch-OS.img

:: Set QEMU vm argumnets
set QEMU_ARGS= -drive format=raw,file="%IMG%",index=0,if=floppy, -m 128M

::echo [r] Starting bochs debugger ...
::bochs -q -f ./config/bochsconfig -rc ./config/commands.txt

cls

:: Check if exist the floppy image
if not exist %IMG% (
    echo [!] Could not find floppy image!
    exit /b
)

:: QEMU CHECKS --------------------------------------------------------------------------------------------------------------------

:: Under windows
if exist "C:\Program Files\qemu\" (
    echo [r] Starting qemu emulation ...
    "C:\Program Files\qemu\qemu-system-x86_64.exe" %QEMU_ARGS%
    exit /b
)

:: Under MSYS2
if exist "C:\msys64\mingw64\bin\" (
    echo [r] Starting qemu emulation ...
    "C:\msys64\mingw64\bin\qemu-system-x86_64" %QEMU_ARGS%
    exit /b
)

:: If previous runs failed ...
echo [!] Could not fin QEMU installation!

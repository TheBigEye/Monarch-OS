@echo off

set IMG=./build/Monarch-OS.img
set DEBUG_QUEMU_ARGS= -m 128 -device VGA,vgamem_mb=64 -no-reboot -d int
set NORMAL_QEMU_ARGS= -drive format=raw,file="%IMG%",index=0,if=floppy, -m 128M

::echo [r] Starting bochs debugger ...
::bochs -q -f ./config/bochsconfig -rc ./config/commands.txt

echo [r] Starting qemu emulation ...
if exist "C:\Program Files\qemu\" (
    "C:\Program Files\qemu\qemu-system-x86_64.exe" %NORMAL_QEMU_ARGS%
) else (
    if exist "C:\msys64\mingw64\bin\" (
        "C:\msys64\mingw64\bin\qemu-system-x86_64" %NORMAL_QEMU_ARGS%
    ) else (
        qemu-system-x86_64 %NORMAL_QEMU_ARGS%
    )
)

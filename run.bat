@echo off

set IMG= "./build/Monarch-OS.img"

::echo [r] Starting bochs debugger ...
::bochs -q -f ./config/bochsconfig -rc ./config/commands.txt

echo [r] Starting qemu emulation ...
if exist "C:\Program Files\qemu\" (
    "C:\Program Files\qemu\qemu-system-x86_64.exe" -fda %IMG% -m 128 -device VGA,vgamem_mb=64
) else (
    if exist "C:\msys64\mingw64\bin\" (
        "C:\msys64\mingw64\bin\qemu-system-x86_64" -fda %IMG% -m 128 -device VGA,vgamem_mb=64
    ) else (
        qemu-system-x86_64 -fda %IMG% -m 128 -device VGA,vgamem_mb=64
    )
)

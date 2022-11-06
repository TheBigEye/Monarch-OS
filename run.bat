@echo off

:: NOTE: you also can try on Virtual Box :)

::echo [r] Starting bochs debugger ...
::bochs -q -f ./bochs/bochsconfig -rc ./bochs/commands.txt

echo [r] Starting qemu emulation ...
qemu-system-x86_64 -fda build/Monarch-OS.img

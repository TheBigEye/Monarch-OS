@echo off

if exist build\*.bin (del /s /q build\*.bin)
if exist build\*.img (del /s /q build\*.img)
if exist build\*.o (del /s /q build\*.o)
if exist build\*.tmp ( del /s /q build\*.tmp)

echo [d] Removed all the object files from /build

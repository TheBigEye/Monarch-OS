::
:: This script is used for remove all the compilation-time generated files
::

@echo off

cls

:: Check if exist build folder
if exist build (
    del /s /q build\*.*
    rmdir build
    echo [d] Removed build output folder
    exit /b

) else (
    echo [!] There is no build folder :D
    exit /b
)

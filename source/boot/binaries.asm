; IMPORTANT!: When we modify some included binary files, we have to recompile all again

%macro INCLUDE_BIN 2
    %define symbol %1
    %define path %2

    SECTION .rodata                       ; create a new section
    GLOBAL symbol                         ; make the symbol global

    ; define the symbol
    symbol:
        incbin path                       ; include the file
        db 0                              ; add a null terminator

        %1_size: dq %1_size - %1          ; define the size of the file
%endmacro

; include the kernel poem text
INCLUDE_BIN bootpoem, "source/binaries/strings/bootpoem.txt"

; include the ascii art
INCLUDE_BIN butterfly, "source/binaries/strings/butterfly.txt"

; include the "It's now safe to turn off your computer"
INCLUDE_BIN turnoffpc, "source/binaries/strings/turnoffpc.txt"

; include the chess wallapaper :)
INCLUDE_BIN wallpaper, "source/binaries/wallpaper.bin"

; include the leafs wallpaper ._.
INCLUDE_BIN hillpaper, "source/binaries/hillpaper.bin"

; include the Windows Chicago wallpaper :D
INCLUDE_BIN workpaper, "source/binaries/workpaper.bin"

; include more files to be used, an example can be found in binaries.h

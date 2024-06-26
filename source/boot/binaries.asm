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

; include the ascii art
INCLUDE_BIN butterfly, "source/binaries/butterfly.txt"

; include the chess wallapaper :)
INCLUDE_BIN wallpaper, "source/binaries/wallpaper.bin"

; include the leafs wallpaper ._.
INCLUDE_BIN leafpaper, "source/binaries/leafpaper.bin"

; include the Windows Chicago wallpaper :D
INCLUDE_BIN workpaper, "source/binaries/workpaper.bin"

; include more files to be used, an example can be found in binaries.h

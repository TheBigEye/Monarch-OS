; START -------------------------------------------------------------------------------------------------------

[org 0x7c00]                ; set the origin of the program to 0x7c00

mov [BOOT_DISK], dl

; set up the stack
mov bp, 0x7c00              ; set the base pointer to 0x7c00
mov sp, bp                  ; set the stack pointer to the base pointer


; CODE --------------------------------------------------------------------------------------------------------

; read the disk
call read_disk

jmp PROGRAM_SPACE

%include "src/boot/print.asm"
%include "src/boot/diskRead.asm"


; END ---------------------------------------------------------------------------------------------------------

; fill the rest of the sector with zeros
times 510-($-$$) db 0

; magic number
dw 0xaa55 ; set the last two bytes to 0xaa55

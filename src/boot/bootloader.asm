[ORG 0x7C00]                ; Set the origin of the program to 0x7C00

; Save the boot disk number in the BOOT_DISK variable
mov [BOOT_DISK], dl

; Setup the stack
mov bp, 0x7c00              ; Set the base pointer to 0x7C00
mov sp, bp                  ; Set the stack pointer to the base pointer

; Clear the BIOS screen
call cls

; Set video mode to 320x200x256 (pixels)
call video_mode

; Set video mode to 80x25x16 (text only)
call text_mode

; Print the bootloader version message
mov bx, MSG_bootloader_version
call print

; Print a newline character
mov bx, nl
call print

; Read the disk sectors
call disk_read

; Jump to the program space
jmp PROGRAM_SPACE

%include "src/boot/screen/print.asm"
%include "src/boot/screen/mode.asm"
%include "src/boot/disk/disk.asm"

MSG_bootloader_version:     db 'Butterfly bootloader v2', 0
nl: db 0x0A, 0x0D, 0

; Fill the left sectors with zeros
times 510-($-$$) db 0

; Magic number
dw 0xAA55 ; Set the last two bytes to 0xAA55

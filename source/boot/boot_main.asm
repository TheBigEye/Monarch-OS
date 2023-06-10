[ORG 0x7C00]    ; set up start address of bootloader

; Define some constants
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel
SECTORS_TO_READ equ 31   ; Sectors to be read by the bootloader

; Save the boot drive number in memory
mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
mov bp, 0x9000

xor ax, ax     ; AX=0 (XOR register to itself clears all bits)
mov es, ax     ; ES=0
mov ds, ax     ; DS=0
mov ss, ax     ; SS=0
mov sp, bp

; Set up screen
call CLEAR_SCREEN
call VIDEO_MODE
call TEXT_MODE

; Print a message on the screen
mov bx, MSG_REAL_MODE
call PRINT_16
call NEWLINE_16

; Load kernel from disk and jump to protected mode
call LOAD_KERNEL
call JUMP_PROTECTED
jmp $ ; Never executed

; Include other source files needed for the bootloader
%include "source/boot/screen/print_16.asm"
%include "source/boot/screen/print_32.asm"
%include "source/boot/screen/mode.asm"
%include "source/boot/disk/disk.asm"
%include "source/boot/bits/GDT.asm"
%include "source/boot/protected.asm"

; 16-bit real mode kernel loader
[BITS 16]
LOAD_KERNEL:
    ; Print a message on the screen
    mov bx, MSG_LOAD_KERNEL
    call PRINT_16
    call NEWLINE_16

    ; Read kernel from disk and store in memory at 0x1000
    MOV bx, KERNEL_OFFSET
    MOV dh, SECTORS_TO_READ
    MOV dl, [BOOT_DRIVE]
    CALL DISK_LOAD
    RET

; 32-bit protected mode
[BITS 32]
LOAD_PROTECTED:
    ; Print a message on the screen and give control to the kernel
    MOV EBX, MSG_PROTECTED_MODE
    CALL PRINT_32
    CALL KERNEL_OFFSET
    JMP $ ; Stay here when the kernel returns control to us (if ever)

; Define some variables and messages in memory
BOOT_DRIVE           db 0
MSG_REAL_MODE        db "[i] 16-bit real mode loaded!", 0
MSG_PROTECTED_MODE   db "[i] 32-bit protected mode loaded!", 0
MSG_LOAD_KERNEL      db "Loading kernel", 0

; Boot sector padding
TIMES 510 - ($-$$) DB 0
DW 0xAA55

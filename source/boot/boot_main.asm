;-----------------------------------------------------------------------------;
;                          Butterfly Kernel Bootloader                        ;
;-----------------------------------------------------------------------------;

; Set up start address of bootloader
[ORG 0x7C00] ; 0x7c00 > Boot & 0x7c00 + 512 (0x7e00) > Kernel

;-------------------------------------;
;              Constants              ;
;-------------------------------------;
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel
BOOT_SECTORS  equ 50     ; Sectors to be read by the bootloader
BOOT_DRIVE    equ 0      ; (0 = floppy A, 1 = floppy B, 0x80 = HDD, 0x81 = HDD 2)

;-------------------------------------;
;        Save Boot Drive Number       ;
;-------------------------------------;
mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot

;-------------------------------------;
;      Set Up Segment Registers       ;
;-------------------------------------;
cli            ; disable interrupts
xor ax, ax     ; set ax register to 0
mov sp, ax     ; set stack pointer (sp) to 0
mov ds, ax     ; set data segment (ds) to 0
mov es, ax     ; set extra segment (es) to 0
mov ss, ax     ; set stack segment (es) to 0

mov sp, 0x9000 ; set stack pointer (sp) to the base pointer address 0x9000
sti            ; enable interrupts

;-------------------------------------;
;           Set Up Screen             ;
;-------------------------------------;
call CLEAR_SCREEN
call TEXT_MODE

; Print a message on the screen
mov bx, MSG_REAL_MODE
call PRINT_16
call NEWLINE_16

;-------------------------------------;
; Load Kernel from Disk and Jump      ;
;        to Protected Mode            ;
;-------------------------------------;
call LOAD_KERNEL
call JUMP_PROTECTED
jmp $ ; Never executed

;-------------------------------------;
;    Include External Assembly Files  ;
;-------------------------------------;
%include "source/boot/screen/print_16.asm"
%include "source/boot/screen/print_32.asm"
%include "source/boot/screen/mode.asm"
%include "source/boot/disk/disk.asm"
%include "source/boot/bits/GDT.asm"
%include "source/boot/protected.asm"

;-------------------------------------;
;   16-bit Real Mode Kernel Loader    ;
;-------------------------------------;
[BITS 16]
LOAD_KERNEL:
    ;---------------------------------;
    ;   Print a Message on the Screen ;
    ;---------------------------------;
    mov bx, MSG_LOAD_KERNEL
    call PRINT_16
    call NEWLINE_16

    ;---------------------------------;
    ; Read Kernel from Disk and Store ;
    ;    in Memory at 0x1000          ;
    ;---------------------------------;
    mov bx, KERNEL_OFFSET
    mov dh, BOOT_SECTORS
    mov dl, [BOOT_DRIVE]
    call DISK_LOAD
    ret

;-------------------------------------;
;          32-bit Protected Mode      ;
;-------------------------------------;
[BITS 32]
LOAD_PROTECTED:
    ;---------------------------------;
    ;   Print a Message on the Screen ;
    ;     and Give Control to the     ;
    ;             Kernel              ;
    ;---------------------------------;
    mov ebx, MSG_PROTECTED_MODE
    call PRINT_32
    call KERNEL_OFFSET
    jmp $ ; Stay here when the kernel returns control to us (if ever)

;-------------------------------------;
;  Define Variables and Messages      ;
;            in Memory                ;
;-------------------------------------;
MSG_REAL_MODE        db "- Welcome to Butterfly Bootloader! -", 0
MSG_PROTECTED_MODE   db "[i] Entering Protected Mode ...", 0
MSG_LOAD_KERNEL      db "[i] Loading Butterfly Kernel ...", 0

;-------------------------------------;
; Fill the Remaining Bytes of the     ;
;       Boot Sector with 0            ;
;-------------------------------------;
TIMES 510 - ($-$$) db 0
dw 0xAA55 ; boot signature (0xAA55)

; 'incbin' saves us from having to concatenate two
; separate binaries manually to create the floppy image
incbin "kernel.bin"

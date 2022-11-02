PROGRAM_SPACE equ 0x8000

Read_disk:
    mov ah, 0x02
    mov bx, PROGRAM_SPACE
    mov al, 64                  ; Sectors to read, change it if you want
    mov dl, [BOOT_DISK]
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02

    int 0x13

    jc Disk_read_fail
    call Disk_read_success

    ret

Disk_read_fail:
    mov bx, MSG_space
    call Printf
    mov bx, MSG_disk_read_fail
    call Printf

    jmp $

Disk_read_success:
    mov bx, MSG_space
    call Printf
    mov bx, MSG_disk_read_success
    call Printf

BOOT_DISK:
    db 0

MSG_space:              db ' ', 0
MSG_disk_read_fail:     db '- Oops, we cant read the disc!', 0
MSG_disk_read_success:  db '- The disc was successfully read :)', 0

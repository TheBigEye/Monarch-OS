PROGRAM_SPACE equ 0x8000

read_disk:
    mov ah, 0x02
    mov bx, PROGRAM_SPACE
    mov al, 64                  ; Sectors to read, change it if you want
    mov dl, [BOOT_DISK]
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02

    int 0x13

    jc disk_read_fail

    ret

BOOT_DISK:
    db 0

msg_disk_read_fail:     db 'Oops!, disk read has failed!',  0

disk_read_fail:
    mov bx, msg_disk_read_fail
    call print

    jmp $

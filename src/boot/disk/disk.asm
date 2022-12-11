PROGRAM_SPACE equ 0x8000   ; Kernel address

; BUG BUG: CANNOT READ BEYOND 64 SECTORS :(
disk_read:
    mov bx, PROGRAM_SPACE  ; Buffer to be read
    mov dh, 64             ; Sectors to read (change it if you want)

    mov ah, 0x02           ; BIOS disk service: read disk sectors
    mov al, dh             ; Sectors number
    mov ch, 0x00           ; Cylinder number
    mov dh, 0x00           ; Head number
    mov cl, 0x02           ; Sector number
    mov dl, [BOOT_DISK]    ; Disk to be read

    int 0x13

    jc disk_read_fail
    call disk_read_success

    ret

BOOT_DISK:
    db 0

; Print fail message if the disk could not be read
disk_read_fail:
    ; Print the error
    mov bx, MSG_DISK_READ_FAIL
    call print
    ; Print a space separation
    mov bx, MSG_SPACE
    call print

    jmp $

; Print sucess message if the disk is read
disk_read_success:
    mov bx, MSG_SPACE
    call print
    mov bx, MSG_DISK_READ_SUCCESS
    call print

; MESSAGES
MSG_SPACE:              db ' ', 0
MSG_DISK_READ_FAIL:     db '- OOPS, we cant read the disk!', 0
MSG_DISK_READ_SUCCESS:  db '- YES!, The disk was successfully read!', 0

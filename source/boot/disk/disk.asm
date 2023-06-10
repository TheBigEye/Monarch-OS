; load 'dh' sectors from drive 'dl' into ES:BX

DISK_LOAD:
    pusha
    ; reading from disk requires setting specific values in all registers
    ; so we will overwrite our input parameters from 'dx'. Let's save it
    ; to the stack for later use.
    push dx

    mov ah, 0x02 ; ah <- int 0x13 function. 0x02 = 'read'
    mov al, dh   ; al <- number of sectors to read (0x01 .. 0x80)
    mov cl, 0x02 ; cl <- sector (0x01 .. 0x11)

    ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')

    ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
    ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00 ; dh <- head number (0x0 .. 0xF)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int 0x13      ; BIOS interrupt
    jc DISK_ERROR ; if error (stored in the carry bit)

    pop dx
    cmp al, dh    ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne SECTORS_ERROR
    popa
    ret

DISK_ERROR:
    mov bx, DISK_READ_ERROR
    call PRINT_16
    call NEWLINE_16
    mov dh, ah ; ah = error code, dl = disk drive that dropped the error
    call PRINT_16_HEX ; check out the code at http://stanislavs.org/helppc/int_13-1.html
    jmp DISK_LOOP

    SECTORS_ERROR:
        mov bx, DISK_SECTORS_ERROR
        call PRINT_16

    DISK_LOOP:
        jmp $

DISK_READ_ERROR:    db "[!] Disk read operation error", 0
DISK_SECTORS_ERROR: db "[!] Disk incorrect sectors number!", 0

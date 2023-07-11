;-----------------------------------------------------------------------------;
;                        Bootloader Disk Load Function                        ;
;-----------------------------------------------------------------------------;

DISK_LOAD:
    pusha

    ; Reading from disk requires setting specific values in all registers.
    ; We will overwrite our input parameters from 'dx', so let's save it
    ; to the stack for later use.
    push dx

    mov ah, 0x02 ; AH <- INT 0x13 function. 0x02 = 'read'
    mov al, dh   ; AL <- Number of sectors to read (0x01 .. 0x80)
    mov cl, 0x02 ; CL <- Sector (0x01 .. 0x11)

    ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    xor ch, ch ; CH <- Cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')

    ; DL <- Drive number. Our caller sets it as a parameter and gets it from BIOS
    ; (0 = floppy, 1 = floppy2, 0x80 = HDD, 0x81 = HDD2)
    xor dh, dh ; DH <- Head number (0x0 .. 0xF)

    ; [ES:BX] <- Pointer to the buffer where the data will be stored.
    ; Caller sets it up for us, and it is actually the standard location for INT 0x13h.
    int 0x13     ; BIOS interrupt

    pop dx
    cmp al, dh   ; BIOS also sets 'AL' to the number of sectors read. Compare it.
    popa
    ret

; FUNCTIONS ---------------------------------------------------------------------------------------------------

; print a string
Printf:
    push ax
    push bx

    mov ah, 0x0e                ; set the high byte of the accumulator to 0x0e

    ; check if we reached the end of the string
    .Loop:
        ; if so, exit
        cmp [bx], byte 0        ; compare the byte at the address in bx to 0
        je .Exit                ; if they are equal, jump to the exit label

        ; otherwise, print the character and continue
        mov al, [bx]            ; set the low byte of the accumulator to the byte at the address in bx
        int 0x10                ; call the BIOS interrupt 0x10
        inc bx                  ; increment the base register
        jmp .Loop               ; jump to the loop label

    .Exit:
        pop ax
        pop bx
        ret


; VARIABLES ---------------------------------------------------------------------------------------------------

; the string to print
MSG_hello:      db 'This is a test string :>', 0

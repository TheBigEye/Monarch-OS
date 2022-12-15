; print a string
print:
    push ax
    push bx

    ; set the high byte of the accumulator to 0x0e
    mov ah, 0x0E

    ; check if we reached the end of the string
    .loop:
        ; if so, exit
        cmp [bx], byte 0        ; compare the byte at the address in bx to 0
        je .exit                ; if they are equal, jump to the exit label

        ; otherwise, print the character and continue
        mov al, [bx]            ; set the low byte of the accumulator to the byte at the address in bx
        int 0x10                ; call the BIOS interrupt 0x10
        inc bx                  ; increment the base register
        jmp .loop               ; jump to the loop label

    .exit:
        pop bx
        pop ax
        ret

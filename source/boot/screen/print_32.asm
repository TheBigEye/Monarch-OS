[BITS 32] ; using 32-bit protected mode

; this is how constants are defined
VIDEO_MEMORY equ 0xB8000
TEXT_COLOR equ 0x0F ; the color byte for each character

PRINT_32:
    pusha
    mov edx, VIDEO_MEMORY

    PRINT_32_LOOP:
        mov al, [ebx] ; [ebx] is the address of our character
        mov ah, TEXT_COLOR

        cmp al, 0 ; check if end of string
        je PRINT_32_DONE

        mov [edx], ax ; store character + attribute in video memory
        add ebx, 1 ; next char
        add edx, 2 ; next video memory position

        jmp PRINT_32_LOOP

    PRINT_32_DONE:
        popa
        ret

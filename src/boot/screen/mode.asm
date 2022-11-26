cls:
    mov ax, 0x03
    int 0x10
    ret

video_mode:
    mov ax, 0x13
    int 0x10
    ret

text_mode:
    mov ax, 1112h
    xor bl, bl
    int 10h
    ret

disable_cursor:
    mov ah, 0x01
    mov bh, 0x00
    mov cx, 0x2000
    int 0x10
    ret


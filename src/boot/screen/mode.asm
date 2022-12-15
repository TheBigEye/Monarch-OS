; clear screen
cls:
    mov ax, 0x03
    int 0x10
    ret

; set video mode to 320x200x256 (pixels)
video_mode:
    mov ax, 0x13
    int 0x10
    ret

; set video mode to 80x25x16 (text only)
text_mode:
    mov ax, 1112h
    xor bl, bl
    int 10h
    ret

; disable cursor (hide)
disable_cursor:
    mov ah, 0x01
    mov bh, 0x00
    mov cx, 0x2000
    int 0x10
    ret

; change pallete
change_pallete:
    mov ah, 0x0B
    mov bh, 0x00
    mov bl, 0x01
    int 0x10
    ret

; 1 Clear the screen (but it sets the video mode to 80x60)
CLEAR_SCREEN:
    mov ah, 0x00
    mov al, 0x12
    ; mov ax, 0x0003
    int 0x10
    ret

; 2 Video mode
VIDEO_MODE:
    mov ax, 0x13
    int 0x10
    ret

; 3 try set 80x50 text mode and 8x8 font
TEXT_MODE:
    mov ax, 0x1112
    xor bl, bl
    int 0x10

    ret

; NOTE, the OS will be reset the mode to 90x60!

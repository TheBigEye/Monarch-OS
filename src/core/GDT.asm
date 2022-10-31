GDT_nulldesc:
    dd 0
    dd 0

GDT_codedesc:
    dw 0xFFFF               ; Limit
    dw 0x0000               ; Base (low)
    db 0x00                 ; Base (medium)
    db 10011010b            ; Flags
    db 11001111b            ; Flags + Upper Limit
    db 0x00                 ; Base (high)

GDT_datadesc:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

GDT_end:

GDT_descriptor:
    GDT_size:
        dw GDT_end - GDT_nulldesc - 1
        dq GDT_nulldesc

codeseg equ GDT_codedesc - GDT_nulldesc
dataseg equ GDT_datadesc - GDT_nulldesc

[bits 32]

EditGDT:
	mov [GDT_codedesc + 6], byte 10101111b

	mov [GDT_datadesc + 6], byte 10101111b
	ret

[bits 16]

GDT_start:
    GDT_null_descriptor:
        dd 0                    ; 4 times 00000000
        dd 0                    ; 4 times 00000000

    GDT_code_descriptor:
        dw 0FFFFh               ; Limit
        dw 0x0000               ; Base (low, 16 bits) +
        db 0x00                 ; Base (medium, 8 bits) = 24
        db 10011010b            ; Flags (Present, Privilege and Type)
        db 11001111b            ; Other flags (Other + limit)
        db 0x00                 ; Base (high, 8 bits)

    GDT_data_descriptor:
        dw 0FFFFh               ; Limit
        dw 0x0000               ; Base (low)
        db 0x00                 ; Base (medium)
        db 10010010b            ; Flags (Present, Privilege and Type)
        db 11001111b            ; Other flags
        db 0x00                 ; Base (high, 8 bits)

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1 ; size
    dq GDT_start               ; start

CODE_SEGMENT equ GDT_code_descriptor - GDT_start
DATA_SEGMENT equ GDT_data_descriptor - GDT_start

[BITS 32]

Load_GDT_64_bits:
	mov [GDT_code_descriptor + 6], byte 10101111b ; Other flags
	mov [GDT_data_descriptor + 6], byte 10101111b ; Other flags
	ret

[BITS 16]

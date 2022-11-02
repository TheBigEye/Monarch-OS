; Mandatory null descriptor to describe the base of the gdt, and invalid operations
GDT_null_descriptor:
    dd 0
    dd 0

GDT_code_entry:
    dw 0xFFFF               ; Limit
    dw 0x0000               ; Base (low)
    db 0x00                 ; Base (medium)
    db 10011010b            ; Present, privilege, descriptor type, type (code, conforming, readable, accessed)
    db 11001111b            ; Granularity, 32-bit default, 64-bit code segement, AVL, limit
    db 0x00                 ; Base (high)

; Same as GDT_code_entry, aside from the type flags
GDT_data_entry:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

GDT_end:

GDT_descriptor:
    GDT_size:
        dw GDT_end - GDT_null_descriptor - 1
        dq GDT_null_descriptor

CODE_SEGMENT equ GDT_code_entry - GDT_null_descriptor
DATA_SEGMENT equ GDT_data_entry - GDT_null_descriptor

[bits 32]

Load_GDT_64_bits:
	mov [GDT_code_entry + 6], byte 10101111b

	mov [GDT_data_entry + 6], byte 10101111b
	ret

[bits 16]

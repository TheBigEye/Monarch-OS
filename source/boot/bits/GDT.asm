;-----------------------------------------------------------------------------;
;                          Global Descriptor Table                            ;
;-----------------------------------------------------------------------------;

GDT_START: ; Don't remove the labels, they're needed to compute sizes and jumps
    ; The GDT starts with a null 8-byte
    dd 0x0       ; 4 bytes
    dd 0x0       ; 4 bytes

;-------------------------------------;
;       GDT for Code Segment          ;
;-------------------------------------;
GDT_CODE:
    dw 0xffff    ; Segment length, bits 0-15
    dw 0x0       ; Segment base, bits 0-15
    db 0x0       ; Segment base, bits 16-23
    db 10011010b ; Flags (8 bits)
    db 11001111b ; Flags (4 bits) + Segment length, bits 16-19
    db 0x0       ; Segment base, bits 24-31

;-------------------------------------;
;      GDT for Data Segment           ;
;-------------------------------------;
GDT_DATA:
    dw 0xffff    ; Segment length, bits 0-15
    dw 0x0       ; Segment base, bits 0-15
    db 0x0       ; Segment base, bits 16-23
    db 10010010b ; Flags (8 bits)
    db 11001111b ; Flags (4 bits) + Segment length, bits 16-19
    db 0x0       ; Segment base, bits 24-31

GDT_END:

;-------------------------------------;
;          GDT Descriptor             ;
;-------------------------------------;
GDT_DESCRIPTOR:
    dw GDT_END - GDT_START - 1 ; Size (16 bit), always one less than its true size
    dd GDT_START ; Address (32 bit)

;-------------------------------------;
;       Constants for Later Use       ;
;-------------------------------------;
CODE_SEGMENT equ GDT_CODE - GDT_START
DATA_SEGMENT equ GDT_DATA - GDT_START

;-----------------------------------------------------------------------------;
;                        Bootloader Protected Mode Jump                       ;
;-----------------------------------------------------------------------------;

[BITS 16]
JUMP_PROTECTED:
    cli                       ; 1. Disable interrupts
    lgdt [GDT_DESCRIPTOR]     ; 2. Load the GDT descriptor
    mov eax, cr0
    or eax, 0x1               ; 3. Set the 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEGMENT:INIT_PROTECTED ; 4. Far jump using a different segment


[BITS 32]
INIT_PROTECTED:              ; We are now using 32-bit instructions
    mov ax, DATA_SEGMENT     ; 5. Update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000         ; 6. Update the stack right at the top of the free space
    mov esp, ebp

    and esp, 0xFFFFFFF0

    ;---------------------------------;
    ;      Enable the A20 Line        ;
    ;---------------------------------;
    in al, 0x92
    or al, 2
    out 0x92, al

    call LOAD_PROTECTED       ; 7. Call a well-known label with useful code

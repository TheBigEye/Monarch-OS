

[BITS 16]
JUMP_PROTECTED:
    cli ; 1. disable interrupts
    lgdt [GDT_DESCRIPTOR] ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1 ; 3. set 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEGMENT:INIT_PROTECTED ; 4. far jump by using a different segment


[BITS 32]
INIT_PROTECTED: ; we are now using 32-bit instructions
    mov ax, DATA_SEGMENT ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call LOAD_PROTECTED; 7. Call a well-known label with useful code

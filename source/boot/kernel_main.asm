;-----------------------------------------------------------------------------;
;                          Butterfly Kernel Entry Point                       ;
;-----------------------------------------------------------------------------;

[BITS 32]
GLOBAL _start

;-------------------------------------;
;             Entry Point              ;
;-------------------------------------;
_start:
    [EXTERN kernel_main] ; Define calling point. Must have the same name as the 'main' function in kernel.c
    call kernel_main     ; Call the C function. The linker will know where it is placed in memory
    jmp $                ; Stay here indefinitely

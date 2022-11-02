
[extern _idt]
IDT_descriptor:
    dw 4095
    dq _idt

    %macro PUSHALL 0
  	    push rax
  	    push rcx
  	    push rdx
  	    push r8
  	    push r9
  	    push r10
  	    push r11
    %endmacro

    %macro POPALL 0
  	    pop r11
  	    pop r10
  	    pop r9
  	    pop r8
  	    pop rdx
  	    pop rcx
  	    pop rax
    %endmacro

; INTERRUPT SERVICE ROUTINE
[extern isr1_handler]
isr1:
    PUSHALL
    call isr1_handler
    POPALL
    iretq
    GLOBAL isr1

Load_IDT:
    lidt[IDT_descriptor]
    sti
    ret
    GLOBAL Load_IDT

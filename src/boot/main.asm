jmp Protected_mode

%include "src/boot/bits/GDT.asm"
%include "src/boot/memory/memory.asm"

Protected_mode:
	call DetectMemory
    call Enable_a20
    cli
    lgdt [GDT_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEGMENT:Protected_mode_main

Enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

[bits 32]

%include "src/boot/bits/CPUID.asm"
%include "src/boot/memory/paging.asm"

Protected_mode_main:
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    call Is_CPUID_supported
    call Is_long_mode_supported
    call SetUpIdentityPaging
    call Load_GDT_64_bits
    jmp CODE_SEGMENT:Long_mode_main

[bits 64]
[extern _start]

%include "src/boot/bits/IDT.asm"

Long_mode_main:
	mov edi, 0xb8000
	mov rax, 0x1f201f201f201f20
	mov ecx, 500

    rep stosq
	call ActivateSSE
	call _start

    jmp $

ActivateSSE:
	mov rax, cr0
	and ax, 0b11111101
	or ax, 0b00000001
	mov cr0, rax

	mov rax, cr4
	or ax, 0b1100000000
	mov cr4, rax

	ret


times 2048-($-$$) db 0

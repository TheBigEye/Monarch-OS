; Check if CPUID is supported
Is_CPUID_supported:
	pushfd
	pop eax

	mov ecx, eax

	xor eax, 1 << 21

	push eax
	popfd

	pushfd
	pop eax

	push ecx
	popfd

	xor eax,ecx
	jz No_CPUID_supported
	ret

; Check if 64BIT long mode is supported
Is_long_mode_supported:
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz No_long_mode_supported
	ret

No_long_mode_supported:
	hlt ; No Long Mode support

No_CPUID_supported:
	hlt ; No CPUID support

[BITS 32]                       ; Set 32-bit mode for the following code

section .text                   ; Define the code section
global asm_insb                 ; Make asm_insb visible to the linker
global asm_insw                 ; Make asm_insw visible to the linker
global asm_outb                 ; Make asm_outb visible to the linker
global asm_outw                 ; Make asm_outw visible to the linker
global asm_getcpu               ; Make asm_getcpu visible to the linker


;----------------------------------------------------------
; asm_insb: reads a byte from an I/O port.
;   Input (cdecl):
;       [esp+8] = port number to read from
;   Output: AL = byte read from port
;----------------------------------------------------------
asm_insb:
    push ebp                    ; Save caller's base pointer
    mov ebp, esp                ; Set up new stack frame

    xor eax, eax                ; Clear EAX register to zero
    mov edx, [ebp + 8]          ; Get port number from first parameter
    in al, dx                   ; Read byte from I/O port into AL

    pop ebp                     ; Restore caller's base pointer
    ret                         ; Return to caller



;----------------------------------------------------------
; asm_insw: reads a word (16 bits) from an I/O port.
;   Input (cdecl):
;       [esp+8] = port number to read from
;   Output: AX = word read from port
;----------------------------------------------------------
asm_insw:
    push ebp                    ; Save caller's base pointer
    mov ebp, esp                ; Set up new stack frame

    xor eax, eax                ; Clear EAX register to zero
    mov edx, [ebp + 8]          ; Get port number from first parameter
    in ax, dx                   ; Read word from I/O port into AX

    pop ebp                     ; Restore caller's base pointer
    ret                         ; Return to caller



;----------------------------------------------------------
; asm_outb: writes a byte to an I/O port.
;   Input (cdecl):
;       [esp+8] = port number to write to
;       [esp+12] = byte value to write
;   Output: None
;----------------------------------------------------------
asm_outb:
    push ebp                    ; Save caller's base pointer
    mov ebp, esp                ; Set up new stack frame

    mov eax, [ebp + 12]         ; Get value to write from second parameter
    mov edx, [ebp + 8]          ; Get port number from first parameter
    out dx, al                  ; Write byte from AL to I/O port

    pop ebp                     ; Restore caller's base pointer
    ret                         ; Return to caller



;----------------------------------------------------------
; asm_outw: writes a word (16 bits) to an I/O port.
;   Input (cdecl):
;       [esp+8] = port number to write to
;       [esp+12] = word value to write
;   Output: None
;----------------------------------------------------------
asm_outw:
    push ebp                    ; Save caller's base pointer
    mov ebp, esp                ; Set up new stack frame

    mov eax, [ebp + 12]         ; Get value to write from second parameter
    mov edx, [ebp + 8]          ; Get port number from first parameter
    out dx, ax                  ; Write word from AX to I/O port

    pop ebp                     ; Restore caller's base pointer
    ret                         ; Return to caller



;----------------------------------------------------------
; asm_getcpu: executes CPUID instruction and returns processor info.
;   Input (cdecl):
;       [esp+8] = CPUID function number
;       [esp+12] = pointer to store EAX result
;       [esp+16] = pointer to store EBX result
;       [esp+20] = pointer to store ECX result
;       [esp+24] = pointer to store EDX result
;   Output: Stores CPUID results in the provided pointers
;   Note: The results vary depending on the CPUID function number
;         and the specific CPU model
;----------------------------------------------------------
asm_getcpu:
    push ebp                    ; Save caller's base pointer
    mov ebp, esp                ; Set up new stack frame
    add esp, -16                ; Allocate 16 bytes of local stack space
    push ebx                    ; Save EBX (required by cdecl)

    mov eax, [ebp + 8]          ; Load CPUID function number into EAX

    cpuid                       ; Execute CPUID instruction
    mov [ebp - 4], eax          ; Store EAX result temporarily
    mov [ebp - 8], ebx          ; Store EBX result temporarily
    mov [ebp - 12], ecx         ; Store ECX result temporarily
    mov [ebp - 16], edx         ; Store EDX result temporarily

    mov eax, [ebp - 4]          ; Retrieve stored EAX result
    mov edx, [ebp + 12]         ; Get pointer for EAX result
    mov [edx], eax              ; Store EAX result at pointer

    mov ecx, [ebp - 8]          ; Retrieve stored EBX result
    mov eax, [ebp + 16]         ; Get pointer for EBX result
    mov [eax], ecx              ; Store EBX result at pointer

    mov edx, [ebp + 20]         ; Get pointer for ECX result
    mov ecx, [ebp - 12]         ; Retrieve stored ECX result
    mov [edx], ecx              ; Store ECX result at pointer

    mov eax, [ebp + 24]         ; Get pointer for EDX result
    mov edx, [ebp - 16]         ; Retrieve stored EDX result
    mov [eax], edx              ; Store EDX result at pointer

    pop ebx                     ; Restore saved EBX
    mov esp, ebp                ; Clean up stack frame
    pop ebp                     ; Restore caller's base pointer
    ret                         ; Return to caller

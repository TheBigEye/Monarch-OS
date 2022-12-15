MemoryRegionCount:
    db 0
    GLOBAL MemoryRegionCount

; Detect the amount of memory regions in the system
DetectMemory:
    ; Set the segment registers to 0
    mov ax, 0
    mov es, ax

    ; Set the destination index to 0x5000
    mov di, 0x5000

    ; Set the value to compare to 0x534D4150
    mov edx, 0x534D4150

    ; Clear the ebx register
    xor ebx, ebx

    .loop:
        ; Call the BIOS interrupt 0x15 with the E820 memory map function
        mov eax, 0xE820
        mov ecx, 24
        int 0x15

        ; Check if the function returned 0
        cmp ebx, 0
        je .exit

        ; Increment the memory region count and the destination index
        add di, 24
        inc byte [MemoryRegionCount]
        jmp .loop

    .exit:
        ret

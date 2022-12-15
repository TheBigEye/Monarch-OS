; The size of a page table entry
PageTableEntry equ 0x1000

; Set up identity paging
SetUpIdentityPaging:
    ; Set the edi register to the start of the page table
    mov edi, PageTableEntry

    ; Set the cr3 register to the start of the page table
    mov cr3, edi

    ; Set up the first three page table entries
    mov dword [edi], 0x2003
    add edi, 0x1000
    mov dword [edi], 0x3003
    add edi, 0x1000
    mov dword [edi], 0x4003
    add edi, 0x1000

    ; Set up the remaining page table entries
    mov ebx, 0x00000003
    mov ecx, 512

    .SetEntry:
        mov dword [edi], ebx
        add ebx, 0x1000
        add edi, 8
        loop .SetEntry

    ; Enable paging by setting the PSE bit in the cr4 register
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Enable long mode by setting the LM bit in the EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging by setting the PG bit in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

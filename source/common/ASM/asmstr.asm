[BITS 32]

section .text
global asm_strlen
global asm_strcmp
global asm_strrev

global asm_chrlwr
global asm_chrupr
global asm_strlwr
global asm_strupr

global asm_strcat
global asm_strncat
global asm_strcpy
global asm_strncpy


;----------------------------------------------------------
; asm_strlen: calculates the length of a null-terminated string.
;   Input: EDI = pointer to string
;   Output: EAX = length (number of characters before 0)
;----------------------------------------------------------
asm_strlen:
    xor eax, eax                  ; Set EAX (length) to 0

    .next:
        cmp byte [edi + eax], 0   ; Compare current byte with 0
        je .done                  ; If 0, jump to done
        inc eax                   ; Increment EAX
        jmp .next                 ; Repeat

    .done:
        ret                       ; Return length in EAX


;----------------------------------------------------------
; asm_strcmp: compares two null-terminated strings.
;   Input (cdecl):
;       [esp+4] = pointer to the first string (s1)
;       [esp+8] = pointer to the second string (s2)
;   Output:
;       EAX = 0 if the strings are equal, or the difference
;               (unsigned char) between the first pair of
;               differing characters.
;----------------------------------------------------------
asm_strcmp:
    mov edi, [esp+4]              ; Load pointer to the first string (s1)
    mov esi, [esp+8]              ; Load pointer to the second string (s2)

    .next:
        mov al, byte [edi]        ; Load the current character from s1
        mov dl, byte [esi]        ; Load the current character from s2
        cmp al, dl                ; Compare the characters
        jne .done                 ; Jump to .done if they differ
        test al, al               ; Test if the character is null (end of string)
        jz .done                  ; Jump to .done if it's the null terminator
        inc edi                   ; Advance the pointer for s1
        inc esi                   ; Advance the pointer for s2
        jmp .next                 ; Repeat the loop

    .done:
        movzx eax, al             ; Zero-extend the character from s1 to 32 bits
        movzx edx, dl             ; Zero-extend the character from s2 to 32 bits
        sub eax, edx              ; Compute the difference between the characters
        ret



;----------------------------------------------------------
; asm_strrev: reverse a null-terminated string in place.
; Input: EDI = pointer to string
; Output: EAX = original pointer to string
; Uses: asm_strlen to compute the string length.
;----------------------------------------------------------
asm_strrev:
    mov edx, edi                  ; save original pointer

    ; Call asm_strlen (expects pointer in EDI) to get the string length in EAX.
    call asm_strlen
    cmp eax, 0
    je .done                      ; if string is empty, nothing to do

    mov ecx, eax
    dec ecx                       ; ECX = length - 1 (offset of last char)
    mov ebx, edx                  ; EBX will be our start pointer (first char)
    mov esi, edx
    add esi, ecx                  ; ESI now points to last character

    .next:
        cmp ebx, esi
        jge .done                 ; when pointers cross, we are done

        ; swap the bytes at [ebx] and [esi]
        mov al, [ebx]
        mov dl, [esi]
        mov [ebx], dl
        mov [esi], al
        inc ebx
        dec esi

        jmp .next

    .done:
        mov eax, edx              ; return original pointer
        ret



;----------------------------------------------------------
; asm_chrlwr: convert the character in AL to lowercase if it is uppercase.
; Input: AL = character
; Output: AL = lowercase character (or unchanged if not uppercase)
;----------------------------------------------------------
asm_chrlwr:
    cmp al, 'A'
    jl .done

    cmp al, 'Z'
    jg .done

    add al, 32

    .done:
        ret



;----------------------------------------------------------
; asm_chrupr: convert the character in AL to uppercase if it is lowercase.
; Input: AL = character
; Output: AL = uppercase character (or unchanged if not lowercase)
;----------------------------------------------------------
asm_chrupr:
    cmp al, 'a'
    jl .done

    cmp al, 'z'
    jg .done

    sub al, 32

    .done:
        ret



;----------------------------------------------------------
; asm_strlwr: convert a null-terminated string to lowercase in place.
; Input: EDI = pointer to string
; Output: EAX = original pointer to string
;----------------------------------------------------------
asm_strlwr:
    push edi                    ; preserve original pointer
    mov esi, edi               ; use ESI for string traversal

    .next:
        mov al, byte [esi]     ; load current character
        cmp al, 0
        je .done               ; if null terminator, we are done
        call asm_chrlwr        ; convert character in AL to lowercase
        mov byte [esi], al     ; store converted character back
        inc esi
        jmp .next

    .done:
        pop eax                ; restore original pointer to return it
        ret



;----------------------------------------------------------
; asm_strupr: convert a null-terminated string to uppercase in place.
; Input: EDI = pointer to string
; Output: EAX = original pointer to string
;----------------------------------------------------------
asm_strupr:
    push edi                  ; preserve original pointer
    mov esi, edi             ; use ESI for string traversal

    .next:
        mov al, byte [esi]    ; load current character
        cmp al, 0
        je .done              ; if null terminator, we are done
        call asm_chrupr       ; convert character in AL to uppercase
        mov byte [esi], al    ; store converted character back
        inc esi
        jmp .next

    .done:
        pop eax               ; restore original pointer to return it
        ret



;----------------------------------------------------------
; asm_strcat: concatenates source string to destination string.
;   Input: ESI = pointer to source string,
;          EDI = pointer to destination string (null-terminated)
;   Output: EAX = original destination pointer
;----------------------------------------------------------
asm_strcat:
    mov eax, edi                  ; Save original destination pointer

    ; Find end of destination string
    .find_end:
        cmp byte [edi], 0         ; Check for null terminator
        je .concat                ; If found, start concatenating
        inc edi                   ; Move to next character
        jmp .find_end

    .concat:
        mov dl, [esi]             ; Load character from source
        mov [edi], dl             ; Store it in destination
        test dl, dl               ; Check if it's null terminator
        jz .done                  ; If null, we're done
        inc esi                   ; Next source character
        inc edi                   ; Next destination position
        jmp .concat

    .done:
        ret                       ; Return original destination pointer



;----------------------------------------------------------
; asm_strncat: concatenates at most ECX bytes from source to destination.
;   Input: ESI = pointer to source string,
;          EDI = pointer to destination string (null-terminated),
;          ECX = maximum number of bytes to concatenate
;   Output: EAX = original destination pointer
;   Note: Always null-terminates the result
;----------------------------------------------------------
asm_strncat:
    mov eax, edi                  ; Save original destination pointer
    test ecx, ecx                 ; Check if count is zero
    jz .done                      ; If zero, nothing to do

    ; Find end of destination string
    .find_end:
        cmp byte [edi], 0         ; Check for null terminator
        je .concat                ; If found, start concatenating
        inc edi                   ; Move to next character
        jmp .find_end

    .concat:
        mov dl, [esi]             ; Load character from source
        test dl, dl               ; Check if it's null terminator
        jz .terminate             ; If null, we're done
        mov [edi], dl             ; Store character in destination
        inc esi                   ; Next source character
        inc edi                   ; Next destination position
        dec ecx                   ; Decrement count
        jnz .concat               ; Continue if count not zero

    .terminate:
        mov byte [edi], 0         ; Ensure null termination

    .done:
        ret                       ; Return original destination pointer



;----------------------------------------------------------
; asm_strcpy: copies a null-terminated string from source to destination.
;   Input: ESI = pointer to source string,
;          EDI = pointer to destination buffer.
;   Output: EAX = original destination pointer.
;   Note: Destination must be large enough to hold the source string.
;----------------------------------------------------------
asm_strcpy:
    mov eax, edi                  ; Save original destination pointer in EAX

    .copy:
        mov dl, [esi]             ; Load a byte from source into DL
        mov [edi], dl             ; Store the byte into destination
        inc esi                   ; Advance source pointer
        inc edi                   ; Advance destination pointer
        cmp dl, 0                 ; Check if byte was the null terminator
        jne .copy                 ; If not, continue copying
        ret                       ; Return original destination pointer in EAX



;----------------------------------------------------------
; asm_strncpy: copies at most ECX bytes from source to destination.
;   Input: ESI = pointer to source string,
;          EDI = pointer to destination buffer,
;          ECX = number of bytes to copy.
;   Output: EAX = original destination pointer.
;   Behavior: If the source string is shorter than ECX, pad the remainder
;             of destination with 0 bytes. If the source is longer than
;             ECX, no null terminator is added.
;----------------------------------------------------------
asm_strncpy:
    mov eax, edi                  ; Save original destination pointer in EAX
    test ecx, ecx                 ; Check if count (n) is zero
    jz .done                      ; If zero, nothing to do

    .copy:
        mov dl, [esi]             ; Load a byte from source into DL
        cmp dl, 0                 ; Check for end-of-string
        je .pad_zero              ; If reached null, start padding with 0's
        mov [edi], dl             ; Store the byte in destination
        inc esi                   ; Advance source pointer
        inc edi                   ; Advance destination pointer
        dec ecx                   ; Decrement count
        jz .done                  ; If count reaches zero, finish
        jmp .copy                 ; Otherwise, continue copying

    .pad_zero:
    .pad_loop:
        mov byte [edi], 0         ; Pad destination with a zero byte
        inc edi                   ; Advance destination pointer
        dec ecx                   ; Decrement count
        jnz .pad_loop             ; Continue padding until count is 0

    .done:
        ret                       ; Return original destination pointer in EAX

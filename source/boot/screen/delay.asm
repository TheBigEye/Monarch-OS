DELAY:
    PUSH AX           ; Guardar el valor de AX en la pila
    PUSH BX           ; Guardar el valor de BX en la pila
    MOV AH, 0         ; Función para leer el contador del temporizador del BIOS
    INT 0x1A          ; Llamada a la interrupción del BIOS
    MOV BX, DX        ; Guardar el valor actual del contador en BX

    LOOP_DELAY:
        MOV AH, 0         ; Función para leer el contador del temporizador del BIOS
        INT 0x1A          ; Llamada a la interrupción del BIOS
        SUB DX, BX        ; Restar el valor anterior del contador al actual
        CMP DX, 8         ; Comprobar si han pasado al menos 1 tick (aproximadamente 0.4 segundos)
        JB LOOP_DELAY      ; Si no, volver a esperar más ticks

    POP BX            ; Restaurar el valor de BX de la pila
    POP AX            ; Restaurar el valor de AX de la pila
    RET               ; Volver a la función que llamó a delay

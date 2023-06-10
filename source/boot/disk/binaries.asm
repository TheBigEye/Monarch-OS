%macro INCLUDE_BIN 2
  SECTION .rodata                       ; create a new section
  GLOBAL %1                             ; make the symbol global

  ; define the symbol
  %1:
	incbin %2                           ; include the file
	db 0                                ; add a null terminator

	%1_size: dq %1_size - %1            ; define the size of the file
%endmacro

; include the ascii art
INCLUDE_BIN butterfly, "source/binaries/butterfly"

; include more files to be used, an example can be found in shell.cpp

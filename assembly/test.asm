LDI R1, 0x41		; Load Ascii A into R1
LDI R2, 0x000000	; Target address 0x031337
LDI R10, 0x1 		; Constant 1
LDI R3, 0x1			; counter i
LDI R4, 0x1a		; max size
ADD R12, R0, R5     ; next instructions place. R[IP] is already pointing to next when executing current

; Main loop operations

STB R1, R2			; store byte from r1 to [r2]
ADD R1, R10, R1     ; Increment R1
ADD R2, R10, R2     ; Increment the address
ADD R3, R10, R3     ; Increment counter
JLE R3, R4, R5		; jmp to start of loop

; Add a newline
LDI R1, 0x0a
STB R1, R2

; Sub R2 back to start
SUB R2, R4, R2

; Write out content at target location
LDI R1, 0x2
INT 				; R3 is already set correctly

; Print hello world
LDI R10, 1			; Constant
LDI R1, 0x48
LDI R2, 0x000000
STB R1, R2
LDI R1, 0x65
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x6c
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x6c
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x6f
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x20
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x57
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x6f
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x72
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x6c
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x64
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x21
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x0a
ADD R2, R10, R2
STB R1, R2

LDI R1, 0x2
LDI R2, 0x0
LDI R3, 0xd
INT 				; Write(1, "Hello World!\n", 0xd)

LDI R1, 0x3
INT
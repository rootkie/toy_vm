; 0x1000 is buffer for stdout
; 0x031337 is buffer to put user input
; 0x00babe is where the challenge is
; decrypt(0x031337, 0x00babe) = flag


; Loop writing A to Z
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


; Print welcome message
LDI R10, 0x3
LDI R1, 0x695472
LDI R2, 0x1000
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x744279
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x456520
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x6c6d75
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x6f6174
STW R1, R2
ADD R2, R10, R2
LDI R10, 0x1
LDI R1, 0x72
ADD R2, R10, R2
STB R1, R2
LDI R1, 0x0a
ADD R2, R10, R2
STB R1, R2

; Write(1, "TriByte Emulator\n", 0x12)
LDI R1, 0x2
LDI R2, 0x1000
LDI R3, 0x12
INT

; load challenge
; a768e0444a8fb2fd1dbb4ecdc57045f0fc171e53b635bbee2050113211ac63eaa270de455e8985f917bb42d8fc7a56f7f635
LDI R10, 0x3
LDI R1, 0xe0a768
LDI R2, 0x00babe		; Challenge address
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x8f444a
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x1db2fd
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xcdbb4e
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x45c570
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x17f0fc
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xb61e53
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xee35bb
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x112050
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xac3211
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xa263ea
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x4570de
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x855e89
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xbbf917
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xfc42d8
STW R1, R2
ADD R2, R10, R2
LDI R1, 0xf77a56
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x00f635
STW R1, R2


; Print Your decryption code:
LDI R10, 0x3
LDI R1, 0x75596f
LDI R2, 0x1000
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x647220
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x726563
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x747970
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x6e696f
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x6f2063
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x3a6465
STW R1, R2
ADD R2, R10, R2
LDI R1, 0x0A
STW R1, R2

; Write(1, "Your decryption code: ", 0x12)
LDI R1, 0x2
LDI R2, 0x1000
LDI R3, 0x17
INT

; Read(0, 0x031337, 0x20)
LDI R1, 0x1
LDI R2, 0x031337
LDI R3, 0x20
INT


; decryption loop
; R11 = SP, stack pointer
; R10 = word size
; R9 = Dest


LDI R11, 0x200000    ; Stack address
LDI R10, 3		     ; Word size

; Setting up the stack
; [SP+3] = *output buffer 0x1000
; [SP+6] = *user input 0x031337
; [SP+9] = *challenge 0xbabe
LDI R1, 0x1000
ADD R11, R10, R9
STW R1, R9
LDI R1, 0x031337
ADD R9, R10, R9
STW R1, R9
LDI R1, 0xbabe
ADD R9, R10, R9
STW R1, R9

; R8 = counter
; R3 = length
; R4 = start loop
LDI R8, 0
LDI R3, 0x33
ADD R12, R0, R4
; Copy a byte of user input to output buffer
LDI R9, 0x6
ADD R11, R9, R9 		; [SP+6] user input addr
LDW R1, R9				; R1 = [SP+6] = 0X031337 = base
LDI R9, 0x1
LDI R5, 0x20 			; R8 % 2^i = R8 & (2^i - 1)
SUB R5, R9, R5    		; R5 = R5 - R9
AND R8, R5, R5 			; R5 is the offset
ADD R1, R5, R1
LDB R1, R1              ; R1 = [base+offset] 1 byte of user input

LDI R9, 0x9
ADD R11, R9, R9  		; [SP+9] challenge addr 0xbabe
LDW R2, R9
ADD R2, R8, R2
LDB R2, R2   			; R2 = 1 byte of challenge

XOR R1, R2, R1  		; R1 = R1 ^ R2

LDI R9, 0x3
ADD R11, R9, R9         ; 
LDW R9, R9              ; R9 = [SP+3] = 0x1000
ADD R9, R8, R9
STB R1, R9              ; [R9+R8] = R1

LDI R1, 0x1
ADD R8, R1, R8
JLE R8, R3, R4

; Add a newline
LDI R1, 0x0a
LDI R2, 0x0fff
ADD R2, R3, R2
STB R1, R2

; Write(1, 0x1000, 0x12)
LDI R1, 0x2
LDI R2, 0x1000			; Length of flag is defined earlier
INT


; exit
LDI R1, 0x3
INT
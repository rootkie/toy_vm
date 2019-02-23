# Introduction

This project is to develop a byte code interpreter with vm emulation for IOTCTF. This is inspired by cLEMENCy developed by Lightning for DEF CON CTF 2017. Rationale is that for embedded system reverse engineers, it is very common to be dealing with custom architecture designs. The players are challenged on their understanding of how embedded system work instead of relying on existing tools.

# Architecture Design

I will use a custom ISA implementation inspired by RISC ISAs. 

## Registers

This system will feature 10 general purpose registers r1 - r10.


1 R0 will hold the value zero.
1 SP Stack pointer
1 IP Instruction Pointer

Implemented in an array.
reg[0] = R0
reg[1:10] = R1 - R10
r[11] = SP
R[12] = IP
R[13:15] = RESERVED

## Endianess
It will be in weird-endian order which treat middle byte as the MSB. Meaning for word 0x800102 would be 0x010802 for added difficulties. So the middle byte is the most significant byte, followed by left most byte and then right most byte. This rule only apply to addressing. Code and Strings are stored as usual left to right.

## Memory

This emulator will use 3 "8-bit byte" word to address the memory. Maximum memory space would be 2^24 = 16mb or 0xFFFFFF byte.

The emulator will implement 1-level paging for memory as well. The emulator will contain a Page table array of mmaped system pages that contain the page information. The page table address will be pre-determined by the emulator.

It will be 256 pages of 64KB page.

0x000000 - 0x600000 DATA 			6MB
0x600000 - 0xE00000 CODE 			8MB
0xF00000 - 0XFF0000 Reserved 		960KB
0XFF0000 - 0XFFFFFF Paging info     64KB

For example, to access 0x010802 memory, it would be:
```C
uint32 PT[256];
PT[0x08] = mmap(0, (0XFFFF) * sizeof(char), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
char *recv = PT[0x08] + 0x0102;
```

For changing of memory values, this emulator will use a load-store architecture. No operation can be done on memory alone. All changes will require 3 steps

1. Load value into register
2. Change value in register
3. Store value into memory.

## Instructions

The instructions will be variable length.

### LOAD-STORE Operations (5)

For these operations, first 4 bit is opcode, second 4 bit is flag for 1 byte or word. |op code|flag|reg|reg|

Load 3 byte word from memory address to register
LDW RX1, [RX2] -> |0001B|0000B|0001B|0010B|
10 12

Load 1 byte from memory address to LSB of register
LDB RX1, [RX2] -> |0001B|0001B|001B|0010B|
11 12

Store 3 byte word from register to memory address 
STW RX1, [RX2] -> |0010B|0000B|0001B|0010B|
20 12

Store LSB from register to memory address
STB RX1, [RX2] -> |0010B|0001B|0001B|0010B|
21 12

** Special 
|Op code|Reg|Word|
Load immediate word to register.
LDI RX, WORD -> |0011B|0001B|0x400000H|
31 40 00 00

### ADD/SUB Operations (2)

|Op code|RX1|RX2|RX3|

RX3 = RX1 + RX2
ADD RX1, RX2, RX3 -> |0100B|0001B|0010B|0011B|
41 23

RX3 = RX1 - RX2
SUB RX1, RX2, RX3 -> |0101B|0001B|0010B|0011B|
51 23

### BRANCH COND Operations (2)

Jump if RX1 less than or equal to RX2. if (RX1 <= RX2)
JLE RX1, RX2, RX3	|0110B|0001B|0010B|0011B|
61 23

Jump if RX1 Equal to RX2
JE RX1, RX2, RX3	|0111B|0001B|0010B|0011B|
71 23

### Logical bitwise Operations (5)

SHR RX, 4BIT, RX     |1000B|0001B|0001B|0001B|
81 11

SHL RX, 4BIT, RX 	 |1001B|0001B|0001B|0001B|
91 11

XOR RX, RX2, RX3     |1010B|0001B|0010B|0011B|
a1 23

AND RX, RX2, RX3     |1011B|0001B|0010B|0011B|
b1 23

OR  RX, RX2, RX3     |1100B|0001B|0010B|0011B|
c1 23

### Interrupts

For interrupts, R1 will contain interrupt code, R2-R10 will be used to pass arguments defined by interrupts.
INT 			|1110B|
e0

For this purpose, only 3 interrupts would be defined.

1. INPUT from STDIN 	(read)

R1 = 0x1H
R2 = Dest
R3 = Size

R1 will hold return value. Return number of bytes read. This read will strip newline.

2. OUTPUT from Memory 	(write)

R1 = 0x2H
R2 = Src
R3 = Size

R1 will hold return value. Return number of bytes written. This write will not stop at any nullbyte / newline.

3. EXIT 				(exit)

R1 = 0x3H

Reserved		|1111B|

## Sample operations

### Load / Store memory values

```
LDI R2, ADDR
LDW R1, [R2]

... Operations on R1 ...

LDI R2, ADDR
STW R1, [R2]
```
### Branching

JLE and JE should cover most condition checks. Just remember to load RX with appropriate value that you want to jump to.

To make a relative branch:

```
LDI R1, 0X8
ADD R2, R1
JLE R3, R4, R2
```
To make an unconditional branch:

```
JE R1, R1, R2
```
To make a Function call:

```
STW IP, [SP]			; Store current IP at top of stack
LDI R10, 3				; Load R10 with value 3
SUB SP, R10, SP 		; Subtract SP with 3
JE R1, R1, ${TARGET} 	; JMP to Target



LDI R10, 3 				; Load R10 with value 3
ADD SP, R10, SP 		; Add SP with 3
LDW IP, [SP] 			; Load IP with value at top of stack to recover
```

### Loop

```
LDI R1, 0x0h			; R1 is the counter
LDI R2, 0x10h			; max
LDI R4, 0X4h 			; Size of next 2 instructions
ADD IP, R4, R3
LDI R4, 0x1h
...						; start of loop pointed by R3
...
ADD R1, R4, R1 			
JLE R1, R2, R3 			; if(R1 <= R2) goto R3;
...
...
```

# Coding Standards

GNU Coding standard


# Todos

1. Memory Management unit (mmap dedicate space for mmu)
2. CPU instructions (large switch)
3. Interrupt handling (interface with linux kernel)
4. Assembler? May be in python...
5. Challenge program

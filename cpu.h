/*
 * cpu.h - CPU emulator inspired by ARM
 * tribyte CPU
 * 
 * Created by Rootkid (Li Bailin) on 19/02/2019
 * Copyright (c) 2018 Li Bailin. All rights reserved.
 *
 */

#ifndef CPU_H
#define CPU_H

#include "memory.h"

#define IP_START CODE_LOW
#define GET_OP(instr) (instr & 0xF0) >> 4
#define GET_LOAD_STORE_FLAG(instr) (instr & 0x0F)
#define GET_LOAD_STORE_RX1(instr) (instr >> 4) & 0x0F
#define GET_LOAD_STORE_RX2(instr) (instr & 0x0F)

#define LOAD_STORE_BYTE_FLAG 0x1


enum registers
{
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	SP,
	IP
};

enum op_codes
{
	LDR = 0b0001,
	STR = 0b0010,
	LDI = 0b0011,
	ADD = 0b0100,
	SUB = 0b0101,
	JLE = 0b0110,
	JE  = 0b0111,
	SHR = 0b1000,
	SHL = 0b1001,
	XOR = 0b1010,
	AND = 0b1011,
	OR  = 0b1100,
	INT = 0b1110
};

enum interrupts
{
	READ = 0x1,
	WRITE = 0x2,
	EXIT = 0x3
};

uint32_t R[16];
bool to_exit;
bool debug;


// struct registers
// struct opcodes
void main_loop();
void interrupt();

#endif


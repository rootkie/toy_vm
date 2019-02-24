/*
 * cpu.c - CPU emulator inspired by ARM
 * tribyte CPU
 * 
 * Created by Rootkid (Li Bailin) on 19/02/2019
 * Copyright (c) 2018 Li Bailin. All rights reserved.
 *
 */

#include "cpu.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>

// assuming programs is already loaded in memory.


// Params
// IN address, Value of IP usually
// OUT instruction, output through parameter.

bool 
fetch (uint32_t address, char *instruction)
{
	char init_instruction[4];
	uint8_t instruction_size=0; // in bytes

	// Read the max length of instruction first
	for (uint8_t i=0; i < 4; i++)
	{
		if (!read_byte(address + i, &init_instruction[i]))
			return false;
	}

	// Modify the size
	char op = init_instruction[0];
	uint8_t op_code = (uint8_t)GET_OP(op);
	
	switch (op_code)
	{
		case LDI: // LDI
		{
			instruction_size = 4;
			break;
		}
		case INT: // INT
		{
			instruction_size = 1;
			break;
		}
		default:
		{
			instruction_size = 2;

		}
	};

	// truncate the result.
	memcpy(instruction, init_instruction, instruction_size);
	// increment IP
	R[IP] += instruction_size;
	return true;
}

void
interrupt()
{
	uint32_t interrupt_code = R[R1];

	switch(interrupt_code)
	{
		case READ:
		{
			if (debug)
			puts("Read interrupt");
			// cache
			volatile uint32_t size = R[R3];
			volatile uint32_t address = R[R2];
			// printf("size: %d address: %x\n", size, address);
			char buffer[size];
			size = read(0, buffer, size);

			// deposit to memory
			for (uint32_t i = 0; i < size; i++)
			{
				write_byte(address+i, &buffer[i]);
			}
			break;
		}
		case WRITE:
		{
			if (debug)
			puts("Write interrupt");
			volatile uint32_t size = R[R3];
			volatile uint32_t address = R[R2];
			// printf("size: %d address: %x\n", size, address);
			char buffer[size];
			// deposit to memory
			for (uint32_t i = 0; i < size; i++)
			{
				read_byte(address+i, &buffer[i]);
			}

			write(1, buffer, size);
			break;
		}
		case EXIT:
		{
			if (debug)
			puts("Exit interrupt");
			to_exit = true;
			break;
		}
	}
}

void
main_loop()
{
	uint32_t cycles = 0;
	char instruction[4] = {0}; // buffer for instruction
	for (uint i=0; i < 16; i++)
		R[i] = 0;
	R[IP] = IP_START;
	to_exit = false;

	char *opcodes[15] = {
	"LDR\x00",
	"STR\x00",
	"LDI\x00",
	"ADD\x00",
	"SUB\x00",
	"JLE\x00",
	"JE\x00",
	"SHR\x00",
	"SHL\x00",
	"XOR\x00",
	"AND\x00",
	"OR\x00",
	"RESERVED\x00",
	"INT\x00",
	"RESERVED\x00",
	};
	
	while(!to_exit)
	{
		cycles++;
		// fetch instructions
		fetch(R[IP], instruction);

		// get op code
		uint8_t current_op_code = GET_OP(instruction[0]);
		if (debug)
		printf("%d %x %x %s\n",cycles ,R[IP], current_op_code, opcodes[current_op_code-1]);
		// switch cases
		switch(current_op_code)
		{
			case LDR:
			{
				uint8_t ldr_flag = GET_LOAD_STORE_FLAG(instruction[0]);
				uint8_t rx1 = GET_LOAD_STORE_RX1(instruction[1]);
				uint8_t rx2 = GET_LOAD_STORE_RX2(instruction[1]);
				if (ldr_flag & LOAD_STORE_BYTE_FLAG)
				{

					read_byte(R[rx2], &R[rx1]);
				}	
				else 
					read_word(R[rx2], &R[rx1]);
				// printf("Loaded %x \n", R[rx1]);
				break;
			}
			case STR:
			{
				uint8_t str_flag = GET_LOAD_STORE_FLAG(instruction[0]);
				uint8_t rx1 = GET_LOAD_STORE_RX1(instruction[1]);
				uint8_t rx2 = GET_LOAD_STORE_RX2(instruction[1]);
				if (str_flag & LOAD_STORE_BYTE_FLAG)
				{	
					if (debug)
						printf("Writing byte %c to %x\n", R[rx1], R[rx2]);
					write_byte(R[rx2], &R[rx1]);
				}
				else 
					write_word(R[rx2], &R[rx1]);
				break;
			}
			case LDI:
			{
				uint8_t rx = instruction[0] & 0x0F;
				memcpy(&R[rx], &instruction[1], 3);

				// A hack to change endianess
				R[rx] = (R[rx] & 0xFF) << 16 | (R[rx] & 0X00FF00) | (R[rx] >> 16);
				// printf("Register %d = 0x%x\n", rx, R[rx]);
				break;
			}
			case ADD:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t rx2 = instruction[1] >> 4 & 0x0F;
				uint8_t rx3 = instruction[1] & 0x0F;
				R[rx3] = R[rx1] + R[rx2];

				// printf("Registers %d + %d = %d \n", rx1, rx2, rx3);
				// printf("Register %d value %x \n", rx3, R[rx3]);
				break;
			}
			case SUB:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t rx2 = instruction[1] >> 4 & 0x0F;
				uint8_t rx3 = instruction[1] & 0x0F;
				R[rx3] = R[rx1] - R[rx2];
				break;
			}
			case JLE:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t rx2 = instruction[1] >> 4 & 0x0F;
				uint8_t rx3 = instruction[1] & 0x0F;
				if (R[rx1] <= R[rx2])
					R[IP] = R[rx3];
				break;
			}
			case JE:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t rx2 = instruction[1] >> 4 & 0x0F;
				uint8_t rx3 = instruction[1] & 0x0F;
				if (rx1 != rx2 && debug)
					printf("%x %x %x\n", R[rx1], R[rx2], R[rx3]);
				if (R[rx1] == R[rx2])
					R[IP] = R[rx3];
				break;
			}
			case SHR:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t shift = instruction[1] >> 4 & 0x0F;
				uint8_t rx2 = instruction[1] & 0x0F;
				R[rx2] = R[rx1] >> shift; 
			}
			case SHL:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t shift = instruction[1] >> 4 & 0x0F;
				uint8_t rx2 = instruction[1] & 0x0F;
				R[rx2] = R[rx1] << shift; 
			}
			case XOR:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t rx2 = instruction[1] >> 4 & 0x0F;
				uint8_t rx3 = instruction[1] & 0x0F;
				// printf("%x ^ %x = ", R[rx1], R[rx2]);
				R[rx3] = R[rx1] ^ R[rx2];
				// printf("%x \n", R[rx3]);
				break;
			}
			case AND:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t rx2 = instruction[1] >> 4 & 0x0F;
				uint8_t rx3 = instruction[1] & 0x0F;
				R[rx3] = R[rx1] & R[rx2];
				// printf("%x = %x ^ %x", R[rx3], R[rx1], R[rx2]);
				break;
			}
			case OR:
			{
				uint8_t rx1 = instruction[0] & 0x0F;
				uint8_t rx2 = instruction[1] >> 4 & 0x0F;
				uint8_t rx3 = instruction[1] & 0x0F;
				R[rx3] = R[rx1] | R[rx2];
				break;
			}
			case INT:
			{
				interrupt();
			}
		}
		// termination
		if (current_op_code == 0)
			break;
	}
	
}

void 
dump_memory()
{
	char t;
	for (int i=0; i < 0x100; i++)
	{
		read_byte(0x600000+i, &t);
		printf("%x ", t);
	}
}


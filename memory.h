/*
 * memory.h - memory emulator with mmu functionalities.
 * tribyte memory
 * 
 * Created by Rootkid (Li Bailin) on 19/02/2019
 * Copyright (c) 2018 Li Bailin. All rights reserved.
 *
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdbool.h>

// Memory address space structure.
// All pages would be RWX to this simple implementation
// Permissions can be added to in Page Table memory space in future
// All memory will be initially casted to char.
#define DATA_LOW  0X000000
#define DATA_HIGH 0X600000
#define CODE_LOW  0X600000
#define CODE_HIGH 0XE00000
#define RESERVED_MEMORY 0XF00000
#define PAGE_TABLE_VIRTUAL_ADDRESS 0xFF0000
#define MAX_MEMORY 0XFFFFFF

// Weird endian macros
// Middle byte is MSB, followed by left most then right most byte.
// Page access through page table using MSB as index and the rest as offset.
#define GET_LSB(address) address & 0xFF
#define GET_MSB(address) (address & 0xFF00) >> 8
#define GET_PAGE_TABLE_INDEX(address) GET_MSB(address)
#define GET_OFFSET(address) address & 0xFF | ((address & 0xFF0000) >> 8)

// Page table entry structs
// Permissions is currently unused
typedef struct _page_table_entry
{
	char *page_start;
	uint8_t permissions; // 8 bit header + permissions. Unused.
}page_table_entry;


extern page_table_entry page_table[0xFFFF/sizeof(page_table_entry)];

// Exposed methods
void init_memory();
bool read_byte(uint32_t address, void *buf);
bool write_byte(uint32_t address, void *buf);

	// For memory access addresses only.
bool read_word(uint32_t address, void *buf);
bool write_word(uint32_t address, void *buf);

#endif
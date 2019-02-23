/*
 * memory.c - memory emulator with mmu functionalities.
 * tribyte memory
 * 
 * Created by Rootkid (Li Bailin) on 19/02/2019
 * Copyright (c) 2018 Li Bailin. All rights reserved.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include "memory.h"

page_table_entry page_table[0xFFFF/sizeof(page_table_entry)] = {{0}};

bool
create_new_page(uint8_t index)
{
	page_table_entry new_entry;
	new_entry.page_start = mmap(0, (0xffff), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	new_entry.permissions = 0;

	if (page_table[index].page_start == NULL)
	{
		page_table[index] = new_entry;
		return true;
	}
	else
	{
		perror("There is already a page allocated at current index");
		return false;
	}
}

char *
translate_virtual_memory(uint32_t address)
{
	// check if address exceeds max supported memory
	if (address > MAX_MEMORY)
		return NULL;

	uint8_t PTI = GET_PAGE_TABLE_INDEX(address);
	uint16_t offset = GET_OFFSET(address);

	// Page does not exist
	if (page_table[PTI].page_start == NULL)
	{
		if(!create_new_page(PTI))
			return NULL;
	}	

	return page_table[PTI].page_start + offset;
}

bool
read_byte(uint32_t address, void *buf)
{
	char *byte = translate_virtual_memory(address);
	if (byte == NULL)
		return false;

	memcpy(buf, byte, 1);
	return true;
}

bool
read_word(uint32_t address, void *buf)
{
	char *t = (char *)buf;
	return read_byte(address, &t[1]) &&
		   read_byte(address+1, &t[0]) &&
		   read_byte(address+2, &t[2]);

}

bool
write_byte(uint32_t address, void *buf)
{
	char *byte = translate_virtual_memory(address);

	if (memcpy(byte, buf, 1))
		return true;

	return false;
}

bool
write_word(uint32_t address, void *buf)
{
	// a hack way of doing this,
	// but it is fast enough to not bother me for now.
	char *t = (char *)buf;
	return write_byte(address, &t[1]) &&
		   write_byte(address+1, &t[0]) &&
		   write_byte(address+2, &t[2]);
}


void
init_memory()
{
	page_table_entry new_entry;
	new_entry.page_start = (char *)&page_table;
	new_entry.permissions = 0;
	page_table[0xFF] = new_entry;
}

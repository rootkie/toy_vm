#include "cpu.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

uint32_t
load_file(char *filename)
{

	FILE *fd = fopen(filename, "r");
	struct stat st;
	stat(filename, &st);
	uint32_t size = st.st_size;
	
	char buf[size];
	fread(buf, size, 1, fd);

	for (uint32_t i=0; i < size; i++)
	{
		write_byte(IP_START+i, &buf[i]);
	}

	fclose(fd);
	return size;
}

int 
main(int argc, char **argv)
{
	if (argc != 2)
	{
		puts("Usage: ./emulator file");
		exit(1);
	};

	init_memory();

	// load file
	// char *instr = "\x31\x40\x00\x00\x32\x60\x00\x00\x20\x21\x10\x21\x41\x23\x31\x00\x00\x01\x32\x40\x20\x00\x33\x00\x00\x10\xe0\x31\x00\x00\x02\xe0\x31\x00\x00\x03\xe0";
	// for (int i=0; i < 37; i++)
	// 	write_byte(IP_START+i, &instr[i]);

	load_file(argv[1]);
	// start cpu
	debug = false;
	main_loop();
}
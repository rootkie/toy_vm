#!/usr/bin/python2

import sys
from math import floor


def assemble(instr):
	instr = instr.strip("\t")
	raw = ""
	instr_part = instr.split(" ")
	op = instr_part[0]
	# print (instr_part)
	if op == "LDW":
		int_op = 1 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		byte1 = chr(int_op)
		byte2 = chr((reg1 << 4) | reg2)
		raw = byte1 + byte2

	elif op == "LDB":
		int_op = 2 << 4 | 1
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		byte1 = chr(int_op)
		byte2 = chr((reg1 << 4) | reg2)
		raw = byte1 + byte2

	elif op == "STW":
		int_op = 2 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		byte1 = chr(int_op)
		byte2 = chr((reg1 << 4) | reg2)
		raw = byte1 + byte2

	elif op == "STB":
		int_op = 2 << 4 | 1
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		byte1 = chr(int_op)
		byte2 = chr((reg1 << 4) | reg2)
		raw = byte1 + byte2

	elif op == "LDI":
		int_op = 3 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		val = int(instr_part[2], 16)
		byte1 = chr(int_op | reg1)
		byte2 = chr(val >> 16 & 0xFF)
		byte3 = chr(val >> 8 & 0xFF)
		byte4 = chr(val & 0xFF)
		raw = byte1 + byte2 + byte3 + byte4
	
	elif op == "ADD":
		int_op = 4 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		reg3 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(reg2 << 4 | reg3)
		raw = byte1 + byte2

	elif op == "SUB":
		int_op = 5 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		reg3 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(reg2 << 4 | reg3)
		raw = byte1 + byte2

	elif op == "JLE":
		int_op = 6 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		reg3 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(reg2 << 4 | reg3)
		raw = byte1 + byte2

	elif op == "JE":
		int_op = 7 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		reg3 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(reg2 << 4 | reg3)
		raw = byte1 + byte2

	elif op == "SHR":
		int_op = 8 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		shift = int(instr_part[2].strip(","))
		reg2 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(shift << 4 | reg2)
		raw = byte1 + byte2

	elif op == "SHL":
		int_op = 9 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		shift = int(instr_part[2].strip(","))
		reg2 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(shift << 4 | reg2)
		raw = byte1 + byte2

	elif op == "XOR":
		int_op = 10 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		reg3 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(reg2 << 4 | reg3)
		raw = byte1 + byte2

	elif op == "AND":
		int_op = 11 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		reg3 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(reg2 << 4 | reg3)
		raw = byte1 + byte2

	elif op == "OR":
		int_op = 12 << 4
		reg1 = int(instr_part[1].strip("R").strip(","))
		reg2 = int(instr_part[2].strip("R").strip(","))
		reg3 = int(instr_part[3].strip("R").strip(","))
		byte1 = chr(int_op | reg1)
		byte2 = chr(reg2 << 4 | reg3)
		raw = byte1 + byte2

	elif op == "INT":
		raw = "\xe0"
	
	# s = ' '.join(hex(ord(x)) for x in raw)
	# print (s, '\n')

	return raw




def main():
	if len(sys.argv) is not 2:
		print ("Usage: python2 assembler.py filename")
		print ("Tested on python 2.7.15")
		return

	output = []

	filename = sys.argv[1]
	file = open(filename, "r")
	for line in file:
		if line is not "\n":
			raw = assemble(line.rstrip().split(";")[0])
			output.append(raw)

	with open("raw.exe", "w") as f:
		# print ''.join(output)
		f.write(''.join(output))



main()
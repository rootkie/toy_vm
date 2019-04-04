#!/usr/bin/python2 

import sys
from math import floor
from binascii import hexlify
import os

def print_string(s):
	template = """	LDI R1, {}
	ADD R2, R10, R2
	STB R1, R2"""
	for c in s:
		t = template.format(hex(ord(c)))
		print t

	print(hex(len(s)))

def print_tribyte(s):
	l = [0,1,2]
	for i in range(0, len(s)-3, 3):
		l[1] = s[i+0]
		l[2] = s[i+1]
		l[0] = s[i+2]
		print '0x'+hexlify(''.join(l))
	
	# handle last 3
	for c in s[-3:]:
		print hexlify(c)

def gen_challenge(flag):
	key = os.urandom(0x21)
	challenge = ""
	for i,c in enumerate(flag):
		challenge += chr( ord(key[i%0x21]) ^ ord(c) )
	
	print "key:", hexlify(key)
	print "challenge", hexlify(challenge)
	f = ""
	for i,c in enumerate(challenge):
		f += chr( ord(key[i%0x21]) ^ ord(c))
	print f

	print "Break to tribytes"
	print "Key:"
	print_tribyte(key)
	print "challenge"
	print_tribyte(challenge)

def main():
	if sys.argv[1] == "-ps":
		print_string(sys.argv[2])
	if sys.argv[1] == "-pt":
		print_tribyte(sys.argv[2])
	if sys.argv[1] == "-g":
		gen_challenge(sys.argv[2])

main()
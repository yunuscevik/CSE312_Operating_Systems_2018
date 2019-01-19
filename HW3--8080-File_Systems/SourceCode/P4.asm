        ; 8080 assembler code
        .hexfile P4.hex
        .binfile P4.com
        ; try "hex" for downloading in hex format
        .download bin
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 1
PRINT_MEM	equ 2
READ_B		equ 3
READ_MEM	equ 4
PRINT_STR	equ 5
READ_STR	equ 6
fileCreate	equ 12
fileClose	equ 13
fileOpen	equ 14
fileRead	equ 15
fileWrite	equ 16
fileSeek	equ 17
dirRead		equ 18


	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

	;This program adds numbers from 0 to 10. The result is stored at variable
	; sum. The results is also printed on the screen.

number		ds 255 ; will keep the sum
fileName	ds 255 ;
arrayRead	ds 255 ;
myhl		ds 255 ;	

begin:
	LXI SP,stack 	; always initialize the stack pointer


	LXI B,fileName		;
	MVI A,READ_STR		;read string from keyboard
	call GTU_OS		;


	MVI A,fileCreate	;creating file named that read from keyboard
	call GTU_OS		;

	MVI A,fileOpen		; file opened
	call GTU_OS		;


	MOV E, B			; keep the file handler


	MVI A, 50
	LXI H,number
	MVI C,0
loop:
	MOV M, C
	INX H
	INR C
	DCR A
	JNZ loop
	

	LXI B,number

	MVI D, 50		; D = 2	 FOR FILEWRITE !!!		
	MVI A,fileWrite
	call GTU_OS

	;close
	MOV B,E
	MVI A,fileClose
	call GTU_OS

	LXI B,fileName
	MVI A,fileOpen		; file opened
	call GTU_OS

	MOV E,B
	LXI B,arrayRead
	MVI D,100
	MVI A,fileRead
	call GTU_OS

	MOV B,E
	MVI A,fileClose
	call GTU_OS

	LXI B,arrayRead
	MVI A,PRINT_STR
	call GTU_OS


	LXI B,fileName
	MVI A,fileOpen		; file opened
	call GTU_OS

	MVI A, 11
	MVI C,50

loopx:
	MOV M, C
	INX H
	INR C
	DCR A
	JNZ loopx

	MOV E,B
	MVI D,61
	LXI B,number
	MVI A,fileWrite
	call GTU_OS

	MOV B,E
	MVI A,fileClose
	call GTU_OS

	LXI B,fileName
	MVI A,fileOpen		; file opened
	call GTU_OS

	MOV E,B
	LXI B,arrayRead
	MVI D,150
	MVI A,fileRead
	call GTU_OS

	MOV B,E
	MVI A,fileClose
	call GTU_OS

	LXI B,arrayRead
	MVI A,PRINT_STR
	call GTU_OS

	hlt

        ; 8080 assembler code
        .hexfile test.hex
        .binfile test.com
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
GET_RND		equ 7

	; Position for stack pointer
stack   equ 0F000H

	org 0000H
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

	; my variables to use in searching
	ORG 8000H

Hi_Name?:	DW '==> Hi, What is your name?',00AH,00H ; null terminated string
Favorite_Num?:	DW '==> What is your favorite number?',00AH,00H
Favorite_Num?_Print: DW '==> You mean, Your favorite number is value in Register B',00AH,00H
Quit_MSG:	DW '==> Thanks, See you later :)',00AH,00H
Metting_MSG:	DW '==> I am sim8080. I am glad to meet you',00AH,00H
Metting_MSG_RegB:	DW '==> You are a person that I met have to value in Register B',00AH,00H
in_mem:		DW '==> Enter a number to load for the Memory(BC)',00AH,00H
out_mem:	DW '==> The value in location of Memory(BC): ',00AH,00H

begin:
	LXI SP,stack 	; always initialize the stack pointer

	; Hi, What is your name?
	LXI b, Hi_Name? ; test for PRINT_STR
	MVI a, PRINT_STR ; store system call type
	call GTU_OS ; call the OS

	; Enter name
	MVI a, READ_STR
	call GTU_OS ; call the OS

	; I am sim8080. I am glad to meet you
	LXI b, Metting_MSG
	MVI a, PRINT_STR ; store system call type
	call GTU_OS ; call the OS

	; You are a person that I met have to value in Register B
	LXI b, Metting_MSG_RegB
	MVI a, PRINT_STR ; store system call type
	call GTU_OS ; call the OS

	; Generate Random Number
	MVI a, GET_RND
	call GTU_OS

	MVI a, PRINT_B
	call GTU_OS ; call the OS

	; What is your favorite number?
	LXI b, Favorite_Num? ; test for PRINT_STR
	MVI a, PRINT_STR ; store system call type
	call GTU_OS ; call the OS

	; Enter a number
	MVI a, READ_B
	call GTU_OS ; call the OS

	MVI a, PRINT_B
	call GTU_OS ; call the OS

	; You mean, Your favorite number is value in Register B
	LXI b, Favorite_Num?_Print ; test for PRINT_STR
	MVI a, PRINT_STR ; store system call type
	call GTU_OS ; call the OS
	

	; Enter a number to load for the Memory(BC)
	LXI B,in_mem ; test for PRINT_STR
	MVI a, PRINT_STR ; store system call type
	call GTU_OS ; call the OS

	; Enter a number
	MVI a, READ_MEM
	call GTU_OS ; call the OS

	MVI a, PRINT_MEM ; store system call type
	call GTU_OS ; call the OS

	; Quit
	LXI b, Quit_MSG
	MVI a, PRINT_STR ; store system call type
	call GTU_OS ; call the OS

	hlt    ; end program
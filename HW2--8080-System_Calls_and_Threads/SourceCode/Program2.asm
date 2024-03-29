        ; 8080 assembler code
        .hexfile Program2.hex
        .binfile Program2.com
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
TExit		equ 8
TJoin		equ 9
TYield		equ 10
TCreate		equ 11
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


sum	ds 10 ; will keep the sum

begin:
	LXI SP,stack 	; always initialize the stack pointer
	MVI d, 76	; D = 50
	LXI B, F1
	MVI A, TCreate
	call GTU_OS

	LXI B, F3
	MVI A, TCreate
	call GTU_OS	
	
	MVI A, TJoin
	call GTU_OS

	MVI A, TJoin
	call GTU_OS

	hlt		; end program



F1:
	MVI c, 0	; init C with 0
	
loop1:
	MOV B, C	; B = A
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	INR c		; ++C
	DCR D		; --D
	JNZ loop1	; goto loop1 if D!=0
	MVI A, TExit
	call GTU_OS
	RET

F3:
	MVI c, 50	; init C with 0

loop3:
	MOV B, C	; B = A
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	INR c		; ++C
	DCR D		; --D
	JNZ loop3	; goto loop3 if D!=0
	MVI A, TExit
	call GTU_OS
	RET
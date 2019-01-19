        ; 8080 assembler code
        .hexfile Program1.hex
        .binfile Program1.com
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

begin:
	LXI SP,stack 	; always initialize the stack pointer
	LXI B, F1
	MVI A, TCreate
	call GTU_OS
	MVI A, TJoin
	call GTU_OS
	hlt		; end program



F1:
	MVI c, 0	; init C with 0
	MVI d, 50	; D = 50
loop:
	MOV B, C	; B = A
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	INR c		; ++C
	DCR D		; --D
	JNZ loop	; goto loop if D!=0

F2:
	MVI c, 1000	; init C with 10
	MVI a, 1	; A = 1
loop:
	ADD c		; A = A + C
	DCR c		; --C
	JNZ loop	; goto loop if C!=0
	STA SUM		; SUM = A
	LDA SUM		; A = SUM
			; Now we will call the OS to print the value of sum
	MOV B, A	; B = A
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS

F3:
	MVI c, 50	; init C with 0
	MVI d, 50	; D = 50
loop:
	MOV B, C	; B = A
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	INR c		; ++C
	DCR D		; --D
	JNZ loop	; goto loop if D!=0
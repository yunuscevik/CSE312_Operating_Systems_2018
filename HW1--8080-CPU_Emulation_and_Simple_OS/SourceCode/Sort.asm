        ; 8080 assembler code
        .hexfile Sort.hex
        .binfile Sort.com
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

	;This program sorts numbers.


size		equ 50
randomList ds 50 ;will keep the sum
begin:
	LXI SP,stack 	; always initialize the stack pointer
	LXI h, randomList	; store randomList address to HL
	MVI c, size

loopInsert:	
	MVI a, GET_RND 	; 
	call GTU_OS		; call the OS
	MOV m, b 		; M = B
	DCR c 			; --C
	INX h 			; ++H register - Increment address
	JNZ loopInsert

	MVI b, size	; size of the randomList for outer loop
outerLoop:  MVI c, size	; size of the randomList for inner loop
	LXI h, randomList	; store randomList address to HL
	
innerLoop:  MOV a, m	; A = M - Take the first element
	INX h		; ++H register - Increase the randomList address
	MOV d, m		; D = M - take the second element
	CMP d		; A == D - Compare A and D
	JC notSwap	; If A < C, jump notSwap       	

	MOV m, a		; M = A - store A to second elements address
	DCX h		; --H register - Decrease address
	MOV m, d		; M = D - store D to first elements address
	INX h		; ++H register - Increase again so we updated
	
notSwap: DCR c		; --C
	JNZ innerLoop	; goto loop if C!=0
	DCR b		; -- B
	JNZ outerLoop	; goto loop if B!=0
	MVI e, size	; init E with size
	LXI h, randomList	; store randomList address to HL 

print:
	MOV b, m		; B = M - take content from HL to b
	MVI A, PRINT_B  ; store the OS call code to A
	call GTU_OS	; call the OS
	INX h		; Increase the HL
	DCR e		; decrease e, element counter
	JNZ print	; goto loop if E!=0

end:	
	HLT		; End program



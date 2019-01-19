        ; 8080 assembler code
        .hexfile search.hex
        .binfile search.com
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

NOTFOUND:	dw "error",00AH,00H ; null terminated string
size		equ 50
randomList ds 50 ;will keep the randomList
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


    

    MVI A,READ_B; the number wil search
	call GTU_OS
	MOV D,B  ; the number wil search
    
	;;; generate first mid
	LXI H,randomList
	MVI E, 0	; init C with 10
	MVI B, 50
	MOV A,B
	DCR A
	STC 
	CMC
	RAR
	MOV C,A
blbs:   ;begin loop binary search
	INX H
	DCR C
	JNZ blbs
    
   

	; BINARY SEARCH !!!!
    	MOV C,A     ; 
loopBS:
	MOV A,M
	SUB D
	JZ Founded
	JC right
	JNC left
	JMP end
    

right: ; C DE MID VAR !!!!!
	INR C
	MOV E, C
	MOV A, B
	SUB E
	STC 
	CMC
	RAR
	ADD E
	MOV C, A
	LXI H, randomList
loopRight:
	INX H
	DCR A
	JNZ loopRight
	
	MOV A,B ;;if (l > r)
	SUB E
	JC ERR
	JMP loopBS
    
left:
	DCR C
	MOV B, C
	MOV A, B
	SUB E
	STC 
	CMC
	RAR
	ADD E
	MOV C, A
    
	LXI H, randomList
loopLeft:
	INX H
	DCR A
	JNZ loopLeft

	MOV A,B
	SUB E
	JC ERR
    
    JMP loopBS
    
ERR:
	LXI B,NOTFOUND
	MVI A,PRINT_STR
	call GTU_OS
	JMP end
    
Founded:
	MOV B, C
	MVI A, PRINT_B
	call GTU_OS
  
	JMP end

end:	
	HLT		; End program


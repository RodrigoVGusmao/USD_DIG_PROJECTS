;LABEL		;Command					;Comment

bits 64
default rel

SEGMENT .data
		szMsg db "Hello world!", 0Dh, 0Ah, 0

SEGMENT .text
		global	main
		extern	ExitProcess
		extern	printf

main:		push	rbp
		mov	rbp, rsp
		sub	rsp, 20h

		lea	rcx, [szMsg]
		call	printf

		xor	rax, rax
		call	ExitProcess
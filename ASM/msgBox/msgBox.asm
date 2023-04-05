bits 64

SECTION .data:
szCaption db "HW App", 0x0
szText db "Hello World", 0x0

SECTION .text:
global _asm_main
extern ExitProcess
extern MessageBoxA

_asm_main:

	push rbp
	mov rbp, rsp
	sub rsp, 32

	mov rcx, 0
	lea rdx, [szText]
	lea r8, [szCaption]
	mov r9, 0

	call MessageBoxA

	xor rax, rax
	call ExitProcess
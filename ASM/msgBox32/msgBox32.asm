bits 32

SECTION .data:
szMessage	db	"Hello World", 0h
szCaption	db	"HW App", 0h

SECTION .text:
global	_asm_main
extern	ExitProcess
extern	MessageBoxA

_asm_main:
		push	DWORD 0
		push	szCaption
		push	szMessage
		push	DWORD 0
		call	MessageBoxA

		xor	eax, eax
		call	ExitProcess
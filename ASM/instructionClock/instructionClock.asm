bits 64
default rel

segment .data
text:	db	'teste %d', 0

segment .text
global _entry

extern ExitProcess
extern clock
extern printf

_entry:
	sub rsp, 28h
	call clock
	mov rsi, rax
	xor rax, rax
	xor rcx, rcx
	mov rdx, 0

loop:	inc rcx
	div rcx
	cmp rcx, 100000000
	jb loop

	call clock
	sub rax, rsi
	lea rcx, [text]
	mov rdx, rax
	call printf
	xor rcx, rcx
	call ExitProcess
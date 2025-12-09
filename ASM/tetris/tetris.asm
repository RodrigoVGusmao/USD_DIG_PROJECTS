bits 64
default rel

segment .data
%define hInstance 400000h

mainWnd:	dq	3
		dq	mainWndProc
		dq	0
		dq	hInstance
		dq	0, 0, 6, 0
		dq	szClass

szClass:	db	"Main", 0
szMain:		db	"T3", 0

segment .text
global _entry

extern LoadIconA
extern LoadCursorA
extern RegisterClassA
extern AdjustWindowRect
extern CreateWindowExA
extern ShowWindow
extern GetMessageA

_entry:		mov	bl, 200h
		lea	rbp, [mainWnd]
		xor	rdi, rdi
		mov	si, 7f00h

		xor	rcx, rcx
		movzx	rdx, si
		call	[LoadIconA]
		mov	[rbp+20h], rax

		xor	rcx, rcx
		movzx	rdx, si
		call	[LoadCursorA]
		mov	[rbp+28h], rax

		mov	rcx, rbp
		call	[RegisterClassA]
		mov	rbp, rax

		push	rdi
		push	rdi
		mov	rsi, 30000000100h
		push	rsi
		push	rdi
		sub	rsp, 28h
		lea	rcx, [rsp+28h]
		call	[AdjustWindowRect]
		mov	rax, [rsp+28h]
		sub	[rsp+30h], rax
		shl	QWORD [rsp+34h], 20h

		xor	rcx, rcx
		mov	rdx, rbp
		lea	r8, [szMain]
		mov	r9d, 0cf0000h
		mov	rbp, 80000000h
		mov	[rsp+20h], rbp
		mov	[rsp+28h], rbp
		push	rdi
		push	hInstance
		push	rdi
		call	[CreateWindowExA]

		mov	rcx, rax
		mov	edx, 10
		call	[ShowWindow]

		push	rdi
		push	rdi
_entry_loop:	lea	rcx, [rsp+20h]
		xor	rdx, rdx
		xor	r8d, r8d
		xor	r9d, r9d
		call	[GetMessageA]

		test	rax, rax
		jz	_entry_loop_exit

		lea	rcx, [rsp+20h]
		call	[DispatchMessageA]
		jmp	_entry_loop

_entry_loop_exit:
		mov	ecx, [rsp+30h]
		call	ExitProcess

mainWndProc:	
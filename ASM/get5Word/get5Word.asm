bits 64
default rel

segment .data
debug:		db	"%p", 0Dh, 0Ah, 0
file:		dw	__?utf16?__('dictionary.txt'), 0
mode:		db	"r", 0

segment .text
global _dllMain
global getDictionary
global freeDictionary
global get5Word

export _dllMain
export getDictionary
export freeDictionary
export get5Word

extern CreateFileW
extern GetProcessHeap
extern GetFileSizeEx
extern HeapAlloc
extern ReadFile
extern CloseHandle
extern HeapFree
extern ExitProcess
extern printf

_dllMain:	xor	eax, eax
		inc	eax
		ret

;PTR_VAR getDictionary(PTR_SZ filename, PTR_QWORD fileSize)
getDictionary:	sub	rsp, 48h

		mov	[rsp+38h], rdx
		xor	edx, edx
		mov	[rsp+30h], rdx
		inc	edx
		mov	eax, 3
		mov	[rsp+20h], eax
		mov	eax, 0x8000001
		mov	[rsp+28h], eax
		mov	r8d, edx
		ror	edx, 1
		xor	r9d, r9d
		call	CreateFileW
		mov	rcx, rax
		inc	rax
		test	rax, rax
		jz	GD_CFW_ERR
		mov	[rsp+30h], rax

		lea	rdx, [rsp+20h]
		call	GetFileSizeEx
		test	eax, eax
		jz	GD_GFSE_ERR

		inc	QWORD [rsp+20h]
		mov	rdx, [rsp+38h]
		mov	rax, [rsp+20h]
		mov	[rdx], rax

		call	GetProcessHeap
		test	eax, eax
		jz	GD_GFSE_ERR

		mov	rcx, rax
		xor	edx, edx
		mov	r8, [rsp+20h]
		call	HeapAlloc
		test	rax, rax
		jz	GD_GFSE_ERR

		mov	[rsp+28h], rax

		mov	rdx, [rsp+20h]
		dec	rdx
		mov	BYTE [rdx+rax], 0Ah

		mov	rcx, [rsp+30h]
		mov	rdx, rax
		mov	r8, [rsp+20h]
		xor	r9d, r9d
		mov	[rsp+20h], r9
		call	ReadFile
		test	rax, rax
		jnz	success

GD_GFSE_ERR:	mov	[rsp+28h], rax
success:	mov	rcx, [rsp+30h]
		call	CloseHandle
		mov	rax, [rsp+28h]

GD_CFW_ERR:	add	rsp, 48h
		ret

;DWORD freeDictionary(PTR_VAR dictionaryAddr)
freeDictionary:	sub	rsp, 28h

		mov	[rsp+20h], rcx
		call	GetProcessHeap
		test	eax, eax
		jz	FD_GFSE_ERR

		mov	rcx, rax
		xor	edx, edx
		mov	r8, [rsp+20h]
		call	HeapFree

FD_GFSE_ERR:	add	rsp, 28h
		ret

;QWORD get5Word(PTR2_VAR)
get5Word:	sub	rsp, 8h
		mov	[rsp], rsi

		xor	eax, eax
		mov	rsi, [rcx]
G5W_loop:	ror	rax, 8
		lodsb
		cmp	al, 10h
		jae	G5W_loop

		mov	dl, 0FFh
		shl	edx, 10h
		test	rax, rdx
		jnz	G5W_error
		shl	rdx, 8
		test	rax, rdx
		jz	G5W_error
		shr	rax, 18h
		jmp	G5W_success

G5W_error:	xor	eax, eax
G5W_success:	mov	[rcx], rsi
		mov	rsi, [rsp]
		add	rsp, 8
		ret
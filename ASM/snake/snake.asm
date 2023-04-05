bits 64
default rel

segment .bss
DC:		resq	1
playerPos:	resq	1

segment .data
%define hInstance 400000h

wndClass:	dq	3
		dq	wndProc
		dq	0
		dq	hInstance
		dq	0, 0, 0, 0
		dq	szClass

szClass:	db	"main", 0
szWindow:	db	"snake", 0
playerLen:	dw	1
playerDir:	db	4
scorePos:	dq	8000000080h

segment .text
global _entry

extern ExitProcess
extern LoadIconA
extern LoadCursorA
extern GetStockObject
extern RegisterClassA
extern AdjustWindowRect
extern CreateWindowExA
extern ShowWindow
extern GetMessageA
extern DispatchMessageA

extern DefWindowProcA
extern PatBlt
extern Rectangle
extern BeginPaint
extern BitBlt
extern EndPaint
extern InvalidateRect
extern GetTickCount
extern SetTimer
extern CreateCompatibleDC
extern CreateCompatibleBitmap
extern SelectObject
extern DeleteObject
extern PostQuitMessage

_entry:		sub	rsp, 68h

		xor	rcx, rcx
		mov	edx, 7f00h
		call	LoadIconA
		mov	[wndClass+20h], rax

		xor	rcx, rcx
		mov	edx, 7f00h
		call	LoadCursorA
		mov	[wndClass+28h], rax

		mov	rcx, 4
		call	GetStockObject
		mov	[wndClass+30h], rax

		lea	rcx, [wndClass]
		call	RegisterClassA

		mov	QWORD [rsp+28h], 0
		mov	DWORD [rsp+30h], 200h
		mov	DWORD [rsp+34h], 200h
		lea	rcx, [rsp+28h]
		call	AdjustWindowRect
		mov	rax, [rsp+28h]
		sub	[rsp+30h],rax
		mov	eax, [rsp+34h]
		mov	[rsp+38h], eax

		xor	ecx, ecx
		lea	rdx, szClass
		lea	r8, szWindow
		mov	r9d, 0cf0000h
		mov	DWORD [rsp+20h], 80000000h
		mov	DWORD [rsp+28h], 80000000h
		mov	QWORD [rsp+40h], 0
		mov	QWORD [rsp+48h], 0
		mov	QWORD [rsp+50h], hInstance
		mov	QWORD [rsp+58h], 0
		call	CreateWindowExA

		mov	rcx, rax
		mov	rdx, 0Ah
		call	ShowWindow

_E_loop:	lea	rcx, [rsp+20h]
		xor	rdx, rdx
		xor	r8d, r8d
		xor	r9d, r9d
		call	GetMessageA

		test	rax, rax
		jz	_E_exit

		lea	rcx, [rsp+20h]
		call	DispatchMessageA
		jmp	_E_loop

_E_exit:	mov	ecx, [rsp+30h]
		call	ExitProcess

wndProc:	sub	rsp, 28h
		cmp	edx, 0fh
		jz	WM_PAINT
		cmp	edx, 113h
		jz	WM_TIMER
		cmp	edx, 101h
		jz	WM_KEYUP
		cmp	edx, 1
		jz	WM_CREATE
		cmp	edx, 2
		jz	WM_DESTROY
		
		call	DefWindowProcA
		add	rsp, 28h
		ret

WM_PAINT:	sub	rsp, 80h
		mov	[rsp+78h], rbx
		mov	[rsp+70h], rdi
		mov	rbx, rcx

		mov	rcx, [DC]
		xor	edx, edx
		xor	r8d, r8d
		mov	r9d, 200h
		mov	DWORD [rsp+20h], 200h
		mov	DWORD [rsp+28h], 42h
		call	PatBlt

		movzx	edi, WORD [playerLen]
WMP_loop:	mov	rcx, [DC]
		mov	edx, [playerPos-8+rdi*8]
		mov	r8d, [playerPos-4+rdi*8]
		lea	r9d, [edx+20h]
		lea	eax, [r8d+20h]
		mov	DWORD [rsp+20h], eax
		call	Rectangle

		dec	rdi
		test	rdi, rdi
		jnz	WMP_loop

		mov	rcx, [DC]
		mov	edx, [scorePos]
		mov	r8d, [scorePos+4]
		lea	r9d, [edx+20h]
		lea	eax, [r8d+20h]
		mov	DWORD [rsp+20h], eax
		call	Rectangle

		mov	rcx, rbx
		lea	rdx, [rsp+48h]
		call	BeginPaint

		mov	rcx, rax
		xor	edx, edx
		xor	r8d, r8d
		mov	r9d, 200h
		mov	DWORD [rsp+20h], 200h
		mov	rdi, [DC]
		mov	[rsp+28h], rdi
		mov	DWORD [rsp+30h], 0
		mov	DWORD [rsp+38h], 0
		mov	DWORD [rsp+40h], 0CC0020h
		call	BitBlt

		mov	rcx, rbx
		lea	rdx, [rsp+48h]
		call	EndPaint

		mov	rbx, [rsp+90h]
		mov	rdi, [rsp+88h]
		add	rsp, 0A8h
		xor	rax, rax
		ret

WM_TIMER:	xor	rdx, rdx
		xor	r8d, r8d
		call	InvalidateRect

		movzx	eax, WORD [playerLen]
		dec	eax
		mov	rdx, [playerPos]
WMTD_loop:	test	eax, eax
		jz	WMTD_loop_end

		mov	rcx, [playerPos-8+eax*8]
		cmp	rdx, [playerPos+eax*8]
		jz	WM_DESTROY
		mov	[playerPos+eax*8], rcx
		dec	eax
		jmp	WMTD_loop

WMTD_loop_end:	lea	rax, [playerPos]
		lea	rdx, [playerPos+4]
		
		movzx	ecx, BYTE [playerDir]
		lea	rcx, [WMT_negX+ecx*8]
		jmp	rcx

WMT_negX:	sub	DWORD [rax], 20h
		jmp	WMT_default
		TIMES WMT_negX+8-$ nop
WMT_negY:	sub	DWORD [rdx], 20h
		jmp	WMT_default
		TIMES WMT_negY+8-$ nop
WMT_posX:	add	DWORD [rax], 20h
		jmp	WMT_default
		TIMES WMT_posX+8-$ nop
WMT_posY:	add	DWORD [rdx], 20h
		jmp	WMT_default
		TIMES WMT_posY+8-$ nop

WMT_default:	and	DWORD [rax], 1FFh
		and	DWORD [rdx], 1FFh

		mov	rcx, [scorePos]
		cmp	rcx, [rax]
		jz	WMTD_score

WMTDS_exit:	add	rsp, 28h
		xor	rax, rax
		ret

WMTD_score:	call	GetTickCount
		mov	edx, eax
		and	eax, 0Fh
		shl	eax, 5
		and	edx, 0F0h
		shl	edx, 1
		mov	[scorePos], eax
		mov	[scorePos+4], edx

		movzx	eax, BYTE [playerLen]
		mov	QWORD [playerPos+eax*8], 0FFFFh

		inc	WORD [playerLen]
		jmp	WMTDS_exit

WM_KEYUP:	sub	r8d, 25h
		cmp	r8d, 3
		ja	WMKU_exit

		mov	r9d, r8d
		xor	r9d, [playerDir]
		test	r9d, 1
		jz	WMKU_exit

		mov	[playerDir], r8b

WMKU_exit:	add	rsp, 28h
		xor	rax, rax
		ret

WM_CREATE:	mov	r8d, 250
		xor	r9, r9
		call	SetTimer

		xor	rcx, rcx
		call	CreateCompatibleDC
		mov	[DC], rax

		mov	rcx, rax
		mov	edx, 200h
		mov	r8d, 200h
		call	CreateCompatibleBitmap

		mov	rcx, [DC]
		mov	rdx, rax
		call	SelectObject

		mov	rcx, rax
		call	DeleteObject

		add	rsp, 28h
		xor	rax, rax
		ret

WM_DESTROY:	xor	ecx, ecx
		call	PostQuitMessage
		
		add	rsp, 28h
		xor	rax, rax
		ret
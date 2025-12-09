bits 64
default rel

segment .bss
DC:		resq	1
playerPos:	resq	1

segment .data
%define hInstance 400000h

wndClass:	dq	3
		dq	0
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

_entry:		dq	0xDEADBEEFDEADBEEF
		xsave [rsp]
		xsave [wndClass]
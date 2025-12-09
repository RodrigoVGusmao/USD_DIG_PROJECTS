bits 64
default rel

segment .data
wndClass:	dq	3
		dq	wndProc
		dq	0
hInstance:	dq	0
		dq	0, 0, 6, 0
		dq	szClass

szClass:	dw	__?utf16?__('entryWnd'), 0
szEntryTitle	dw	__?utf16?__('emulator'), 0

segment .text
global _entry

extern GetModuleHandle
extern ExitProcess
extern LoadIconW
extern LoadCursorW
extern RegisterClassW
extern CreateWindowW
extern ShowWindow
extern GetMessageW
extern TranslateMessage
extern DispatchMessageW

_entry:		sub rsp, 28h
		
		call GetModuleHandle
		mov [hInstance], rax

		xor rcx, rcx
		mov edx, 7F00h
		call LoadIconW
		mov [wndClass+20h], rax

		xor rcx, rcx
		mov edx, 7F00h
		call LoadCursorW
		mov [wndClass+28h], rax

		lea rcx, [wndClass]
		call RegisterClassW

		mov rcx, rax
		
		xor 
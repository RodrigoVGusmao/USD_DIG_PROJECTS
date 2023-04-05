bits 32
default rel

segment .data
dllName:	db	"C:\Users\digao\OneDrive\Documentos\programming\c_c++\keyboardStateHook\bin\Debug\keyboardStateHook.dll", 0
procName:	db	"LoadLibraryA", 0

segment .text
global _entry

extern LoadLibraryA
extern GetProcAddress
extern ExitProcess

_entry:		push	dllName
		call	[LoadLibraryA]

		push	procName
		push	eax
		call	[GetProcAddress]

		push	0
		push	0
		call	eax

		push	0
		call	[ExitProcess]
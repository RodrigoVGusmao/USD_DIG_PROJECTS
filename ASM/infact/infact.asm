bits 64
default rel

segment .data
fileName:	db	"snake.exe", 0
err_CreateFile:	db	"could not open file: ", 0
err_ReadFile:	db	"could not read file: ", 0

segment .text
global _entry

extern CreateFileA
extern GetFileSizeEx
extern GetProcessHeap
extern HeapAlloc
extern HeapReAlloc
extern ReadFile
extern ExitProcess

_entry:		sub rsp, 38h

		lea rcx, [fileName]
		mov edx, 0C0000000h
		xor r8d, r8d
		xor r9, r9
		mov DWORD [rsp+20h], 3
		mov DWORD [rsp+28h], 80h
		mov QWORD [rsp+30h], r8
		call CreateFileA

		mov rcx, rax
		mov rbx, rax
		cmp rax, -1
		je exit

		lea rdx, [rsp+30h]
		call GetFileSizeEx

		call GetProcessHeap

		mov rcx, rax
		mov edx, 1
		mov r8, [rsp+30h]
		inc r8
		call HeapAlloc

		mov ecx, -2
		test rax, rax
		je exit

		mov rcx, rbx
		mov rdx, rax
		mov rdi, rax
		mov r8d, [rsp+30h]
		lea r9d, [rsp+28h]
		mov QWORD [rsp+20h], 0
		call ReadFile

		xor rcx, rcx
		mov ecx, [rdi+3Ch]
		xor r12, r12
		mov r12d, DWORD [rdi+rcx+3Ch]

		call GetProcessHeap

		mov rcx, rax
		mov edx, 1
		mov r8, rdi
		mov r9, [rsp+30h]
		lea r9, [r9+r12+1]
		call HeapReAlloc

		xor rcx, rcx
		not rcx
		mov rax, 61746164692Eh
loop_idata:	inc rcx
		cmp [rdi+rcx], rax
		jne loop_idata

		mov edx, [rdi+rcx+10h]

		mov r8d, [rdi+rcx+14h]
		mov r9, [rsp+30h]
		add r9, rdi

		mov rax, r9
		sub rax, r12
		add r9, 8
loop_rewrite:	sub r9, 8
		mov r11, [r9]
		mov [r9+r12], r11
		cmp r9, rax
		jg loop_rewrite

		mov r9, `teste\0\0\0`
		xor rax, rax
		lea eax, [r8d+edx]
		mov [rax+rdi], r9

		add edx, r12d
		mov [rdi+rcx+10h], edx

		xor rcx, rcx
exit:		call ExitProcess
bits 64
default rel

%define test_count 4
%define matrix_size 1000
%define cache_elements 8

;stack labels
%define ptr_mat1 rsp + 70h
%define ptr_mat2 rsp + 68h
%define ptr_temp rsp + 60h
%define ptr_resu rsp + 58h
%define ptr_vec1 rsp + 50h
%define ptr_vec2 rsp + 48h
%define ptr_resv rsp + 40h
%define time_start rsp + 38h
%define time_end rsp + 30h
%define time_frequency rsp + 28h

segment .data
text:		db	"test%d: %f", 0Dh, 0Ah, 0
debug:		db	"step %d/%d/%d", 0Dh, 0Ah, 0

segment .text
global _entry

extern ExitProcess
extern calloc
extern rand
extern QueryPerformanceCounter
extern QueryPerformanceFrequency
extern printf

_entry:		sub rsp, 78h

		lea rcx, [time_frequency]
		call [QueryPerformanceFrequency]

		lea rbx, [ptr_resu-8]
		xor r14d, r14d

		xor r12d, r12d
		mov r12b, 4
alloc_ptr:	mov r13d, matrix_size
		mov ecx, 8
		mov edx, r13d
		call [calloc]
		mov [rbx+r12*8], rax

alloc_data:	mov ecx, 8
		mov edx, matrix_size
		call [calloc]
		mov rcx, [rbx+r12*8]
		mov [rcx+r13*8-8], rax
		mov r15, rax

		cmp r12d, 3
		jae init_zero

		mov r14d, matrix_size
set_data:	call [rand]
		cvtsi2sd xmm0, rax
		movsd [r15+r14*8-8], xmm0
		dec r14d
		jnz set_data
		jmp over_init_zero

init_zero:	mov rdi, rax
		mov ecx, matrix_size
		xor eax, eax
		rep stosq

over_init_zero:	dec r13d
		jnz alloc_data
		
		dec r12d
		jnz alloc_ptr

;start test 1
		inc r14d
		mov ebx, test_count
loop_repeat_t1:	lea rcx, [time_start]
		call QueryPerformanceCounter

		mov r9d, matrix_size
loop_test1_i:	mov rax, [ptr_resu]
		mov rax, [rax + r9*8-8]
		mov rcx, [ptr_mat1]
		mov rcx, [rcx + r9*8-8]
		mov r10d, matrix_size
loop_test1_j:	mov r11d, matrix_size
		movsd xmm2, [rax + r10*8-8]
loop_test1_k:	movsd xmm0, [rcx + r11*8-8]

		mov rdx, [ptr_mat2]
		mov rdx, [rdx + r11*8-8]

		mulsd xmm0, [rdx + r10*8-8]
		addsd xmm2, xmm0

		dec r11d
		jnz loop_test1_k

		movsd [rax + r10*8-8], xmm2

		dec r10d
		jnz loop_test1_j

		dec r9d
		jnz loop_test1_i

		lea rcx, [time_end]
		call [QueryPerformanceCounter]
		mov rax, [time_end]
		sub rax, [time_start]
		add r13, rax

		mov rdx, [ptr_resu]
		call set_matrix_0

		dec ebx
		jnz loop_repeat_t1

		shr r13, 2

		mov [rsp-8], r13
		cvtsi2sd xmm1, [time_frequency]
		cvtsi2sd xmm0, [rsp-8]
		divsd xmm0, xmm1
		movsd [rsp-8], xmm0
		mov r8, [rsp-8]

		lea rcx, [text]
		mov rdx, r14
		call [printf]

;start test 2
		inc r14
		mov ebx, test_count
		xor r13d, r13d
loop_repeat_t2:	lea rcx, [time_start]
		call [QueryPerformanceCounter]

		mov r9d, matrix_size
loop_test2_ti:	mov rax, [ptr_temp]
		mov rax, [rax + r9*8-8]
		mov r10d, matrix_size
loop_test2_tj:	mov rcx, [ptr_mat2]
		mov rcx, [rcx + r10*8-8]
		mov rdx, [rcx + r9*8-8]
		mov [rax + r10*8-8], rdx

		dec r10d
		jnz loop_test2_tj

		dec r9d
		jnz loop_test2_ti

		mov r9d, matrix_size
loop_test2_i:	mov rax, [ptr_resu]
		mov rax, [rax + r9*8-8]
		mov rcx, [ptr_mat1]
		mov rcx, [rcx + r9*8-8]
		mov r10d, matrix_size
loop_test2_j:	mov r11d, matrix_size
		movsd xmm2, [rax + r10*8-8]
loop_test2_k:	movsd xmm0, [rcx + r11*8-8]

		mov rdx, [ptr_temp]
		mov rdx, [rdx + r10*8-8]

		mulsd xmm0, [rdx + r11*8-8]
		addsd xmm2, xmm0

		dec r11d
		jnz loop_test2_k

		movsd [rax + r10*8-8], xmm2

		dec r10d
		jnz loop_test2_j

		dec r9d
		jnz loop_test2_i

		lea rcx, [time_end]
		call [QueryPerformanceCounter]
		mov rax, [time_end]
		sub rax, [time_start]
		add r13, rax

		mov rdx, [ptr_resu]
		call set_matrix_0

		dec rbx
		jnz loop_repeat_t2

		shr r13, 2

		mov [rsp-8], r13
		cvtsi2sd xmm1, [time_frequency]
		cvtsi2sd xmm2, [rsp-8]
		divsd xmm2, xmm1
		movsd [rsp-8], xmm2
		mov r8, [rsp-8]

		lea ecx, [text]
		mov edx, r14d
		call [printf]

;start test 3
		inc r14d
		mov ebx, test_count
		xor r13d, r13d
loop_repeat_t3:	lea rcx, [time_start]
		call QueryPerformanceCounter

		mov r9d, matrix_size
loop_test3_i:	mov rax, [ptr_resu]
		mov rax, [rax + r9*8-8]
		mov rcx, [ptr_mat1]
		mov rcx, [rcx + r9*8-8]
		mov r10d, matrix_size
loop_test3_k:	mov r11d, matrix_size
		mov rdx, [ptr_mat2]
		mov rdx, [rdx + r10*8-8]
		movsd xmm1, [rcx + r10*8-8]
		movsd xmm2, [rax + r11*8-8]
loop_test3_j:	movsd xmm0, [rdx + r11*8-8]

		mulsd xmm0, xmm1
		addsd xmm2, xmm0

		dec r11d
		jnz loop_test3_j

		movsd [rax + r11*8-8], xmm2

		dec r10d
		jnz loop_test3_k

		dec r9d
		jnz loop_test3_i

		lea rcx, [time_end]
		call [QueryPerformanceCounter]
		mov rax, [time_end]
		sub rax, [time_start]
		add r13, rax

		mov rdx, [ptr_resu]
		call set_matrix_0

		dec ebx
		jnz loop_repeat_t3

		shr r13, 2

		mov [rsp-8], r13
		cvtsi2sd xmm1, [time_frequency]
		cvtsi2sd xmm0, [rsp-8]
		divsd xmm0, xmm1
		movsd [rsp-8], xmm0
		mov r8, [rsp-8]

		lea rcx, [text]
		mov rdx, r14
		call [printf]

;start test 4
		inc r14d
		mov ebx, test_count
		xor r13d, r13d
loop_repeat_t4:	lea rcx, [time_start]
		call QueryPerformanceCounter

		xor r9d, r9d
loop_test4_i:	mov rax, [ptr_resu]
		lea rax, [rax + r9*8]
		mov rcx, [ptr_mat1]
		lea rcx, [rcx + r9*8]
		xor r10d, r10d
loop_test4_k:	xor r11d, r11d
		mov rdx, [ptr_mat2]
		lea rdx, [rdx + r10*8]
loop_test4_j:	mov r12d, cache_elements
loop_test4_i2:	mov rsi, [rax + r12*8-8]
		mov rdi, [rcx + r12*8-8]
		mov r15d, cache_elements
loop_test4_k2:	mov rbp, [rdx + r15*8-8]
		movsd xmm1, [rdi + r15*8-8]
		mov r8d, cache_elements
loop_test4_j2:	movsd xmm0, [rbp + r8*8-8]
		mulsd xmm0, xmm1
		addsd xmm0, [rsi + r8*8-8]
		movsd [rsi + r8*8-8], xmm0

		dec r8d
		jnz loop_test4_j2

		dec r15d
		jnz loop_test4_k2

		dec r12d
		jnz loop_test4_i2

		add r11w, 8
		cmp r11w, 1000
		ja loop_test4_j

		add r10w, 8
		cmp r10w, 1000
		ja loop_test4_k

		add r9w, 8
		cmp r9w, 1000
		jbe loop_test4_i

		lea rcx, [time_end]
		call [QueryPerformanceCounter]
		mov rax, [time_end]
		sub rax, [time_start]
		add r13, rax

		mov rdx, [ptr_resu]
		call set_matrix_0

		dec ebx
		jnz loop_repeat_t4

		shr r13, 2

		mov [rsp-8], r13
		cvtsi2sd xmm1, [time_frequency]
		cvtsi2sd xmm0, [rsp-8]
		divsd xmm0, xmm1
		movsd [rsp-8], xmm0
		mov r8, [rsp-8]

		lea rcx, [text]
		mov rdx, r14
		call [printf]

		call [ExitProcess]

set_matrix_0:	mov eax, matrix_size
loop_addr:	mov r9, [rdx + rax*8-8]
		lea rdi, [r9]
		mov ecx, matrix_size
		xor rax, rax
		rep stosq
		ret
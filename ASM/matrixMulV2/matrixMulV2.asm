bits 64
default rel

%define matrix_size 1000
%define test_count 1
%define expoent_test_count 0

;main stack thing
%define st_time_freq rsp + 70h
%define st_time_start rsp + 68h
%define st_time_end rsp + 60h
%define st_
%define st_
%define st_
%define st_ptr_temp rsp + 40h
%define st_ptr_resu rsp + 38h
%define st_ptr_mat2 rsp + 30h
%define st_ptr_mat1 rsp + 28h

segment .data
str_text:		db	"test %d: %f", 0Dh, 0Ah, 0

segment .text
global _entry
extern QueryPerformanceFrequency
extern QueryPerformanceCounter
extern malloc
extern rand
extern printf
extern ExitProcess

_entry:		sub	rsp, 78h

		lea	rcx, [st_time_freq]
		call	[QueryPerformanceFrequency]
		shl	QWORD [st_time_freq], expoent_test_count

;init data
		mov	r15d, 1000
		mov	r14d, 8000
		mov	rbp, 4
lp_res_ptr_v:	mov	ecx, r14d
		call	[malloc]
		mov	[st_ptr_mat1 + rbp*8 - 8], rax
		lea	rbx, [rax]

		mov	esi, r15d
lp_res_double_v:mov	ecx, r14d
		call	[malloc]
		mov	[rbx + rsi*8 - 8], rax
		lea	r12, [rax]

		cmp	rbp, 3
		jb	init_data_rand

		lea	edi, [r12]
		mov	ecx, r15d
		xor	eax, eax
		rep	stosq

		dec	esi
		jnz	lp_res_double_v

		dec	ebp
		jnz	lp_res_ptr_v

init_data_rand:	mov	r13d, r15d
lp_data_init:	call	[rand]
		cvtsi2sd xmm0, eax
		mov	[r12], rax

		dec	r13d
		jnz	lp_data_init

		dec	esi
		jnz	lp_res_double_v

		dec	ebp
		jnz	lp_res_ptr_v

;start test 1
		inc	esi
		xor	edi, edi
		mov	rbp, test_count
lp_test1_repeat:lea	rcx, [st_time_start]
		call	[QueryPerformanceCounter]

		mov	r8d, r15d
lp_test1_i:	mov	rax, [st_ptr_resu]
		mov	rax, [rax + r8*8 - 8]

		mov	rcx, [st_ptr_mat1]
		mov	rcx, [rcx + r8*8 - 8]

		mov	r9d, r15d
lp_test1_j:	movsd	xmm0, [rax + r9*8 - 8]

		mov	r10d, r15d
lp_test1_k:	mov	rdx, [st_ptr_mat2]
		mov	rdx, [rdx + r10*8 - 8]

		movsd	xmm1, [rcx + r10*8 - 8]
		mulsd	xmm1, [rdx + r9*8 - 8]
		addsd	xmm0, xmm1

		dec	r10d
		jnz	lp_test1_k

		movsd	[r12 + r9*8 - 8], xmm0

		dec	r9d
		jnz	lp_test1_j

		dec	r8d
		jnz	lp_test1_i

		lea	rcx, [st_time_end]
		call	[QueryPerformanceCounter]
		add	rdi, [st_time_end]
		sub	rdi, [st_time_start]

		dec	rbp
		jnz	lp_test1_repeat

		lea	rcx, [str_text]
		mov	edx, esi
		cvtsi2sd xmm0, rdi
		cvtsi2sd xmm1, [st_time_freq]
		divsd	xmm0, xmm1
		movsd	[rsp-8], xmm0
		mov	r8, [rsp-8]
		call	[printf]

;start test 2
		inc	esi
		xor	edi, edi
		mov	rbp, test_count
lp_test2_repeat:lea	rcx, [st_time_start]
		call	[QueryPerformanceCounter]

		mov	r8d, r15d
lp_transpose_j:	mov	rdx, [st_ptr_mat2]
		mov	rdx, [rdx + r8*8 - 8]

		mov	r9d, r15d
lp_transpose_k:	mov	rbx, [st_ptr_temp]
		mov	rbx, [rbx + r9*8 - 8]

		movsd	xmm0, [rdx + r9*8 - 8]
		movsd	[rbx + r8*8 - 8], xmm0

		dec	r9d
		jnz	lp_transpose_k

		dec	r8d
		jnz	lp_transpose_j

		mov	r8d, r15d
lp_test2_i:	mov	rax, [st_ptr_resu]
		mov	rax, [rax + r8*8 - 8]

		mov	rcx, [st_ptr_mat1]
		mov	rcx, [rcx + r8*8 - 8]

		mov	r9d, r15d
lp_test2_j:	mov	rbx, [st_ptr_temp]
		mov	rbx, [rbx + r9*8 - 8]

		movsd	xmm0, [rax + r9*8 - 8]

		mov	r10d, r15d
lp_test2_k:	movsd	xmm1, [rcx + r10*8 - 8]

		mulsd	xmm1, [rbx + r10*8 - 8]
		addsd	xmm0, xmm1

		dec	r10d
		jnz	lp_test2_k

		movsd	[rax + r9*8 - 8], xmm0

		dec	r9d
		jnz	lp_test2_j

		dec	r8d
		jnz	lp_test2_i

		lea	rcx, [st_time_end]
		call	[QueryPerformanceCounter]
		add	rdi, [st_time_end]
		sub	rdi, [st_time_start]

		dec	rbp
		jnz	lp_test2_repeat

		lea	rcx, [str_text]
		mov	edx, esi
		cvtsi2sd xmm0, rdi
		cvtsi2sd xmm1, [st_time_freq]
		divsd	xmm0, xmm1
		movsd	[rsp-8], xmm0
		mov	r8, [rsp-8]
		call	[printf]

;start test 3
		inc	esi
		xor	edi, edi
		mov	rbp, test_count
lp_test3_repeat:lea	rcx, [st_time_start]
		call	[QueryPerformanceCounter]

		mov	r8d, r15d
lp_test3_i:	mov	rax, [st_ptr_resu]
		mov	rax, [rax + r8*8 - 8]

		mov	rcx, [st_ptr_mat1]
		mov	rcx, [rcx + r8*8 - 8]

		mov	r9d, r15d
lp_test3_k:	mov	rdx, [st_ptr_mat2]
		mov	rdx, [rdx + r9*8 - 8]

		movsd	xmm1, [rcx + r9*8 - 8]

		mov	r10d, r15d
lp_test3_j:	movsd	xmm2, [rdx + r10*8 - 8]

		mulsd	xmm2, xmm1
		addsd	xmm2, [rax + r10*8 - 8]

		movsd	[rax + r10*8 - 8], xmm2

		dec	r10d
		jnz	lp_test3_j

		dec	r9d
		jnz	lp_test3_k

		dec	r8d
		jnz	lp_test3_i

		lea	rcx, [st_time_end]
		call	[QueryPerformanceCounter]
		add	rdi, [st_time_end]
		sub	rdi, [st_time_start]

		dec	rbp
		jnz	lp_test3_repeat

		lea	rcx, [str_text]
		mov	edx, esi
		cvtsi2sd xmm0, rdi
		cvtsi2sd xmm1, [st_time_freq]
		divsd	xmm0, xmm1
		movsd	[rsp-8], xmm0
		mov	r8, [rsp-8]
		call	[printf]

;alinhamento

;start test 4
		inc	esi
		xor	edi, edi
		mov	rbp, test_count
lp_test4_repeat:lea	rcx, [st_time_start]
		call	[QueryPerformanceCounter]

		mov	r8d, r15d
lp_test4_i:	mov	rax, [st_ptr_resu]
		lea	rax, [rax + r8*8 - 64]

		mov	rcx, [st_ptr_mat1]
		lea	rcx, [rcx + r8*8 - 64]

		mov	r9d, r15d
lp_test4_k:	mov	rdx, [st_ptr_mat2]
		lea	rdx, [rdx + r9*8 - 64]

		mov	r10d, r15d
lp_test4_j:	mov	r11d, 8
lp_test4_i2:	mov	rbx, [rax + r11*8 - 8]
		lea	rbx, [rbx + r10*8 - 8]

		mov	rdi, [rcx + r11*8 - 8]
		lea	rdi, [rdi + r9*8 - 8]

		mov	r12d, 8
lp_test4_k2:	mov	r14, [rdx + r12*8 - 8]
		lea	r14, [r14 + r10*8 - 8]

		movsd	xmm1, [rdi + r12*8 - 8]

		mov	r13d, 8
lp_test4_j2:	movsd	xmm2, [r14 + r13*8 - 8]
		mulsd	xmm2, xmm1
		addsd	xmm2, [rbx + r13*8 - 8]
		movsd	[rbx + r13*8 - 8], xmm2

		dec	r13d
		jnz	lp_test4_j2

		dec	r12d
		jnz	lp_test4_k2

		dec	r11d
		jnz	lp_test4_i2

		sub	r10d, 8
		ja	lp_test4_j

		sub	r9d, 8
		ja	lp_test4_k

		sub	r8d, 8
		ja	lp_test4_i

		lea	rcx, [st_time_end]
		call	[QueryPerformanceCounter]
		add	rdi, [st_time_end]
		sub	rdi, [st_time_start]

		dec	rbp
		jnz	lp_test4_repeat

		lea	rcx, [str_text]
		mov	edx, esi
		cvtsi2sd xmm0, rdi
		cvtsi2sd xmm1, [st_time_freq]
		divsd	xmm0, xmm1
		movsd	[rsp-8], xmm0
		mov	r8, [rsp-8]
		call	[printf]

		call	[ExitProcess]

%ifdef dummy prototype
		inc	esi
		xor	edi, edi
		mov	rbp, 4
lp:		lea	rcx, [st_time_start]
		call	[QueryPerformanceCounter]

		lea	rcx, [st_time_end]
		call	[QueryPerformanceCounter]
		add	rdi, [st_time_end]
		sub	rdi, [st_time_start]

		dec	rbp
		jnz	lp

		lea	rcx, [str_text]
		mov	edx, esi
		cvtsi2sd xmm0, rdi
		cvtsi2sd xmm1, [st_time_freq]
		divsd	xmm0, xmm1
		movsd	[rsp-8], xmm0
		mov	r8, [rsp-8]
		call	[printf]
%endif
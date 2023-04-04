bits 16
org 100h
%define alloc (8)
%define logAlloc (3)

		mov	dx, sdMessage
		mov	ah, 9
		int	21h

		xor	bp, bp
		xor	bx, bx
		xor	cx, cx
		xor	si, si
		mov	ah, 1
loop:		int	21h
		cmp	al, 0Dh
		je	out
		
		sub	al, 30h
		jl	operations
		
		cmp	al, 09h
		ja	loop
		imul	cx, cx, 0Ah
		add	cl, al
		jmp	loop
		
operations:	test	bp, bp
		jnz	loop
		
		not	al
		mov	bl, al

		cmp	al, 0
		jz	valid
		cmp	bl, 2
		jz	valid
		cmp	al, 4
		jz	valid
		cmp	bl, 5
		jz	valid
		jmp	loop

valid:		shl	bx, logAlloc
		add	bx, funcOffset
		mov	bp, bx
		mov	bx, cx
		xor	cx, cx
		jmp	loop

out:		mov	si, 0Bh
		jmp	bp
back:		lea	dx, [sdResult+si]
		mov	ah, 9
		int	21h

		mov	ax, 4c01h
		int	21h

sdMessage	db	"write the operation: (num1 +|-|*|/ num2)", 0Dh, 0Dh, 0Ah, 24h
sdResult	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24h
sdNotANum	db	"NaN", 0Dh, 0Dh, 0Ah, 24h

NaN:		mov	dx, sdNotANum
		mov	ah, 9
		int	21h

		mov	ax, 4c01h
		int	21h

itoa:		mov	cx, 0Ah
itoaLoop:	xor	dx, dx
		div	cx
		add	dl, 30h
		dec	si
		mov	[sdResult+si], dl
		test	ax, ax
		jnz	itoaLoop
		test	bx, bx
		jz	back
		mov	ax, bx
		xor	bx, bx
		jmp	itoaLoop
		jmp	back	

funcOffset:	mov	ax, bx
		xor	dx, dx
		cmp	cx, 0
		je	NaN
		idiv	cx
		xor	bx, bx
		jmp	itoa

		times	2*alloc-$+funcOffset nop

offset1:	mov	ax, bx
		sub	ax, cx
		xor	bx, bx
		jmp	itoa

		times	2*alloc-$+offset1 nop

offset2:	mov	ax, bx
		add	ax, cx
		xor	bx, bx
		jmp	itoa

		times	alloc-$+offset2 nop

		mov	ax, bx
		xor	dx, dx
		imul	ax, cx
		mov	dx, bx
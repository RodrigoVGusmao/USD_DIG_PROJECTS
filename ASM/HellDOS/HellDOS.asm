ORG 100h

		jmp	start
cdsMessage:	db	0B7h,9Ah, 93h, 93h, 90h, 0DFh, 0B2h, 0ACh, 0D2h, 0BBh, 0B0h, 0ACh, 0DEh, 0F2h, 0F5h, 0DBh

		mov	BYTE [bx], 50
		nop
		nop
		mov	BYTE [ebx], 50

start:		mov	bx, cdsMessage-1
dec_loop:	inc	bx
		not	BYTE [bx]
		cmp	BYTE [bx], 24h
		jnz	dec_loop

		mov	ah, 09
		mov	dx, cdsMessage
		int	21h

		mov	ax, 4C01h
		int	21h
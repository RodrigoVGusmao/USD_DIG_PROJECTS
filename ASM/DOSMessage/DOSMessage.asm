ORG 100h

szMessage 	db	"Eu estive aqui", 0Dh, 0Ah, 24h

		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop

		push	cs
		pop	ds

		mov	dx, szMessage
		mov	ah, 9
		int	21h

		mov	ax, 4C01h
		int	21h
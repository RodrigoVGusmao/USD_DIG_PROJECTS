bits 16
org 100h

		mov	dx, msMessage
		mov	ah, 9
		int	21h

		mov	ax, 4C01h
		int	21h

msMessage	db	"This is just a test!!", 0Dh, 0Dh, 0Ah, 24h
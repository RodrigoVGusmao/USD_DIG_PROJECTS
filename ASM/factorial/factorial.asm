bits 64
default rel

SEGMENT .text

global _factorial
global _DllMain

export _factorial
export _DllMain

_factorial:	mov	eax, 1
		
		cmp	ecx, 1
		jbe	.zero

		mov	r8d, 1
.for_loop:	inc	r8d
		mul	r8d

		cmp	r8d, ecx
		jne	.for_loop

.zero:		ret

_DllMain	mov	eax, 1
		ret



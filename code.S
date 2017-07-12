#include "constants.h"

.pool
.text
.code 16
.global _start
_start:
	push	{lr}

	## Message
	ldr	r0, =SHELLCODE_MESSAGE
	ldr	r1, =IOLOG_ADDR
	blx	r1

	## Flush cache
	ldr r4, =FLUSH_DCACHE_ADDR
	blx r4

	ldr r4, =INVALIDATE_DCACHE_ADDR
	blx r4

_end:
	pop	{pc}

.pool
.align 2
SHELLCODE_MESSAGE: .asciz “nicolas is here!\n"
.end


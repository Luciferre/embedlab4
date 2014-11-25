/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */


#include <asm.h>
#include <../include/bits/swi.h>
#include <../include/bits/errno.h>

	.file	"read.S"
	.text

	.global	read
FUNC(read)
read:
	swi	READ_SWI
	LDR     r1, =errno
	MOV     r2, r0
        cmp     r0, #0	
        bge     L2   
	neg	r2, r2
        MOV     r0, #-1
L2:
        STR     r2,[r1]
        bx      lr

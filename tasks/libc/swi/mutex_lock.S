/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
	.file "mutex_lock.S"

FUNC(mutex_lock)
	swi	MUTEX_LOCK
	LDR     r1, =errno
	MOV     r2, r0
        cmp     r0, #0	
        bge     L2   
	neg	r2, r2
        MOV     r0, #-1
L2:
        STR     r2,[r1]
        bx      lr

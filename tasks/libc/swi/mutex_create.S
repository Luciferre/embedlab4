/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 *
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 */

#include <asm.h>
#include <bits/swi.h>
	.file "mutex_create.S"

FUNC(mutex_create)
	swi	MUTEX_CREATE
	LDR     r1, =errno
	MOV     r2, #0
        cmp     r0, #0	
        bge     L2  
	MOV     r2, r0 
	neg	r2, r2
        MOV     r0, #-1
L2:
        STR     r2,[r1]
        bx      lr



/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
	.file "mutex_unlock.S"

FUNC(mutex_unlock)
	ldr	r3, =errno
	swi	MUTEX_UNLOCK
	cmp	r0, #0
	bge	ret
	neg	r0, r0
	str	r0, [r3]
	mov	r0, #-1
ret:
	mov	pc, lr

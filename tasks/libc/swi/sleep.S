/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 *
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 */

#include <asm.h>
#include <bits/swi.h>

	.file "sleep.S"
	.text
	.global sleep

FUNC(sleep)
	swi	SLEEP_SWI
	mov	pc, lr

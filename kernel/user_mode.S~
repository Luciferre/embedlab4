/* user_mode.S: set usermode stack and switch to user mode
 * Author: shan gao <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 *
 * Date:   2014-10-30 2014
 */

#include <bits/swi.h>
	.file "user_mode.S"
	.text

	.global user_mode

user_mode:
	sub 	sp, sp, #4
@	stmfd	sp!, {r8}
	stmfd	sp!, {r0-r12, lr}
	ldr	r3, =ksp
	str	sp, [r3]
	ldr     r3, =r8
    	str     r8, [r3]
	mrs	r4, cpsr
	bic	r4, r4, #0xff
	orr	r4, r4, #0x50
	msr	cpsr, r4

	ldr	sp, =0xa3000000
	@set up user stack
	stmfd	sp!,{r0}
	stmfd	sp!,{r1}
	ldr	pc, =0xa0000000
	

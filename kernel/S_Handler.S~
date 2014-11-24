/* S_Handler.S: prepare to call the SWI c handler.
                restore R8 and save other registers
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 *
 * Date:   2014-10-30  2014
 */

	.file "S_Handler.S"
	.text
	.global S_Handler
	.global C_SWI_Handler
S_Handler:
	SUB	sp, sp, #4		@leave room on stack for SPSR
	STMFD	sp!, {r0-r12, lr}	@store user's gp registers
	mrs     r4, cpsr
    eor     r4, r4, #0x80
    msr     cpsr, r4

	ldr     r3, =r8
    ldr     r8, [r3]

	MRS	r2, spsr		@get SPSR into gp registers
	STR	r2, [sp, #14*4]		@store SPSR above gp registers
	MOV	r1, sp			@pointer to parameters on stack
	LDR	r0, [lr, #-4]		@extract the SWI number
	BIC	r0,r0,#0xff000000	@get SWI # by bit-maskingù)
	BL	C_SWI_Handler		@goto handler (see púsp!‚rev lecture)
	LDR	r2, [sp, #14*4]		@restore SPSR (NOT ‚
	MSR	spsr, r2		@restore SPSR from r2
	LDMFD	sp!, {r0-r12, lr}	@unstack user's registers
	ADD	sp, sp, #4		@remove space used to store SPSR
	MOVS	pc, lr			@return from handler

@ irqHandler.S: IRQ handler, call c function
@
@ Author: jian wang<jianw3@andrew.cmu.edu>
@ 	Shan Gao <shang@andrew.cmu.edu>
@ Date: Oct 31st 2014
#include <asm.h>
#include <arm/interrupt.h>
.file "irqHandler.S"
FUNC(irqHandlerAsm)
irqHandlerAsm:
    stmfd sp!, {r0-r12,lr}

    @call c function
    bl    irqHandler

    @enable irq
    mov   r1, #INTERRUPT_MASK
    mrs   r2, cpsr
    eor   r2, r1
    msr   cpsr, r2

    ldmfd sp!, {r0-r12,lr}
    sub   lr, lr, #4
    movs  pc, lr

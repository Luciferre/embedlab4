/*
 *  timerIrqSetup.c: set the interrupt controller and
 *  OS timer register
 *
 *  Authors: jian wang<jianw3@andrew.cmu.edu>
 *           shan gao <shang@andrew.cmu.edu>
 *  Date: 12:30 Am Oct. 31th 2014
 */
#include <types.h>
#include <arm/reg.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
void timerIrqSetup(){
    /*clear the OSTMR interrupt bit in the ICLR to generates IRQ*/
    reg_clear(INT_ICLR_ADDR, 1<<INT_OSTMR_0);
    /*enable the os timer interrupt*/
    reg_write(INT_ICMR_ADDR, 1<<INT_OSTMR_0);
    /*set the value of OSMR 0 to 10ms*/
    reg_write(OSTMR_OSMR_ADDR(0),OSTMR_COUNT);
    //reg_write(OSTMR_OSMR_ADDR(0),3250);
    /*enable the OSMR0 */
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    reg_clear(OSTMR_OSCR_ADDR, 0xffffffff);
}

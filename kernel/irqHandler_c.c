/* irqHander_c.c: handle the interruput.If not timer interrupt, return.
*               for timer interrupt, update the global variable system_time
*               according to the OSCR register and reset it.
* Author: shang <shang@andrew.cmu.edu>
*         jian wang <jianw3@andrew.cmu.edu>
*
* Date:   2014-10-30 2014
*/
#include <types.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

volatile unsigned long system_time=0;
volatile unsigned long time_drift=0;
void irqHandler(){
    unsigned volatile int oscrValue, icprValue;

    icprValue = reg_read(INT_ICPR_ADDR);

    if((icprValue & (1<<INT_OSTMR_0)) == 0)
       return;
//    system_time++;

    oscrValue = reg_read(OSTMR_OSCR_ADDR);

    system_time += oscrValue/OSTMR_COUNT;

    time_drift += oscrValue % OSTMR_COUNT;

    if(time_drift >= OSTMR_COUNT){
        system_time++;
        time_drift -= OSTMR_COUNT;
    }
    reg_write(OSTMR_OSCR_ADDR,0);
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

}

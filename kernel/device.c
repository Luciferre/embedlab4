/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 *
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <bits/errno.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent
 * tasks with logical devices.
 * These logical devices should be signalled periodically
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update
 * on every timer interrupt. In dev_update check if it is
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_q;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 9000};
//const unsigned long dev_freq[NUM_DEVICES] = { 100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	int i;
	for (i = 0; i < NUM_DEVICES; i++) {
        	devices[i].sleep_q = 0;
        	devices[i].next_match = dev_freq[i];
    	}
}

/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
int dev_wait(unsigned int dev __attribute__((unused)))
{
	disable_interrupts();

  	tcb_t* cur_tcb = get_cur_tcb();
    if(cur_tcb->holds_lock !=0){
        return -EHOLDSLOCK;
    }
 	tcb_t* sleep_queue = devices[dev].sleep_q;
  	if(sleep_queue == 0)
	{
		devices[dev].sleep_q = cur_tcb;
		devices[dev].sleep_q -> sleep_queue = 0;
	}
	else{
		while(sleep_queue -> sleep_queue !=0)
			sleep_queue = sleep_queue -> sleep_queue;
		sleep_queue->sleep_queue = cur_tcb;
		sleep_queue = sleep_queue-> sleep_queue;
		sleep_queue -> sleep_queue =0;
	}

  	dispatch_sleep();
	enable_interrupts();
	
	return 0;
}

/**
 * @brief Signals the occurrence of an event on all applicable devices.
 * This function should be called on timer interrupts to determine that
 * the interrupt corresponds to the event frequency of a device. If the
 * interrupt corresponded to the interrupt frequency of a device, this
 * function should ensure that the task is made ready to run
 */
void dev_update(unsigned long millis __attribute__((unused)))
{
	disable_interrupts();
	int i = 0;
	bool_e flag = FALSE;
	for (i = 0; i < NUM_DEVICES; i++) {
	        if (devices[i].next_match <= millis) {
        		devices[i].next_match += dev_freq[i];
            		tcb_t* sleep_queue = devices[i].sleep_q;

            		if (sleep_queue != 0) {
				flag = TRUE;
               			 while(sleep_queue != 0) {
				    	runqueue_add(sleep_queue,sleep_queue->native_prio);
				   	sleep_queue = sleep_queue->sleep_queue;
				 }
              			 devices[i].sleep_q = 0;

           		 }
		}
	}

	if(flag == TRUE)
		 dispatch_save();
	enable_interrupts();

}

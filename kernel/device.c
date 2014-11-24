/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

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
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	printf("dev_init\n");
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
void dev_wait(unsigned int dev __attribute__((unused)))
{ 
	disable_interrupts();	

  	tcb_t* cur_tcb = get_cur_tcb();
	printf("dev_wait sleep %d\n", cur_tcb->native_prio);
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
	//printf("dev_update\n");
	int i = 0;
	bool_e flag = FALSE;	
	for (i = 0; i < NUM_DEVICES; i++) {
	        if (devices[i].next_match == millis) {
        		devices[i].next_match += dev_freq[i];
            		tcb_t* sleep_queue = devices[i].sleep_q;
            		if (sleep_queue != 0) {
				flag = TRUE;
               			 while(sleep_queue != 0) {
					printf("dev_update wake up %d\n", sleep_queue->native_prio);
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


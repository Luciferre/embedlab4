/** @file sched.c
 *
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 *
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */
uint32_t idle_stack[OS_KSTACK_SIZE/sizeof(uint32_t)] __attribute__((aligned(8)));

void sched_init(task_t* main_task  __attribute__((unused)))
{

}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */

static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	unsigned int i;
	dev_init();
	runqueue_init();
	for(i = 0; i < num_tasks; i++)
	{

		system_tcb[i+1].native_prio = i+1;
		system_tcb[i+1].cur_prio = i+1;
		system_tcb[i+1].context.r4 = (uint32_t)(*tasks)[i].lambda;
		system_tcb[i+1].context.r5 = (uint32_t)(*tasks)[i].data;
		system_tcb[i+1].context.r6 = (uint32_t)(*tasks)[i].stack_pos;
        	system_tcb[i+1].context.r7 = (int)system_tcb[i+1].kstack_high;
		system_tcb[i+1].context.sp = (void *)system_tcb[i+1].kstack_high;
		system_tcb[i+1].context.lr = launch_task;
		system_tcb[i+1].context.r8 = global_data;		
		system_tcb[i+1].holds_lock = 0;
		system_tcb[i+1].sleep_queue = 0;
		system_tcb[i+1].mutex_sleep_queue = 0;
		system_tcb[i+1].pending = 0;		

		runqueue_add(&system_tcb[i+1], i+1);
	}

	system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
   	system_tcb[IDLE_PRIO].context.r4 = (uint32_t) &idle;
   	system_tcb[IDLE_PRIO].context.r5 = 0;
  	system_tcb[IDLE_PRIO].context.r6 = (uint32_t)&idle_stack[0] ;
    	system_tcb[IDLE_PRIO].context.r7 = (int)system_tcb[IDLE_PRIO].kstack_high;
  	system_tcb[IDLE_PRIO].context.sp = (void *)system_tcb[i+1].kstack_high;
	system_tcb[IDLE_PRIO].context.lr = &launch_task;
	system_tcb[IDLE_PRIO].context.r8 = global_data;
	system_tcb[IDLE_PRIO].holds_lock = 0;
	system_tcb[IDLE_PRIO].sleep_queue = 0;
	system_tcb[IDLE_PRIO].mutex_sleep_queue = 0;
	system_tcb[IDLE_PRIO].pending = 0;	
	runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
	
	dispatch_nosave();

}

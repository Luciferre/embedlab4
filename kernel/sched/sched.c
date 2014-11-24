/** @file sched.c
 *
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
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
	//printf("allocate task\n");
	unsigned int i;
	dev_init();
	runqueue_init();
	for(i = 0; i < num_tasks; i++)
	{

		system_tcb[i+1].native_prio = i+1;
		system_tcb[i+1].context.r4 = (uint32_t)(*tasks)[i].lambda;
		system_tcb[i+1].context.r5 = (uint32_t)(*tasks)[i].data;
		system_tcb[i+1].context.r6 = (uint32_t)(*tasks)[i].stack_pos;
        	system_tcb[i+1].context.r7 = (int)system_tcb[i+1].kstack_high;
		system_tcb[i+1].context.sp = (void *)system_tcb[i+1].kstack_high;
		system_tcb[i+1].context.lr = launch_task;
		system_tcb[i+1].context.r8 = global_data;
		//printf("r4:%d	r5:%d	r6:%d	r7:%d\n",system_tcb[i+1].context.r4,system_tcb[i+1].context.r5,system_tcb[i+1].context.r6, system_tcb[i+1].context.r7);
		//printf("task r4:%d	r5:%d	r6:%d\n",(*tasks)[i].lambda,(*tasks)[i].data,(*tasks)[i].stack_pos);
		system_tcb[i+1].holds_lock = 0;
		system_tcb[i+1].sleep_queue = 0;
		//system_tcb[i].kstack[0] = (uint32_t)tasks[i]->stack_pos;//how to initialize?
		//system_tcb[i].kstack_high[0] = system_tcb[i].kstack[0] + OS_KSTACK_SIZE/sizeof(uint32_t); // how to initialize?

		runqueue_add(&system_tcb[i+1], i+1);
	}

	system_tcb[63].native_prio = 63;
   	system_tcb[63].context.r4 = (uint32_t) &idle;
   	system_tcb[63].context.r5 = 0;
  	system_tcb[63].context.r6 = (uint32_t)&idle_stack[0] ;
    	system_tcb[63].context.r7 = (int)system_tcb[63].kstack_high;
  	system_tcb[63].context.sp = (void *)system_tcb[i+1].kstack_high;
	system_tcb[63].context.lr = &launch_task;
	system_tcb[63].context.r8 = global_data;
	system_tcb[63].holds_lock = 0;
	system_tcb[63].sleep_queue = 0;
	//system_tcb[63].kstack[0] = (uint32_t) &idle_stack[0];//idle stack is useful?
	runqueue_add(&system_tcb[63], 63);
	//dispatch_init(&system_tcb[63]);
	dispatch_nosave();

}

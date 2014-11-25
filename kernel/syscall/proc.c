/** @file proc.c
 *
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 *
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include <lock.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	size_t i = 0;
	//num tasks is greater than the maximum number of tasks the OS supports
	if(num_tasks > OS_MAX_TASKS - 2 )
		return -EINVAL;
	//tasks points to region whose bounds lie outside valid address space

	for(i = 0; i < num_tasks; i++){
		if(!valid_addr(tasks[i].stack_pos, 0, USR_START_ADDR, USR_END_ADDR))
			return -EFAULT;
	}
	//The given task set is not schedulable – some tasks may not meet their deadlines
	for (i = 0; i < num_tasks; i++){
		if(tasks[i].T < tasks[i].C || tasks[i].T == 0)
			return -ESCHED;
	}
	if(!assign_schedule(&tasks, num_tasks))//ub_test
		return -ESCHED;
	//printf("task_CREATE r4:%x	r5:%d	r6:%x	C:%d	T:%d\n",tasks[0].lambda,tasks[0].data,tasks[0].stack_pos,tasks[0].C,tasks[0].T);
	disable_interrupts();
	allocate_tasks(&tasks, num_tasks);

  return 1; /* remove this line after adding your code */
}

int event_wait(unsigned int dev  __attribute__((unused)))
{

	if (dev > NUM_DEVICES)
  	{
		return -EINVAL;
  	}
  	dev_wait(dev);
  return 1; /* remove this line after adding your code */
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}

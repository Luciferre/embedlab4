/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 *
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	int u_n = 0, u_max;
	unsigned i, j;
	if(num_tasks == 0 || num_tasks == 1)
		return 1;	

	task_t tmp_task;
	for(i =0; i < num_tasks; i++)
	{
		u_n += ((*tasks)[i].C) / ((*tasks)[i].T);
		for(j = 0; j < num_tasks; j++)
		{
			if((*tasks)[j].T > (*tasks)[j+1].T){
				tmp_task = (*tasks)[j];
				(*tasks)[j] = (*tasks)[j+1];
				(*tasks)[j+1] = tmp_task;
			}
		}
		
	}
	
	u_max = num_tasks * ((2 << (1 / num_tasks)) - 1);
	if(u_n <= u_max)		
		return 1;
	return 0; 
}
	



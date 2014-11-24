/*C_SWI_Handler.c: a SWI handler for read, write, exit, sleep, time syscalls
 *
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 *
 *
 * Date:   9:08	OCT 15 2014
 */
#include <bits/fileno.h>
#include <exports.h>
#include <bits/swi.h>
#include <syscall.h>
#include <lock.h>
void C_SWI_Handler(unsigned num, unsigned* regs)
{
	switch(num)
	{
		//read
		case READ_SWI:
			//printf("READ_SWI %c\n", regs[1]);
			regs[0] = read_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);
			break;
		//write
		case WRITE_SWI:
			//printf("WRITE_SWI %c\n", regs[1]);
			regs[0] = write_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);
			break;
		case TIME_SWI:
			//printf("TIME_SWI\n");
			regs[0] = time_syscall();
			break;
		case SLEEP_SWI:
			//printf("SLEEP_SWI %d\n", regs[0]);
			sleep_syscall((unsigned long)regs[0]);
			break;
		case CREATE_SWI:
			//printf("CREATE_SWI %d\n", regs[1]);
		   	task_create((task_t *)regs[0], (size_t) regs[1]);
		    	break;
		case EVENT_WAIT:
			//printf("EVENT_WAIT_swi %d\n", regs[0]);
		   	event_wait((unsigned int) regs[0]);
		   	break;
		case MUTEX_CREATE:
			printf("MUTEX_CREATE_swi\n");
			mutex_create_swi();
			break;
       	 	case MUTEX_LOCK:
			printf("MUTEX_LOCK_swi %d\n", regs[0]);
			mutex_lock((unsigned int) regs[0]);
			break;
        	case MUTEX_UNLOCK:
			printf("MUTEX_UNLOCK_swi %d\n", regs[0]);
			mutex_unlock((int) regs[0]);
			break;
		default:
			//puts("instruction is unrecognized\n");
			invalid_syscall(num);
		//	exit(0x0badc0de);
			break;
	}
}

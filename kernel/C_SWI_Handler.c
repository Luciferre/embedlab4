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

void C_SWI_Handler(unsigned num, unsigned* regs)
{
	switch(num)
	{
		//read
		case READ_SWI:
			regs[0] = read_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);
			break;
		//write
		case WRITE_SWI:
            printf("swi handler write\n");
			regs[0] = write_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);
			break;
		case TIME_SWI:
			regs[0] = time_syscall();
			break;
		case SLEEP_SWI:
			sleep_syscall((unsigned long)regs[0]);
			break;
		case CREATE_SWI:
		   	task_create((task_t *)regs[0], (size_t) regs[1]);
		    	break;
		case EVENT_WAIT:
		   	event_wait((unsigned int) regs[0]);
		   	break;
		default:
			//puts("instruction is unrecognized\n");
			invalid_syscall(num);
		//	exit(0x0badc0de);
			break;
	}
}

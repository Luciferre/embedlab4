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
			printf("read_swi %c\n", regs[1]);
			regs[0] = read_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);
			break;
		//write
		case WRITE_SWI:
			printf("write_swi %c\n", regs[1]);
			regs[0] = write_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);
			break;
		case TIME_SWI:
			printf("time_swi\n");
			regs[0] = time_syscall();
			break;
		case SLEEP_SWI:
			printf("sleep_swi %c\n", regs[0]);
			sleep_syscall((unsigned long)regs[0]);
			break;
		case CREATE_SWI:
			printf("task_create_swi %c\n", regs[1]);
		   	task_create((task_t *)regs[0], (size_t) regs[1]);
		    	break;
		case EVENT_WAIT:
			printf("event_swi %c\n", regs[0]);
		   	event_wait((unsigned int) regs[0]);
		   	break;
		default:
			//puts("instruction is unrecognized\n");
			invalid_syscall(num);
		//	exit(0x0badc0de);
			break;
	}
}

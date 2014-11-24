/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>
#include "bits/swi.h"

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f
#define SDRAMStart 0xa0000000
#define SDRAMEnd 0xa3ffffff
#define SDRAMStart 0xa0000000
#define SDRAMEnd 0xa3ffffff
#define ROMStart 0x00000000
#define ROMEnd 0x00ffffff

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{
	unsigned i;	
	char *buffer = (char*) buf;
	char tmp;
	
	if(fd != STDIN_FILENO) 
		return -EBADF;   

	if((count > (SDRAMEnd - SDRAMStart)) || (buf < (void*)SDRAMStart) || (buf > (void*)SDRAMEnd))
		return -EFAULT;

	for(i = 0; i < count; i++){
		tmp = getc();
		if (tmp == DEL_CHAR || tmp ==0x08){
			if(i > 0){
				puts("\b \b");
				buffer[i--] = '\0';
				i--;
			}

		}
		else if(tmp == EOT_CHAR)
			return i;
		else if( tmp == '\r' || tmp == '\n'){
			buffer[i] = '\n';
			putc('\n');
			return i;
		}
		else{
			buffer[i] = tmp;
			putc(tmp);
		}
	}
	
	return count;
	
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

  	unsigned i;
	char *buffer = (char *) buf;

	if(fd != STDOUT_FILENO)
	{	
		return -EBADF;
	}
	if(( (((unsigned)buf >ROMEnd)&&((unsigned)buf < SDRAMStart)) || ((unsigned)buf > SDRAMEnd) || ( (((unsigned)buf + count) > ROMEnd)&&(((unsigned)buf + count) < SDRAMStart)) ) || (((unsigned)buf + count) > SDRAMEnd))
	{
		return -EFAULT;
	}	
	for( i = 0; i < count; i++)
	{
		putc(buffer[i]);
	}
	return count;
	
}


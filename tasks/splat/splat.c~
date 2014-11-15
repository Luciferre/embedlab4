/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Authors: shang <shang@andrew.cmu.edu>          
 * Date:    9:22 Nov 3
 * Links to libc.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	while(1)
	{
		write(STDOUT_FILENO, "|", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "/", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "-", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
		write(STDOUT_FILENO, "\\", 1);
		sleep(200);
		write(STDOUT_FILENO, "\b \b", 3);
	}
	return 0;
}

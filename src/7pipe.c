//This is the same program from the lab03_demo7.c 
/*
	program runs "ls | sort" command using 2 child processes
	
	dup2 is used to duplicate an old file descriptor into a new one
	normal file descriptor table
	
		0 [ standard input  ]
		1 [ standard output ]
		2 [ standard error  ]	

	pipe is used to communicate between child processes
	unused ends of the pipe should be closed if unused
	in that process
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int fds[2];
	pipe(fds);
	// Child 1 will duplicate downstream into stdin
	if(fork() == 0)
	{
		dup2(fds[0], 0); // normally 0 is for stdin
				 // will now read to fds[0]
				 // end of pipe 
		close(fds[1]);   // close other end of pipe
		execlp("sort", "sort", 0);
		//execlp("wc", "wc", "-l", 0); // Note the first argument is the command
					       // After it are the arguments including
					       // original command
	}
	
	// Child2 duplicates upstream into stdout
	else if (fork() == 0)
	{
		dup2(fds[1], 1); // normally 1 is for stdout
				 // will now write to fds[1] 
				 // end of pipe
		close(fds[0]);   // close other end of pipe
		execlp("ls", "ls", 0);
	}
	// Parent
	else
	{
		close(fds[0]);
		close(fds[1]);
		wait(0);
		wait(0);
	}
}

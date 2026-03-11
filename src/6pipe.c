// a simple program to demonstrate pipe usage in C
//this program creates a shell, that takes only one word command, and executes it in the child process
//using the execlp function, it can't execute commands with arguments like "ls -l", nor can it execute commands with pipe symbol like "ls | sort",

//the structure of pipe:
//the child process executes the command, and its output is sent to the write end of the pipe, not the terminal yet
//the parent process reads from teh read end of the pipe, and prints the output to the terminal/standard output/stdout
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>     


int main()
{
    int fd[2];

    if (pipe(fd) == -1)
    {
        printf("Pipe creation failed\n");
        exit(1);
    }

    char buf[100];
    gets(buf);
    if (strcmp(buf, "exit") == 0) // if user type exit, strcmp returns 0, else 1
    {
        printf("Exiting program...\n");
        exit(0);
    }


    int pid = fork();

    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);    
    }


    else if  (pid ==0)
    { // child process
        close(fd[0]);  // close unused read end
        dup2(fd[1], 1);     //redirect stdout to pipe write end
        execlp(buf, buf, NULL); //this command will run on the child process, and it's output won't be printed on the terminal
        //instead, it will be sent to the write end of the pipe
        //if execlp fails, the following lines will be executed
        //if execlp runs correctly, control is transferred to the new program, the follwoing lines won't be executed
        printf("Command not found: %s\n", buf);
        exit(1);
    }

    else
    {
        //parent process
        close(fd[1]); //close the unused write end
        wait(NULL);  
        char parentBuf[100];
        read(fd[0], parentBuf, sizeof(parentBuf)); //read from the read end
        printf("Output from child process:\n%s", parentBuf);
    }
    return 0;
}
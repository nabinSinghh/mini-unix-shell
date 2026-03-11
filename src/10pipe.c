//this program is extension of the 9pipe.c program, but here we can execute both single and double word commands, separated by |
//here, it checks if there is a pipe symbol in the command line input from the user, and based on it, 
//if there is no pipe symbol, it will not create a secon child process using the fork() system call
//and it will just execute the command as a single command using execvp function, and print the output to the standard output
//it won't make use of the pipe() system call if there is no pipe symbol in the command line input
//if there is |, it will work like the 9pipe.c program, creating two child processes, and executing the two commands with pipe communication between them, 
//and the output of the first command is sent to the second command through the pipe, and the second command prints the output to the terminal/standard output/stdout

//but this program still doesnt' have output redirction support using '>' symbol
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>     

#define MAX_ARGS 20

int main()
{
    char cmd_line[256];
    char *args[MAX_ARGS];  // array of pointers to store tokens/arguments

    while(1)
    {
        printf("\nThis is my shell\n");
        printf("Enter your command, type 'exit' to quit\n");
        gets(cmd_line);   //read first command
        if (strcmp(cmd_line, "exit") == 0)
        {
            printf("Exiting program...\n");
            exit(0);
        }

        int i = 0;
        args[i] = strtok(cmd_line, " ");
        
        while (args[i] != NULL)
        {
            printf("Token %d: %s\n", i, args[i]);
            i++;
            args[i] = strtok(NULL, " ");
        }
        printf(" the total number of tokens is %d\n", i);

        args[i] = NULL;   //execvp requirement, last argument must be NULL, so that execvp knows where the arguments end
        //this nullization was already done in the previous while loop, but just to be sure, we do it again here

        // Find the pipe position
        int pipe_index = -1;
        for (int j = 0; j < i; j++)
        {
            if (strcmp(args[j], "|") == 0)
            {
                pipe_index = j;
                break;
            }
        }

        // Split the array at the pipe
        if (pipe_index != -1)
        {
            args[pipe_index] = NULL;  // Terminate first command at pipe
        }

        int fd[2];
        if (pipe_index != -1)  // Only create pipe if needed
        {
            if (pipe(fd) == -1)
            {
                 printf("Pipe creation failed\n");
                 exit(1);
            }
        }

        if (fork() == 0)
        {
            // Child 1: execute first command
            close(fd[0]);  // close unused read end
            if (pipe_index == -1)
            {
                // No pipe found, execute single command
                execvp(args[0], args); //execute command with args
                // only runs if exec fails
                perror("Command not found");
                exit(1);
            }
            else
            {
                // Pipe found, execute first command
                dup2(fd[1], 1);    //redirect stdout to pipe write end
                execvp(args[0], args); //execute first command with args
                // only runs if exec fails
                perror("Command not found");
                exit(1);
            }
        }

        if (pipe_index != -1)  // Only create Child 2 if pipe exists
        {
            if (fork() == 0)
            {
                // Child 2: execute second command
                close(fd[1]); //close unused write end
                dup2(fd[0], 0);    //redirect stdin to pipe read end
                execvp(args[pipe_index + 1], &args[pipe_index + 1]); //execute second command
                perror("Command not found");
                exit(1);
            }
        }

        // Parent process
        if (pipe_index != -1)
        {
            close(fd[0]);
            close(fd[1]);
        }
        wait(NULL);
        if (pipe_index != -1)
            wait(NULL);  // Wait for Child 2 only if it exists
    }
    return 0;
}
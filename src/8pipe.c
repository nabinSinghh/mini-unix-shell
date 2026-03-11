//this program is the extension of the 6pipe.c program
//everything is the same, except now the program can take the two piped commands from user istead of hardcoding them
//but still, only one word commands are allowed, no arguments, since we are using execlp function, not execvp function
//and the command must be of two commands form only, separated by a pipe symbol "|", like
//"ls | sort", "ps | wc", ps | grep", etc.
//if user type only one command(not piped command), it won't work


//Working method:
//the child process executes the first command, and its output is sent to the write end of the pipe, not the terminal yet
//the second child process reads from the read end of the pipe, and executes the second command, 
//which prints the output to the terminal/standard output/stdout
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
        args[i] = strtok(cmd_line, " |");
        
        while (args[i] != NULL)
        {
            printf("Token %d: %s\n", i, args[i]);
            i++;
            args[i] = strtok(NULL, " |");
        }
        printf(" the total number of tokens is %d\n", i);
        args[i] = NULL;   //execvp requirement, last argument must be NULL, so that execvp knows where the arguments end
        //this nullization was already done in the previous while loop, but just to be sure, we do it again here

        int fd[2];
        if (pipe(fd) == -1)
        {
            printf("Pipe creation failed\n");
            exit(1);
        }

        if (fork() == 0)
        {
            // Child process
            close(fd[0]);  // close unused read end
            dup2(fd[1], 1);    //redirect stdout to pipe write end
            execlp(args[0], args[0], 0); //execute first command
            // only runs if exec fails
            perror("Command not found");
            exit(1);
        }

        else if (fork() == 0)
        {
            // second child process
            close(fd[1]); //close unused write end
            dup2(fd[0], 0);    //redirect stdin to pipe read end
            execlp(args[1], args[1], 0); //execute second command
        }

        else
        {
            // Parent process
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}
//this program is inbuilt to need two commands separated by a pipe(|) symbol[piped command] from the user to run, 
//else(non piped command) it will not run correctly, however, it can take commands with arguments like "ls -l", "ps -aux", etc.
//the program creates two child processes, the first child process executes the first command, 
//and its output is sent to the write end of the pipe, not the terminal yet
//the second child process reads from the read end of the pipe, and executes the second command, 
//which prints the output to the terminal/standard output/stdout

//IN short, it's just like 8pipe.c, but now the program can take commands with arguments, not just single word commands
//because we are using execvp function instead of execlp function,
//it checks if there is a pipe symbol in the command line input from the user
//and the | symbol isn't used as a delimiter while tokenizing, instead we tokenize based on space character only
//and we find the position of the |, so the commands beofre | runs in the first child process, 
//and the commands after | runs in the second child process, and the output of the first command is sent to the second command through the pipe
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
        if (pipe_index != -1)  //this means, a | symbol was found at args[pipe_index]
        {
            args[pipe_index] = NULL;  // Terminate first command at pipe
        }//the | was replaced with NULL to separate the two commands, execvp runs with the argument, and stops at NULL
        //this NULL works as a delimiter for execvp to know where the arguments of the first command end, 
        //and where the arguments of the second command start
        else
        {
            printf("No pipe symbol found. Please enter a command with a pipe.\n");
            continue; // prompt user again
        }
        

        int fd[2];
        if (pipe(fd) == -1)
        {
             printf("Pipe creation failed\n");  
             exit(1);
        }

        if (fork() == 0)
        {
            // Child 1: execute first command
            close(fd[0]);  // close unused read end
            dup2(fd[1], 1);    //redirect stdout to pipe write end
            execvp(args[0], args); //execute first command with args
            // only runs if exec fails
            perror("Command not found");
            exit(1);
        }

        else if (fork() == 0)
        {
            // Child 2: execute second command
            close(fd[1]); //close unused write end
            dup2(fd[0], 0);    //redirect stdin to pipe read end
            execvp(args[pipe_index + 1], &args[pipe_index + 1]); //execute second command
            //in above, &args[pipe_index + 1] is the pointer to the first argument of the second command, 
            //we used Pipe_index + 1 to skip the pipe symbol, which was set to NULL earlier, and pass the rest of the arguments as well
            perror("Command not found");
            exit(1);
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
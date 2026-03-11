//this program is extension of the 9pipe.c program,
//it checks if there is a pipe symbol in the command line input from the user
//if there is no pipe symbol, it will not create a secon child process using the fork() system call
//and it will just execute the command as a single command using execvp function, and print the output to the standard output
//it won't make use of the pipe() system call if there is no pipe symbol in the command line input

//and this program has both the input redirction support using '<' symbol and output redirection using '>' symbol
#include <stdio.h>  
#include <stdlib.h> //for exit()
#include <unistd.h> //for fork(), pipe(), dup2(), execvp()
#include <fcntl.h>  //for open()
#include <sys/wait.h> //for wait() 
#include <string.h>     //for strtok() and strcmp()

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
           // printf("Token %d: %s\n", i, args[i]);
            i++;
            args[i] = strtok(NULL, " ");
        }
        //printf(" the total number of tokens is %d\n", i);

        args[i] = NULL;   //execvp requirement, last argument must be NULL, so that execvp knows where the arguments end
        //this nullization was already done in the previous while loop, but just to be sure, we do it again here


        int output_redir_index = -1;
        for (int j = 0; j < i; j++)
        {
            if (strcmp(args[j], ">") == 0)
            {
                output_redir_index = j; //saves teh index of > in output_redir_index variable
                break;
            }
        }

        int input_redir_index = -1;
        for (int j = 0; j < i; j++)
        {
            if (strcmp(args[j], "<") == 0)
            {
                input_redir_index = j;  //the index of < is saved in input_redir_index variable
                break;
            }
        }


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
            
            // Handle input redirection FIRST (applies to both piped and non-piped)
            if (input_redir_index != -1)
            {
                int in_fd = open(args[input_redir_index + 1], O_RDONLY, 0644);
                if (in_fd == -1) {
                    perror("Cannot open input file");
                    exit(1);
                }
                dup2(in_fd, 0);  // Redirect stdin FROM file
                close(in_fd);
                args[input_redir_index] = NULL;  // Remove < from args
            }

            if (pipe_index == -1) //no pipe symbol | found, execute single command
            {
                if (output_redir_index == -1) //no output redirection symbol '>' either
                {                    
                    execvp(args[0], args); //execute command with args
                    // only runs if exec fails
                    perror("Command not found");
                    exit(1);
                }

                else
                {
                    //Output redirection for the single non-piped command, redirect output to the file specified after '>'
                    args[output_redir_index] = NULL; // Terminate command at output redirection
                    int out_fd = open(args[output_redir_index + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if (out_fd == -1) {
                        perror("Cannot open output file");
                        exit(1);
                    }
                    dup2(out_fd, 1); // Redirect stdout to the file
                    close(out_fd);
                    execvp(args[0], args); // Execute command
                    // only runs if exec fails
                    perror("Command not found");
                    exit(1);
                }
            }

            else // Pipe found, execute first command, and redirect output to pipe
            //there can't be first the > output redirectoin symbol and then the |, so chill...
            {
                close(fd[0]);  // close unused read end
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
                close(fd[1]); //close unused write end
                if (output_redir_index == -1) //no output redirection symbol '>' found, execute second command normally
                {
                    // Child 2: execute second command
                    dup2(fd[0], 0);    //redirect stdin to pipe read end
                    execvp(args[pipe_index + 1], &args[pipe_index + 1]); //execute second command
                    perror("Command not found");
                    exit(1);
                }

                else //output redirection symbol '>' found, redirect output of second command to the file specified after '>'
                {
                    args[output_redir_index] = NULL; // Terminate second command at output redirection
                    int out_fd = open(args[output_redir_index + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if (out_fd == -1) {
                        perror("Cannot open output file");
                        exit(1);
                    }
                    dup2(out_fd, 1); // Redirect stdout to the file
                    close(out_fd);
                    // Child 2: execute second command
                    dup2(fd[0], 0);    //redirect stdin to pipe read end
                    execvp(args[pipe_index + 1], &args[pipe_index + 1]); //execute second command
                    perror("Command not found");
                    exit(1);
                }
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
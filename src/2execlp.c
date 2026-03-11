//a simple shell proram whith the demosntration of the execlp function
//it first reads the command from the user using gets() function
//then it creates a child process which executes the command from the user using the execlp function
//this progarm can run for one command only by now, it doesn't support arguments yet, since strok isn't used yet,
//nor it supports redirection or piping yet.
//arguments support and tokenization using strtok will be added in the next program 4strtok.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() 
{
    char line[256];
    while (1) 
    {
        printf("myshell> ");
        gets(line);   //read command

        // exit condition
        if (strcmp(line, "exit") == 0)   //strcmp compare two strings, here, [string stored in line] and ["exit"]
        {                           // if they are same, strcmp function returns 0, and if 0 == 0, hten true
            printf("Exiting shell...\n");
            break;
        }

        pid_t pid = fork();

        if (pid == 0) 
        {
            // CHILD PROCESS
            execlp(line, line, NULL);

            // exec runs only if it fails
            printf("Command not found: %s\n", line);
            exit(1);
        }

        else if (pid > 0) 
        {
            // PARENT PROCESS
            wait(NULL);
        }

        else 
        {
            // fork failed
            perror("fork failed");
        }
    }

    return 0;
}

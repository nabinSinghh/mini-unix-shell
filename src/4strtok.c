//this program is the extension of the 2practice.c program
//it adds the tokenization part using strtok function to split the command line into command and arguments
//then it uses execvp function to execute the command with arguments
//but it still does not support redirection or piping yet.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 20

int main()
{
    char line[256];
    char *args[MAX_ARGS];  // we will use the array of pointers(pointer to char) to store the tokens/arguments
              //so that, each pointers point to each argument string

    while (1)
    {
        printf("myshell> ");
        fflush(stdout);
        gets(line);   //read command

        //exit builtin
        if (strcmp(line, "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }

        //TOKENIZATION (NEW PART) 
        int i = 0;
        args[i] = strtok(line, " ");

        while (args[i] != NULL)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL;   //execvp requirement, last argument must be NULL, so that execvp knows where the arguments end
        //this nullization was already done in the previous while loop, but just to be sure, we do it again here


        pid_t pid = fork();

        if (pid == 0)
        {
            // CHILD
            execvp(args[0], args);

            // only runs if exec fails
            perror("Command not found");
            exit(1);
        }

        else if (pid > 0)
        {
            // PARENT
            wait(NULL);
        }
        
        else
        {
            perror("fork failed");
        }
    }

    return 0;
}

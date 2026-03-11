//this program demonstrates the use of gets() to read the user input
//a simple shell that runs all the time, and wait for user to input the command, and 
//echo back the command user typed in.

//it however, does not execute the command yet., that function is added in the 2practice.c

#include <stdio.h>
#include <string.h>

int main()
{
    char line[256];

    while (1)
    {
        printf("myshell> ");

        //fgets(line, sizeof(line), stdin);
        gets(line);
        
        printf("You typed: %s\n", line);
    }

    return 0;
}

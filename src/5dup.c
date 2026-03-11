// a simple program that demonstrates the use of dup() system call, it duplicates the standard output file descriptor to a file
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>


int main()
{
    int fd;
    fd = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);    

    dup2(fd, 1);  //duplicate fd to standard output (1)
    //in simple terms, it transfers what it was to be done to the standard output to the file instead
    //control transfers from right to left.
    printf("This will be wirtten to the file outpu.txt, but no in the standard output/stdout/terminal\n");
    close(fd);
    return 0;
}
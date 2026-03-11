// Example program demonstrating the use of strtok to tokenize a string
#include <stdio.h>
#include <string.h>

int main() 
{
    char line[100];

    printf("Enter a line: ");
    gets(line);

    char *token;  //this is the pointer to the string/token

    token = strtok(line, " ,|");   // first token, separated by delimiter space(<space>), comma(,), or pipe(|)
    // but can be any delimiter, not just space, like comma, semicolon, | etc.

    int counter = 0;
    while (token != NULL) 
    {
        printf("Token Index %d: %s\n", counter, token);
        token = strtok(NULL, " ,|");   //that same pointer now points to the next token
        //need to use the same delimiters again
        counter++;
    }

    //how the strtok works internally:
    //when strotok is called for the first time, it takes the string to be tokenized as the first argument
    //and the delimiters as the second argument
    //it finds the first token in the string, replaces the delimiter after the token with a null character '\0'
    //and returns a pointer to the first token
    //subsequent calls to strtok should pass NULL as the first argument
    //this tells strtok to continue tokenizing the same string
    //it keeps track of its position in the string internally
    //it finds the next token, replaces the delimiter with '\0', and returns a pointer to the next token
    //this process continues until there are no more tokens left, at which point strtok returns NULL    

        //how strtok works:
    //1. First call: strtok(line, delimiters)
    //   - finds the first word
    //   - cuts it with '\0' where delimiter is
    //   - returns pointer to that word
    //
    //2. Next calls: strtok(NULL, delimiters)
    //   - continues from where it left off
    //   - finds next word
    //   - cuts it with '\0'
    //   - returns next word
    //
    //3. When no more words: strtok returns NULL    
    //NULL simple means "no more tokens", or "\0" in string terminologies


    return 0;
}

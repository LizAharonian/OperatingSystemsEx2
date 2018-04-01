#include <stdio.h>
#include <memory.h>
#include <malloc.h>

#define INPUT_SIZE 100
#define INPUT_PARAMS
int main() {
    // while (1) {
    char input[INPUT_SIZE];
    fgets(input, INPUT_SIZE, stdin);
    printf("prompt>");
    char args[INPUT_SIZE][INPUT_SIZE];

    const char s[2] = " ";
    char *token;
    //get the first token
    int i = 0;
    token = strtok(input, s);
    strcpy(args[i], token);
    //walk through other tokens
    while (token != NULL) {
        printf(" %s\n", args[i]);
        i++;
        token= strtok(NULL, s);
        strcpy(args[i], token);
    }

    return (0);


    // }
}
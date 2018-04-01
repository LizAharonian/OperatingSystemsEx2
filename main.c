#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#define INPUT_SIZE 100
#define INPUT_PARAMS 10
int main() {
    // while (1) {
    printf("prompt>");

    char input[INPUT_SIZE];
    fgets(input, INPUT_SIZE, stdin);
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
        if (token !=NULL) {
            strcpy(args[i], token);
        }
    }

    //calling execv
    /*int stat,waited,ret_code;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {  // Son
        ret_code = execvp(args[0],args[1]);
        if (ret_code == -1)
        {
            perror("exec failed ");
            exit(-1);
        }
    }
    else
    {  *//* Father *//*
        printf("Father: after fork, son proc id is %d \n",pid);
        waited = wait(&stat);   *//* stat can tell what happened *//*
        printf("Father: Son proc completed,  id is %d \n", waited);
    }*/


    return (0);


    // }
}
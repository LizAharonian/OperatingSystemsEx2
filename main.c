#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#define INPUT_SIZE 100
#define JOBS_NUM 100
#define INPUT_PARAMS 10
int main() {
    char jobs[INPUT_SIZE][INPUT_SIZE];
    int pids[JOBS_NUM];
    int j =0;

    while (1) {
        printf("prompt>");
        int isBackground = 0;
        char input[INPUT_SIZE];
        char copyInput[INPUT_SIZE];
        fgets(input, INPUT_SIZE, stdin);
        strcpy(copyInput,input);

        //remove '/n'
        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "jobs")==0) {
            for (int i=0; i<j; i++) {
                pid_t returnPid = waitpid(pids[i], NULL, WNOHANG);
                if (returnPid ==0) {
                    //todo: check number of spaces!!
                    printf("%d         %s\n", pids[i],jobs[i]);
                }
            }

        }else {

            char *args[INPUT_SIZE];

            const char s[2] = " ";
            char *token;
            //get the first token
            int i = 0;
            token = strtok(input, s);
            args[i]= token;
            //walk through other tokens
            while (token != NULL) {
                printf(" %s\n", args[i]);

                token = strtok(NULL, s);
                if (token != NULL && strcmp(token, "&") != 0) {
                    i++;
                    args[i]= token;
                } else if (token != NULL && strcmp(token, "&") == 0) {
                    isBackground = 1;
                }
            }
            i++;
            args[i] = NULL;


            //calling execv
            int pid = callExecv(args, isBackground);
            pids[j] = pid;
            // token = strtok(input, s);
            strcpy(jobs[j], copyInput);
            printf("%s", jobs[j]);
            j++;

        }
    }
    return (0);

}

int callExecv(char **args, int isBackground) {
    int stat, waited, ret_code;
    pid_t pid;
    pid = fork();
    if (pid == 0) {  // son
        ret_code = execvp(args[0], &args[0]);
        if (ret_code == -1) {
            perror("exec failed ");
            exit(-1);
        }
    } else {   //father prints pid of son
        printf("%d \n", pid);
        if (!isBackground) {
            wait(&stat);   // stat can tell what happened
        }
        //printf("Father: Son proc completed,  id is %d \n", waited);
    }
    return pid;
}
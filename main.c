#include <stdio.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

//defines
#define INPUT_SIZE 1000
#define JOBS_NUM 1000
#define HOME "HOME"
#define SUCCESS 1
#define FAIL -1
#define TRUE 1
#define FALSE 0
#define ERROR "Error in system call"

//declarations
int callExecv(char **args, int isBackground);
int cdImplementation(char *args[],char lastCdDir[INPUT_SIZE]);
void printJobs(int pids[], char jobs[JOBS_NUM][INPUT_SIZE], int j);
void makeArgs(char *args[INPUT_SIZE], char input[INPUT_SIZE], int* isBackground);
int changeSpecipicCdDir(char *dir,char lastCdDir[INPUT_SIZE], int isCdMinus);
int changeCd2Home(char lastCdDir[INPUT_SIZE]);

/**
 * main function.
 * runs the program
 * @return 0
 */
int main() {
    //allocate memory for jobs arrays
    char jobs[JOBS_NUM][INPUT_SIZE];
    int pids[JOBS_NUM];
    int j =0;
    char lastCdDir[INPUT_SIZE] ="";
    //while loop runs the shell
    while (TRUE) {
        printf("prompt>");
        int isBackground = FALSE;
        //allocate memory for input
        char input[INPUT_SIZE];
        char copyInput[INPUT_SIZE];
        //scan input from user
        fgets(input, INPUT_SIZE, stdin);
        //remove '/n'
        input[strlen(input) - 1] = '\0';
        //copy input
        strcpy(copyInput,input);
        //operate the action
        if (strcmp(input, "jobs")==0) {
            printJobs(pids,jobs,j);
        } else if (strcmp(input, "exit")==0) {
            printf("%d \n", getpid());
            exit(0);
        } else {
            //allocate memory for args array
            char *args[INPUT_SIZE];
            makeArgs(args, input, &isBackground);
            if (args[0]==NULL){
                continue;
            }
            if (strcmp(args[0],"cd")==0) {
                printf("%d \n", getpid());
                cdImplementation(args,lastCdDir);
            } else {
                //calling execv
                int pid = callExecv(args, isBackground);
                //update pids array
                pids[j] = pid;
                strcpy(jobs[j], copyInput);
                j++;
            }
        }
    }
    return (0);
}

/**
 * callExecv function.
 * @param args - array for execvp function
 * @param isBackground - tells if parent need to wait to son
 * @return pid of son
 */
int callExecv(char **args, int isBackground) {
    int stat, retCode;
    pid_t pid;
    pid = fork();
    if (pid == 0) {  // son
        retCode = execvp(args[0], &args[0]);
        if (retCode == FAIL) {
            fprintf(stderr, ERROR);
            printf("\n");
            exit(FAIL);
        }
    } else {   //father prints pid of son
        printf("%d \n", pid);
        if (!isBackground) {
            wait(&stat);   // stat can tell what happened
        }
    }
    return pid;
}

/**
 * cdImplementation function.
 * @param args - input arguments
 * @return success or failure
 */
int cdImplementation(char *args[],char lastCdDir[INPUT_SIZE]){
    if (args[1] ==NULL){

        return changeCd2Home(lastCdDir);

    } else {
        int i =0;
        while (args[i]!=NULL) {
            i++;
        }
        if (i==2 && strcmp(args[1],"-")==0){
            return changeSpecipicCdDir(lastCdDir,lastCdDir,TRUE);

        } else if (i==2&& strcmp(args[1],"~")==0) {
            return changeCd2Home(lastCdDir);
        } else {
            return changeSpecipicCdDir(args[1],lastCdDir,FALSE);
        }
    }
}
/**
 * changeSpecipicCdDir function.
 * @param dir - cd dir
 * @param lastCdDir - prev cd dir
 * @param isCdMinus - indicates if command is "cd -"
 * @return FAIL or SUCCESS
 */

int changeSpecipicCdDir(char *dir,char lastCdDir[INPUT_SIZE], int isCdMinus) {
    char cwd[INPUT_SIZE];
    getcwd(cwd,INPUT_SIZE);
    if (chdir(dir) == FAIL) {
        fprintf(stderr, ERROR);
        printf("\n");
        return FAIL;
    } else {
        if(isCdMinus==TRUE){
            printf("%s\n", dir);
        }
        strcpy(lastCdDir,cwd);

        return SUCCESS;
    }
}

/**
 * changeCd2Home function.
 * @param lastCdDir - prev cd dir
 * @return FAIL or SUCCESS
 */
int changeCd2Home(char lastCdDir[INPUT_SIZE]){
    char cwd[INPUT_SIZE];
    getcwd(cwd,INPUT_SIZE);
    if(chdir(getenv(HOME))==FAIL) {
        fprintf(stderr, ERROR);
        printf("\n");
        return FAIL;
    } else {
        strcpy(lastCdDir,cwd);
        return SUCCESS;
    }
}

/**
 * printJobs function.
 * @param pids - array of jobs pids
 * @param jobs - array of jobs commands
 * @param j - len of jobs array
 */
void printJobs(int pids[], char jobs[JOBS_NUM][INPUT_SIZE], int j){
    int flag =FALSE;
    int i;
    for (i=0; i<j; i++) {
        pid_t returnPid = waitpid(pids[i], NULL, WNOHANG);
        if (returnPid ==0) {
            flag=TRUE;
            printf("%d ", pids[i]);
            int len = strlen(jobs[i]);
            int k;
            for (k=0; k<len;k++){
                if (!((k==len-1)&&jobs[i][k]=='&')){
                    printf("%c", jobs[i][k]);

                }
            }
        }
    }
    if (flag) {
        printf("\n");
    }
}

/**
 * makeArgs function.
 * @param args - array for execvp function
 * @param input - user's input
 * @param isBackground - tells if parent need to wait to son
 */
void makeArgs(char *args[INPUT_SIZE], char input[INPUT_SIZE], int* isBackground){
    const char s[2] = " ";
    char *token;
    //get the first token
    int i = 0;
    token = strtok(input, s);
    args[i]= token;
    //walk through other tokens
    while (token != NULL) {
        token = strtok(NULL, s);
        if (token != NULL && strcmp(token, "&") != 0) {
            i++;
            args[i]= token;
        } else if (token != NULL && strcmp(token, "&") == 0) {
            *isBackground = TRUE;
        }
    }
    i++;
    args[i] = NULL;
}
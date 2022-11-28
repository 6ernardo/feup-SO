#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define _OPEN_SYS_ITOA_TEXT
#define false 0
#define true 1
#define BUFFSIZE 80

unsigned long int strToUnsignedLong(char* in){
    unsigned long int out = 0;
    for(int i = 0; in[i]!='\0'; i++){
        out*=10;
        out+=(in[i]-'0');
    }
    return out;
}

unsigned long int doStuff(unsigned long int in, float chance, float timer, int thisn){
    srand(time(NULL));
    //printf("A fazer coisas\n");
    float num = rand() / (float) RAND_MAX;
    //printf("NUMERO RANDOM %f\n", num);
    if(num < chance){
        printf("[p%d] lock on token (val = %lu)\n", thisn, in);
        sleep(timer);
        printf("[p%d] Released lock on token\n", thisn);
    }
    return(in+1);
}

int main(int argc, char* args[]){
    int rf;
    int wt;
    int thisN = 1;
    if(argc!=4){
        printf("Needs 3 arguments\n");
        return -1;
    }
    
    int n = (int)atof(args[1]);
    float timer = atof(args[2]);
    float chance = atof(args[3]);
    char pipe1[80] = "pipento1",pipe2[80] = "pipe1to2";
    for(int i = 0; i < n-1; i++){
        char pipename[80], n1[30], n2[30];
        sprintf(n1,"%d",i+1);
        sprintf(n2,"%d",i+2);
        strcpy(pipename, "pipe");
        strcat(pipename, n1);
        strcat(pipename, "to");
        strcat(pipename, n2);
        mkfifo(pipename,0777);
    }
    mkfifo("pipento1",0777);
    unsigned long int out = 1;
    char buf[8] = "1";
    
    for(int i = 1; i < n; i++)
    {
        pid_t pide = fork();
        if(pide<0){
            printf("Fork error\n");
            return -1;
        }
        else if(pide==0){
            char n1[30], n2[30];
            //CHILD
            thisN = i+1;
            sprintf(n1,"%d",thisN-1);
            sprintf(n2,"%d",thisN);
            strcpy(pipe1, "pipe");
            strcat(pipe1, n1);
            strcat(pipe1, "to");
            strcat(pipe1, n2);
            if(thisN==n){
                strcpy(pipe2, "pipento1");
                wt = open(pipe2,O_WRONLY);
                if(write(wt, buf, 8)==-1){
                    return -1;
                }
            }
            else{
                sprintf(n1,"%d",thisN);
                sprintf(n2,"%d",thisN+1);
                strcpy(pipe2, "pipe");
                strcat(pipe2, n1);
                strcat(pipe2, "to");
                strcat(pipe2, n2);
            }
            i = n;
        }
        else{
            //PARENT
        }
    }

    
    printf("Processo %d pipe1 %s \t pipe2 %s\n",thisN,pipe1,pipe2);

    

    while(true){
        //printf("Processo %d entrou no loop épico\n",thisN);
        rf = open(pipe1,O_RDONLY);
        out = read(rf,buf,8);
        out = strToUnsignedLong(buf);
        //printf("Processo %d acabou de ler %lu\n",thisN,out);
        out = doStuff(out,chance,timer,thisN);
        //printf("Processo %d acabou de fazer coisas %lu\n",thisN,out);
        wt = open(pipe2,O_WRONLY);
        if(write(wt, &out, 8)==-1){
            return -1;
        }
        //printf("Processo %d acabou de escrever\n",thisN);
    }

    return 0;
}
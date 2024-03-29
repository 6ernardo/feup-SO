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
        fprintf(stderr, "usage: tokenring n p t\n");
        exit(EXIT_FAILURE);
    }
    
    int n = (int)atof(args[1]);
    float chance = atof(args[2]);
    float timer = atof(args[3]);

    if(n<=1 || timer<0){
        printf("Invalid input\n");
        return -1;
    }

    char pipe1[80] = "pipento1",pipe2[80] = "pipe1to2";
    for(int i = 0; i < n-1; i++){
        char pipename[80];
        sprintf(pipename,"pipe%dto%d",i+1,i+2);
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
            //CHILD
            thisN = i+1;
            sprintf(pipe1,"pipe%dto%d",thisN-1,thisN);
            if(thisN==n){
                strcpy(pipe2, "pipento1");
                wt = open(pipe2,O_WRONLY);
                sprintf(buf,"%lu",(unsigned long int)1);
                if(write(wt, buf, 8)==-1){
                    return -1;
                }
            }
            else{
                sprintf(pipe2,"pipe%dto%d",thisN,thisN+1);
            }
            i = n;
        }
        else{
            //PARENT
            //NAO SE FAZ NADA
        }
    }


    

    while(true){
        rf = open(pipe1,O_RDONLY);
        out = read(rf,buf,8);
        out = strToUnsignedLong(buf);
        out = doStuff(out,chance,timer,thisN);
        wt = open(pipe2,O_WRONLY);
        sprintf(buf,"%lu",out);
        if(write(wt, buf, 8)==-1){
            printf("Write error\n");
            return -1;
        }
    }

    return 0;
}

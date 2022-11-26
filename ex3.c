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

#define _OPEN_SYS_ITOA_TEXT
#define false 0
#define true 1
#define BUFFSIZE 80

unsigned long int doStuff(unsigned long int in, float chance, float time, int thisn){
    float num = rand() / (float) RAND_MAX;
    if(num < chance){
        printf("[p%d] lock on token (val = %lu)", thisn, in);
        sleep(time);
        printf("[p%d] Released lock on token", thisn);
    }
    return(in+1);
}

int makeChild(int i, int limit, float time, float chance){
    char temp[80], n1[30], n2[30];
    char pipe1[80], pipe2[80];
    FILE* rf;
    FILE* wt;
    int thisn = i;

    unsigned long int out = 0, k = 0;
    sprintf(n1,"%d",i);
    sprintf(n2,"%d",i+1);
    strcpy(temp, "pipe");
    strcat(temp, n1);
    strcat(temp, "to");
    strcat(temp, n2);

    strcpy(pipe1, temp);

    if(i<limit-1){
        sprintf(n1,"%d",i+1);
        sprintf(n2,"%d",i+2);
        strcpy(temp, "pipe");
        strcat(temp, n1);
        strcat(temp, "to");
        strcat(temp, n2);
        
    
    
        pid_t pide = fork();
        if(pide<0){
            printf("Fork error\n");
            return -1;
        }
        else if(pide==0){
            //Nada
        }
        else{
            makeChild(i+1, limit, time, chance);
        }

    }
    else{
        strcpy(temp,"pipento1");
        k = 300;
    }
    
    strcpy(pipe2, temp);

    while(true){
        rf = fopen(pipe1, "r+");
        wt = fopen(pipe2, "w+");
        char num[80];
        fread(num,100,1,rf);
        k = (unsigned long int) atof(num);
        if(k!=out){
            out = doStuff(out,chance,time,1);
        }
        sprintf(num,"%lu",out);
        fwrite(num,strlen(num)+1,1,wt);
        fclose(rf);
        fclose(wt);
    }

    return 0;
}

int main(int argc, char* args[]){
    FILE* rf;
    FILE* wt;
    if(argc!=4){
        printf("Needs 3 arguments\n");
        return -1;
    }
    
    int n = (int)atof(args[1]);
    float time = atof(args[2]);
    float chance = atof(args[3]);
    char thispipe[80];
    for(int i = 0; i < n-1; i++){
        char pipename[80], n1[30], n2[30];
        sprintf(n1,"%d",i+1);
        sprintf(n2,"%d",i+2);
        strcpy(pipename, "pipe");
        strcat(pipename, n1);
        strcat(pipename, "to");
        strcat(pipename, n2);
        mkfifo(pipename,0666);
        strcpy(thispipe,pipename); //NO FIM VAI FICAR IGUAL A PIPE N-1 TO N
    }
    mkfifo("pipento1",0666);
    unsigned long int out = 0;
    


    pid_t pide = fork();
    if(pide<0){
        printf("Fork error\n");
        return -1;
    }
    else if(pide==0){
        //out = doStuff(out,chance,time, n);
    }
    else{
        makeChild(1, n, time, chance);
    }
    

    

    unsigned long int k = 0;
    while(true){
        rf = fopen("pipento1","r+");
        wt = fopen("pipe1to2","w+");
        char num[80];
        fread(num,100,1,rf);
        k = (unsigned long int) atof(num);
        if(k!=out){
            out = doStuff(out,chance,time,1);
        }
        sprintf(num,"%lu",out);
        fwrite(num,strlen(num)+1,1,wt);
        fclose(rf);
        fclose(wt);
    }

    return 0;
}
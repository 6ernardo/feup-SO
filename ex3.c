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

int makeChild(int i, int limit, float timer, float chance){
    char temp[80], n1[30], n2[30];
    char pipe1[80], pipe2[80];
    FILE* rf;
    FILE* wt;
    int thisn = i;

    unsigned long int out = 1, k = 0;
    sprintf(n1,"%d",i);
    sprintf(n2,"%d",i+1);
    strcpy(temp, "pipe");
    strcat(temp, n1);
    strcat(temp, "to");
    strcat(temp, n2);
    strcat(temp, ".fifo");

    strcpy(pipe1, temp);

    if(i<limit-1){
        sprintf(n1,"%d",i+1);
        sprintf(n2,"%d",i+2);
        strcpy(temp, "pipe");
        strcat(temp, n1);
        strcat(temp, "to");
        strcat(temp, n2);
        strcat(temp, ".fifo");
    
    
        pid_t pide = fork();
        if(pide<0){
            printf("Fork error\n");
            return -1;
        }
        else if(pide==0){
            //Nada
        }
        else{
            makeChild(i+1, limit, timer, chance);
        }

    }
    else{
        strcpy(temp,"pipento1.fifo");
        k = 300;
    }
    
    strcpy(pipe2, temp);

    while(true){
         
        rf = fopen(pipe1,"r");
        char num[80];
        fread(num,sizeof(num),1,rf);
        k = (unsigned long int) atof(num);
        //printf("%lu meme %lu \n",k, out);
        if((k+1)!=out){
            out = doStuff(k, chance, timer, 1);
            out = k+1;
            //printf("%lu cur Out\n",out);
            wt = fopen(pipe2,"w");
            fprintf(wt,"%lu",out);
            fclose(wt);
            
        }
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
    float timer = atof(args[2]);
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
        strcat(pipename, ".fifo");
        mkfifo(pipename,0666);

        wt = fopen(pipename,"w");
        fprintf(wt,"%lu",(unsigned long int) 0);
        fclose(wt);
    }
    char pipento1[] = "pipento1.fifo";
    mkfifo(pipento1,0666);
    wt = fopen(pipento1,"w");
    fprintf(wt,"%lu",(unsigned long int) 0);
    fclose(wt);
    
    unsigned long int out = 300;
    


    pid_t pide = fork();
    if(pide<0){
        printf("Fork error\n");
        return -1;
    }
    else if(pide==0){
        //out = doStuff(out,chance,time, n);
    }
    else{
        makeChild(1, n, timer, chance);
    }
    
    
    

    unsigned long int k = 0;
    while(true){
        
        rf = fopen("pipento1.fifo","r");
        char num[80];
        fread(num,sizeof(num),1,rf);
        k = (unsigned long int) atof(num);
        //printf("%lu meme %lu \n",k, out);
        if((k+1)!=out){
            out = doStuff(k, chance, timer, 1);
            //printf("%lu cur Out\n",out);
            wt = fopen("pipe1to2.fifo","w");
            fprintf(wt,"%lu",out);
            fclose(wt);
            out = k;
        }
        //fwrite(num,strlen(num)+1,1,wt);
        sleep(2);
        
    }

    return 0;
}
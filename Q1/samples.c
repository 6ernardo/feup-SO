#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

    if(argc!=4){
        fprintf(stderr, "usage: samples file numberfrags maxfragsize\n");
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[2]);
    int m = atoi(argv[3]);

    //abrir ficheiro, descobrir tamanho do texto
    FILE *f = fopen(argv[1], "r");

    fseek(f, 0, SEEK_END);
    long len = ftell(f);

    fseek(f, 0, SEEK_SET);

    //gerar n numeros aleatorios
    int rn[n];

    srandom(0);

    for(int i=0; i<n; i++){
        rn[i]=random() % (len-m);
    }

    //criar um imprimir n samples
    for(int j=0; j<n; j++){
        char str[m];
        fseek(f,rn[j], SEEK_SET);
        for(int k=0; k<m; k++){
            char c=fgetc(f);
            if(c=='\n'){
                c=' ';
            }
            str[k]=c;
        }
        printf(">%s<\n",str);
    }

    fclose(f);
}
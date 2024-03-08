#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
int checkarg(int argc, char *argv[]);
void print_usage_and_exit();

void print_usage_and_exit(){
    fprintf(stderr, "usage: hencode infile [outfile] \n");
    exit(-1);
}
int checkarg(int argc, char *argv[]){
    int cas = 0;
    if(argc == 1){
        print_usage_and_exit();
    }
    if(argc == 3){
        if(argv[1] == NULL || argv[2] == NULL){
            fprintf(stderr, "file not found\n");
            exit(-1);
        }
        else{
            cas = 2;
            return cas;
        }
    }
    if(argc ==2){
        if(argv[1] == NULL){
            fprintf(stderr, "file not found\n");
            exit(-1);
        }else{
            cas =3; 
            return cas;
        }
    }
    return cas;
}

int main(int argc, char *argv[]){
    

    int cas = checkarg(argc, argv);
    printf("%d", cas);

    return 0;
}
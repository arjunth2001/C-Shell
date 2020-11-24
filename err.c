#include "headers.h"
int err;
int skip;
void set_err(int n){
    err=n;
}
int geterr(){
    return err;
}
void print_err(int err_no){

    if(err_no==0){
        printf("\n:'(");
    }
    else{

        printf("\n:')");

    }
        
}
void resetskip(){
    skip=0;
}

int shouldSkip(){
    return skip;
}
void setskip(char * input){

    for(int i=0;i< strlen(input);i++){

        if(input[i]=='@'||input[i]=='$'){
            skip=1;
        }

    }

}

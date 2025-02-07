#include "utils.h"

int read_int(){
    int res, val;
    do{
        res = scanf("%d", &val);
    }while(res != 1);
    return val;
}

void read_filename(char* filename){
    int res;
    do{
        res = scanf("%s", filename);
    }while(res != 1);
}

int myMax(int a, int b){
    return (a>b?a:b);
}

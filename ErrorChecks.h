#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structures.h"

int checkDbExisting(FILE* fl, FILE* ind, char* errorMes){
    if( ind == NULL || fl == NULL){
        strcpy(errorMes, "database is not created");
        return 0;
    }
    return 1;
}

int checkDbTaskExisting(FILE* fl, char* errorMes){
    if(fl == NULL){
        strcpy(errorMes, "database is not created");
        return 0;
    }
    return 1;
}

int checkIndexExisting(FILE* ind, int id, char* errorMes){
    fseek(ind, 0, SEEK_END);

    if (ftell(ind) == 0 || id * sizeof(struct Indexer) > ftell(ind))
    {
        strcpy(errorMes, "ID is not exist");
        return 0;
    }

    return 1;
}

int checkCourceExisting(struct Cource *cource, char* errorMes){

    if (cource->isExist == 0)
    {
        strcpy(errorMes, "Cource is not exist");
        return 0;
    }

    return 1;
}


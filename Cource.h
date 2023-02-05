#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorChecks.h"

#define COURCE_FL "course.fl"  
#define COURCE_IND "course.ind"
#define COURCE_GARBAGE "courseGarbage.txt"
#define COURCE_SIZE sizeof(struct Cource)
#define INDEXER_SIZE sizeof(struct Indexer)

int insertGarbageCourceId(int id){
    FILE *gb = fopen(COURCE_GARBAGE, "rb+");
    int indexCount = 0;
    fscanf(gb, "%d", &indexCount);
    int *indexIds = malloc(indexCount * sizeof(int));

    for (int i = 0; i < indexCount; ++i) {
        fscanf(gb, "%d", indexIds + i);
    }

    fclose(gb);
    gb = fopen(COURCE_GARBAGE, "wb+");
    fprintf(gb, "%d", indexCount + 1);

    for (int i = 0; i < indexCount; ++i) {
        fprintf(gb, " %d", indexIds[i]);
    }

    fprintf(gb, " %d", id);
    free(indexIds);
    fclose(gb);
    return 1;
}

int rewriteGarbageItem(struct Cource *cource){
    FILE *gb = fopen(COURCE_GARBAGE, "rb+");
    int indexCount = 0;
    fscanf(gb, "%d", &indexCount);
    int *indexIds = malloc(indexCount * sizeof(int));

    for (int i = 0; i < indexCount; ++i) {
        fscanf(gb, "%d", indexIds + i);
    }

    cource->id = indexIds[0];

    fclose(gb);
    gb = fopen(COURCE_GARBAGE, "wb+");
    fprintf(gb, "%d", indexCount - 1);

    for (int i = 1; i < indexCount; ++i) {
        fprintf(gb, " %d", indexIds[i]);
    }

    free(indexIds);
    fclose(gb);
    return 1;
}

int getCource(struct Cource *cource, int id, char* errorMes){
    FILE* ind = fopen(COURCE_IND, "rb+");
    FILE* fl = fopen(COURCE_FL, "rb+");


    if(!checkDbExisting(fl, ind, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    if(!checkIndexExisting(ind, id, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    struct Indexer indexer;
    fseek( ind, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread( &indexer, INDEXER_SIZE, 1, ind);

    fseek(fl, indexer.address, SEEK_SET);
    fread(cource, COURCE_SIZE, 1,  fl);

    if(!checkCourceExisting(cource, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    fclose(ind);
    fclose(fl);

    return 1;
}

int updateCource(struct Cource cource, char* errorMes){
    FILE* ind = fopen(COURCE_IND, "rb+");
    FILE* fl = fopen(COURCE_FL, "rb+");


    if(!checkDbExisting(fl, ind, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    if(!checkIndexExisting(ind, cource.id, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    struct Indexer indexer;
    struct Cource oldCource;
    fseek( ind, (cource.id - 1) * INDEXER_SIZE, SEEK_SET);
    fread( &indexer, INDEXER_SIZE, 1, ind);

    fseek(fl, indexer.address, SEEK_SET);
    fread(&oldCource, COURCE_SIZE, 1,  fl);
    fseek(fl, indexer.address, SEEK_SET);

    if(!checkCourceExisting(&oldCource, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    fwrite( &cource, COURCE_SIZE, 1, fl);

    fclose(ind);
    fclose(fl);

    return 1;
}

int deleteCource(int id, char* errorMes){
    FILE* ind = fopen(COURCE_IND, "rb+");
    FILE* fl = fopen(COURCE_FL, "rb+");

    if(!checkDbExisting(fl, ind, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    if(!checkIndexExisting(ind, id, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    struct Cource cource;
    if(!getCource(&cource, id, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    cource.isExist = 0;

    fclose(ind);
    fclose(fl);
    if(!updateCource(cource, errorMes)){
        fclose(ind);
        fclose(fl);
        return 0;
    }

    insertGarbageCourceId(id);

    return 1;
}

int insertCource(struct Cource cource, char* errorMes){
    FILE* ind = fopen(COURCE_IND, "ab+");
    FILE* fl = fopen(COURCE_FL, "ab+");
    FILE* gb = fopen(COURCE_GARBAGE, "rb+");


    if(!checkDbExisting(fl, ind, errorMes)){
        fclose(ind);
        fclose(fl);
        fclose(gb);
        return 0;
    }

    struct Indexer indexer;

    int gbCount =  0;
    fscanf(gb, "%d", &gbCount);
    fclose(gb);

    if(gbCount){
        rewriteGarbageItem(&cource);
        fclose(ind);
        fclose(fl);
        ind = fopen(COURCE_IND, "rb+");
        fl = fopen(COURCE_FL, "rb+");
        fseek(ind, (cource.id - 1) * INDEXER_SIZE, SEEK_SET);
        fread( &indexer, INDEXER_SIZE, 1, ind);
        fseek(fl, indexer.address, SEEK_SET);
    } else{
        fseek( ind, 0, SEEK_END);
        if(ftell(ind)){
            fseek(ind, -(long)INDEXER_SIZE, SEEK_END);
            fread( &indexer, INDEXER_SIZE, 1, ind);
            cource.id = indexer.id + 1;
        } else{
            cource.id = 1;
        }
    }

    cource.taskCount = 0;
    cource.firstTaskAddress = -1;
    cource.isExist = 1;

    fwrite( &cource, COURCE_SIZE, 1, fl);
    indexer.id = cource.id;
    indexer.address = (cource.id - 1) * COURCE_SIZE;

    fseek(ind, (cource.id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, ind);

    fclose(ind);
    fclose(fl);

    return 1;
}

int systemCourceGetAll(struct Cource *cources, int *count, char *errorMes){
    FILE* fl = fopen(COURCE_FL, "rb+");
    FILE* ind = fopen(COURCE_IND, "rb+");
    if(!checkDbExisting(fl, ind, errorMes)){
        fclose(fl);
        fclose(ind);
        return 0;
    }
    struct Indexer indexer;
    fseek(ind, -(long)INDEXER_SIZE, SEEK_END);
    fread( &indexer, INDEXER_SIZE, 1, ind);
    for (int i = 0; i < indexer.id; ++i) {
        fread( cources + i, COURCE_SIZE, 1, fl);
    }
    *count = indexer.id;

    fclose(fl);
    fclose(ind);
    return 1;
}
int getCourceTotalCount(int *count, char *errorMes){
    FILE* fl = fopen(COURCE_FL, "rb+");
    FILE* ind = fopen(COURCE_IND, "rb+");
    if(!checkDbExisting(fl, ind, errorMes)){
        fclose(fl);
        fclose(ind);
        return 0;
    }
    int totalCount = 0;
    struct Cource cource;
    while(!feof(fl)){
        fread(&cource, COURCE_SIZE, 1, fl);
        if(cource.isExist){
            totalCount++;
        }
    }
    fclose(fl);
    fclose(ind);
    *count = totalCount - 1;
    return  1;
}


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structures.h"
#include "Cource.h"

#define TASK_FL "task.fl"
#define TASK_GARBAGE "taskGarbage.txt"
#define TASK_SIZE sizeof(struct Task)

int insertGarbageTaskAddress(long address){
    FILE *gb = fopen(TASK_GARBAGE, "ab+");
    int indexCount = 0;
    fscanf(gb, "%d", &indexCount);
    long *taskAddresses = malloc(indexCount * sizeof(long));

    for (int i = 0; i < indexCount; ++i) {
        fscanf(gb, "%ld", taskAddresses + i);
    }

    fclose(gb);
    gb = fopen(TASK_GARBAGE, "wb+");
    fprintf(gb, "%ld", indexCount + 1);

    for (int i = 0; i < indexCount; ++i) {
        fprintf(gb, " %ld", taskAddresses[i]);
    }

    fprintf(gb, " %ld", address);
    free(taskAddresses);
    fclose(gb);
    return 1;
}

int rewriteGarbageTask(struct Task *task, long *address){
    FILE *gb = fopen(TASK_GARBAGE, "ab+");
    int indexCount = 0;
    fscanf(gb, "%d", &indexCount);
    long *taskAddresses = malloc(indexCount * sizeof(long));

    for (int i = 0; i < indexCount; ++i) {
        fscanf(gb, "%ld", taskAddresses + i);
    }

    *address = taskAddresses[0];

    fclose(gb);
    gb = fopen(TASK_GARBAGE, "wb+");
    fprintf(gb, "%ld", indexCount - 1);

    for (int i = 1; i < indexCount; ++i) {
        fprintf(gb, " %d", taskAddresses[i]);
    }

    free(taskAddresses);
    fclose(gb);
    return 1;
}

int insertTask(struct Cource cource, struct Task task, char *errorMes){
    FILE* fl = fopen(TASK_FL, "ab+");
    FILE *gb = fopen(TASK_GARBAGE, "ab+");

    if(!checkDbTaskExisting(fl, errorMes)){
        fclose(gb);
        fclose(fl);
        return 0;
    }

    int gbCount = 0;
    if(gb != NULL){
        fscanf(gb, "%d", &gbCount);
        fclose(gb);
    }

    long address;
    struct Task prevTask;
    if(gbCount){
        rewriteGarbageTask(&task, &address);
        fclose(fl);
        fl = fopen(TASK_FL, "rb+");
        fseek(fl, address, SEEK_SET);
        fread(&prevTask, TASK_SIZE, 1, fl);
        task.id = prevTask.id;
    }else{
        fseek( fl, 0, SEEK_END);
        address = ftell(fl);
        if(ftell(fl)){
            fseek(fl, -(long)TASK_SIZE, SEEK_END);
            fread( &prevTask, TASK_SIZE, 1, fl);
            task.id = prevTask.id + 1;
        } else{
            task.id = 1;
        }
    }
    task.courceId = cource.id;
    task.nextTaskAddress = cource.firstTaskAddress;
    cource.firstTaskAddress = address;
    cource.taskCount++;
    if(!updateCource(cource, errorMes)){
        fclose(fl);
        return 0;
    }
    fseek( fl, address, SEEK_SET);
    fwrite(&task, TASK_SIZE, 1, fl);

    fclose(fl);

    return 1;
}


int getTask(struct Cource *cource, struct Task *task, int id, char *errorMes){
    FILE* fl = fopen(TASK_FL, "rb+");

    if(!checkDbTaskExisting(fl, errorMes)){
        return 0;
    }

    if(cource->firstTaskAddress == -1){
        strcpy(errorMes, "This cource does not have any task");
        fclose(fl);
        return 0;
    }

    fseek(fl, cource->firstTaskAddress, SEEK_SET);
    fread(task, TASK_SIZE, 1, fl);
    if(task->id == id){
        fclose(fl);
        return 1;
    }

    while(task->nextTaskAddress != -1){
        fseek(fl, task->nextTaskAddress, SEEK_SET);
        fread(task, TASK_SIZE, 1, fl);
        if(task->id == id){
            return 1;
        }
    }
    fclose(fl);
    strcpy(errorMes, "This cource does not have task with this id");
    return 0;
}

int updateTask(struct Task task, char *errorMes){
    FILE* fl = fopen(TASK_FL, "rb+");

    if(!checkDbTaskExisting(fl, errorMes)){
        return 0;
    }

    fseek(fl, (task.id - 1) * TASK_SIZE, SEEK_SET);
    fwrite(&task, TASK_SIZE, 1, fl);
    fclose(fl);
    return 1;
}

int deleteTask(struct Cource cource, struct Task task, char *errorMes){
    FILE* fl = fopen(TASK_FL, "rb+");
    FILE *gb = fopen(TASK_GARBAGE, "ab+");

    if(!checkDbTaskExisting(fl, errorMes)){
        fclose(gb);
        fclose(fl);
        return 0;
    }
    if(cource.firstTaskAddress == -1){
        fclose(gb);
        fclose(fl);
        strcpy(errorMes, "This cource does not have task with this id");
        return 0;
    }
    struct Task prevTask;
    long deletedAddress = (task.id - 1) * TASK_SIZE;
    fseek(fl, cource.firstTaskAddress, SEEK_SET);
    fread(&prevTask, TASK_SIZE, 1, fl);
    if(prevTask.id == task.id){
        cource.firstTaskAddress = task.nextTaskAddress;
    }else{
        while(prevTask.nextTaskAddress != deletedAddress){
            if(prevTask.nextTaskAddress != -1){
                break;
            }
            fseek(fl, prevTask.nextTaskAddress, SEEK_SET);
            fread(&prevTask, TASK_SIZE, 1, fl);
        }
        if(prevTask.nextTaskAddress == -1) {
            fclose(gb);
            fclose(fl);
            strcpy(errorMes, "This cource does not have task with this id");
            return 0;
        }else{
            prevTask.nextTaskAddress = task.nextTaskAddress;
        }
    }
    insertGarbageTaskAddress(deletedAddress);
    fclose(gb);
    fclose(fl);
    cource.taskCount--;
    if(!updateCource(cource, errorMes)){
        fclose(gb);
        fclose(fl);
        return 0;
    }
    if(!updateTask(prevTask, errorMes)){
        fclose(gb);
        fclose(fl);
        return 0;
    }
    return 1;
}

int deleteTasksByCource(struct Cource cource, char *errorMes){
    FILE* fl = fopen(TASK_FL, "rb+");
    FILE *gb = fopen(TASK_GARBAGE, "ab+");

    if(!checkDbTaskExisting(fl, errorMes)){
        fclose(gb);
        fclose(fl);
        return 0;
    }
    struct Task deletedTask;
    long deletedAddress = cource.firstTaskAddress;
    while(deletedAddress != -1 && !ftell(fl)){
        fseek(fl, deletedAddress, SEEK_SET);
        fread(&deletedTask, TASK_SIZE, 1, fl);
        insertGarbageTaskAddress(deletedAddress);
        deletedAddress = deletedTask.nextTaskAddress;
        deletedTask.nextTaskAddress = -1;
        cource.taskCount--;
        fclose(fl);
        updateTask(deletedTask, errorMes);
        fl = fopen(TASK_FL, "rb+");
    }
    fclose(gb);
    fclose(fl);
    updateCource(cource, errorMes);
    return 1;

}

int getTaskByAddress(struct Task *task, long address, char *errorMes){
    FILE* fl = fopen(TASK_FL, "rb+");

    if(!checkDbTaskExisting(fl, errorMes)){
        fclose(fl);
        return 0;
    }
    if(address == -1){
        strcpy(errorMes, "Task is not exist");
        return 0;
    }
    fseek(fl, address, SEEK_SET);
    fread(task, TASK_SIZE, 1, fl);
    fclose(fl);
    return 1;

}

int systemTaskGetAll(struct Task *tasks, int *count, char *errorMes){
    FILE* fl = fopen(TASK_FL, "rb+");

    if(!checkDbTaskExisting(fl, errorMes)){
        fclose(fl);
        return 0;
    }
    struct Task task;
    fseek(fl, -(long)TASK_SIZE, SEEK_END);
    fread( &task, TASK_SIZE, 1, fl);
    fseek(fl, 0, SEEK_SET);
    for (int i = 0; i < task.id; ++i) {
        fread( tasks + i, TASK_SIZE, 1, fl);
    }
    *count = task.id;

    fclose(fl);
    return 1;
}

int getTaskTotalCount(int *count, char *errorMes){
    FILE* fl = fopen(TASK_FL, "rb+");
    FILE *gb = fopen(TASK_GARBAGE, "ab+");
    if(!checkDbTaskExisting(fl, errorMes)){
        fclose(gb);
        fclose(fl);
        return 0;
    }
    int deletedCount = 0;
    if(gb != NULL){
        fscanf(gb, "%d", &deletedCount);
    }
    int totalCount = 0;
    struct Task task;
    fseek(fl, 0, SEEK_SET);
    while(!feof(fl)){
        fread(&task, TASK_SIZE, 1, fl);
        totalCount++;
    }
    *count = totalCount - 1 - deletedCount;
    return 1;
}

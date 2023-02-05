
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cource.h"
#include "Task.h"
#include "ConsoleActions.h"

#define COURCE_ARRAY_SIZE (COURCE_SIZE * 20)
#define TASK_ARRAY_SIZE (TASK_SIZE * 20)
#define COMMAND_SIZE 10


int main() {
    struct Cource *cources = malloc(COURCE_ARRAY_SIZE);
    struct Task *tasks = malloc(TASK_ARRAY_SIZE);
    struct Cource cource;
    struct Task task;
    char errorMes[128];
    char command[COMMAND_SIZE];
    int res = 1;

    /* strcpy(cource.name, "cource1");
    strcpy(cource.description, "description1");
    cource.firstTaskAddress = -1;
    insertCource(cource, errorMes);

    strcpy(cource.name, "cource2");
    strcpy(cource.description, "description2");
    cource.firstTaskAddress = -1;
    insertCource(cource, errorMes);

    strcpy(cource.name, "cource3");
    strcpy(cource.description, "description3");
    cource.firstTaskAddress = -1;
    insertCource(cource, errorMes);

    strcpy(cource.name, "cource4");
    strcpy(cource.description, "description4");
    cource.firstTaskAddress = -1;
    insertCource(cource, errorMes);

    strcpy(cource.name, "cource5");
    strcpy(cource.description, "description5");
    cource.firstTaskAddress = -1;
    insertCource(cource, errorMes);

    getCource(&cource, 1, errorMes);
    strcpy(task.name, "task11");
    strcpy(task.content, "content11");
    insertTask(cource, task, errorMes);

    getCource(&cource, 2, errorMes);
    strcpy(task.name, "task21");
    strcpy(task.content, "content21");
    insertTask(cource, task, errorMes);
    getCource(&cource, 2, errorMes);
    strcpy(task.name, "task22");
    strcpy(task.content, "content22");
    insertTask(cource, task, errorMes);

    getCource(&cource, 3, errorMes);
    strcpy(task.name, "task31");
    strcpy(task.content, "content31");
    insertTask(cource, task, errorMes);
    getCource(&cource, 3, errorMes);
    strcpy(task.name, "task32");
    strcpy(task.content, "content32");
    insertTask(cource, task, errorMes);
    getCource(&cource, 3, errorMes);
    strcpy(task.name, "task33");
    strcpy(task.content, "content33");
    insertTask(cource, task, errorMes); */

    while (strcmp(command, "end")){
        printf("Enter the command: \n");
        scanf("%s", command);
        if(!strcmp(command, "get-m")){
            res = getMAction(&cource, errorMes);
        }else if(!strcmp(command, "get-s")){
            res = getSAction(&cource, &task, errorMes);
        }else if(!strcmp(command, "del-m")){
            res = delMAction(&cource, errorMes);
        }else if(!strcmp(command, "del-s")){
            res = delSAction(&cource, &task, errorMes);
        }else if(!strcmp(command, "update-m")){
            res = updateMAction(&cource, errorMes);
        }else if(!strcmp(command, "update-s")){
            res = updateSAction(&cource, &task, errorMes);
        }else if(!strcmp(command, "insert-m")){
            res = insertMAction(&cource, errorMes);
        }else if(!strcmp(command, "insert-s")){
            res = insertSAction(&cource, &task, errorMes);
        }else if(!strcmp(command, "calc-m")){
            res = calcMAction(errorMes);
        }else if(!strcmp(command, "calc-s")){
            res = calcSAction(errorMes);
        }else if(!strcmp(command, "ut-m")){
            res = utMAction(cources, errorMes);
        }else if(!strcmp(command, "ut-s")){
            res = utSAction(tasks, errorMes);
        }else if(!strcmp(command, "end")){
            printf("Finish\n");
        }else{
            printf("Command is not valid\n");
        }
        if(!res){
            printf(errorMes);
            res = 1;
        }
        printf("\n");
    }

    return 0;
}

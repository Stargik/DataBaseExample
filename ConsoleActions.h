#ifndef DATABASEEXAMPLE_CONSOLEACTIONS_H
#define DATABASEEXAMPLE_CONSOLEACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cource.h"
#include "Task.h"

#define COURCE_ARRAY_SIZE (COURCE_SIZE * 20)
#define TASK_ARRAY_SIZE (TASK_SIZE * 20)
#define COMMAND_SIZE 10
#define COURCE_NAME_SIZE 50
#define COURCE_DESCRIPTION_SIZE 256
#define TASK_NAME_SIZE 50
#define TASK_CONTENT_SIZE 256

void printCource(struct Cource cource){
    printf("%3d%20s%20s\n", cource.id, cource.name, cource.description);
}

void printTask(struct Task task){
    printf("%3d%3d%20s%20s\n", task.id, task.courceId,
           task.name, task.content);
}

int getMAction(struct Cource *cource, char *errorMes){
    int id = -1;
    char command[10];
    scanf("%s", command);
    if(!strcmp(command, "id")){
        scanf("%d", &id);
        if(getCource(cource, id, errorMes)){
            printf("\n");
            printCource(*cource);
        }else{
            return 0;
        }
    } else if(!strcmp(command, "all")){
        struct Cource *cources = malloc(COURCE_ARRAY_SIZE);
        int courcesCount;
        systemCourceGetAll(cources, &courcesCount, errorMes);
        for (int i = 0; i < courcesCount; ++i) {
            if(cources[i].isExist)
                printCource(cources[i]);
        }
    } else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    return 1;
}

int getSAction(struct Cource *cource, struct Task *task, char *errorMes){
    int id = -1;
    char command[10];
    scanf("%d", &id);
    if(getCource(cource, id, errorMes)){
        scanf("%s", command);
        if(!strcmp(command, "id")){
            scanf("%d", &id);
            if(getTask(cource, task, id, errorMes)){
                printf("\n");
                printTask(*task);
            }else{
                return 0;
            }
        } else if(!strcmp(command, "all")){
            long address = cource->firstTaskAddress;
            if(address == -1){
                strcpy(errorMes, "Cource does not have any task");
                return 0;
            }
            while(address != -1){
                if(getTaskByAddress(task, address, errorMes)){
                    printTask(*task);
                    address = task->nextTaskAddress;
                }else{
                    return 0;
                }
            }
        }else{
            strcpy(errorMes, "Command is not valid");
            return 0;
        }
    }else{
        return 0;
    }
    return 1;
}

int delMAction(struct Cource *cource, char *errorMes){
    int id = -1;
    char command[10];
    scanf("%s", command);
    if(!strcmp(command, "id")){
        scanf("%d", &id);
        if(getCource(cource, id, errorMes)){
            if(!deleteCource(id, errorMes)){
                return 0;
            }
            cource->isExist = 0;
            if(!deleteTasksByCource(*cource, errorMes)){
                return 0;
            }
            printf("Course is deleted\n");
        }else{
            return 0;
        }
    } else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    return 1;
}

int delSAction(struct Cource *cource, struct Task *task, char *errorMes){
    int id = -1;
    char command[10];
    scanf("%d", &id);
    if(getCource(cource, id, errorMes)){
        scanf("%s", command);
        if(!strcmp(command, "id")){
            scanf("%d", &id);
            if(getTask(cource, task, id, errorMes)){
                if(!deleteTask(*cource, *task, errorMes)){
                    return 0;
                }
                printf("Task is deleted\n");
            } else{
                return 0;
            }
        }else{
            strcpy(errorMes, "Command is not valid");
            return 0;
        }
    }else{
        return 0;
    }
    return 1;
}

int updateMAction(struct Cource *cource, char *errorMes){
    int id = -1;
    char command[10];
    scanf("%s", command);
    if(!strcmp(command, "id")){
        scanf("%d", &id);
        if(getCource(cource, id, errorMes)){
            scanf("%s", command);
            if(!strcmp(command, "name")){
                char name[COURCE_NAME_SIZE];
                scanf("%50[^\n]", name);
                fflush(stdin);
                strcpy(cource->name, name);
            } else if(!strcmp(command, "description")){
                char description[COURCE_DESCRIPTION_SIZE];
                scanf("%256[^\n]", description);
                fflush(stdin);
                strcpy(cource->description, description);
            }else{
                strcpy(errorMes, "Command is not valid");
                return 0;
            }
            if (!updateCource(*cource, errorMes)){
                return 0;
            }
            printf("Course is updated\n");
        }else{
            return 0;
        }
    } else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    return 1;
}

int updateSAction(struct Cource *cource, struct Task *task, char *errorMes){
    int id = -1;
    char command[10];
    scanf("%d", &id);
    if(getCource(cource, id, errorMes)){
        scanf("%s", command);
        if(!strcmp(command, "id")){
            id = -1;
            scanf("%d", &id);
            if(getTask(cource, task, id, errorMes)){
                scanf("%s", command);
                if(!strcmp(command, "name")){
                    char name[TASK_NAME_SIZE];
                    scanf("%50[^\n]", name);
                    fflush(stdin);
                    strcpy(task->name, name);
                } else if(!strcmp(command, "content")){
                    char content[TASK_CONTENT_SIZE];
                    scanf("%256[^\n]", content);
                    fflush(stdin);
                    strcpy(task->content, content);
                }else{
                    strcpy(errorMes, "Command is not valid");
                    return 0;
                }
                if(!updateTask(*task, errorMes)){
                    return 0;
                }
                printf("Task is updated\n");
            } else{
                return 0;
            }
        }else{
            strcpy(errorMes, "Command is not valid");
            return 0;
        }
    }else{
        return 0;
    }
    return 1;
}

int insertMAction(struct Cource *cource, char *errorMes){
    char command[10];
    scanf("%s", command);
    if(!strcmp(command, "name")) {
        char name[COURCE_NAME_SIZE];
        scanf("%50[^\n]", name);
        fflush(stdin);
        strcpy(cource->name, name);
    }else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    scanf("%s", command);
    if(!strcmp(command, "description")){
        char description[COURCE_DESCRIPTION_SIZE];
        scanf("%256[^\n]", description);
        fflush(stdin);
        strcpy(cource->description, description);
    }else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    if(!insertCource(*cource, errorMes)){
        return 0;
    }
    printf("Course is added\n");
    return 1;
}

int insertSAction(struct Cource *cource, struct Task *task, char *errorMes){
    char command[10];
    scanf("%s", command);
    if(!strcmp(command, "courceid")) {
        int courceid = -1;
        scanf("%d", &courceid);
        task->courceId = courceid;
    }else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    scanf("%s", command);
    if(!strcmp(command, "name")) {
        char name[TASK_NAME_SIZE];
        scanf("%50[^\n]", name);
        fflush(stdin);
        strcpy(task->name, name);
    }else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    scanf("%s", command);
    if(!strcmp(command, "content")){
        char description[TASK_CONTENT_SIZE];
        scanf("%256[^\n]", description);
        fflush(stdin);
        strcpy(task->content, description);
    }else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    if(getCource(cource, task->courceId, errorMes)){
        if(!insertTask(*cource, *task, errorMes)){
            return 0;
        }
    }else{
        return 0;
    }
    printf("Task is added\n");
    return 1;
}

int calcMAction(char *errorMes){
    int count = 0;
    if(!getCourceTotalCount(&count, errorMes)){
        return 0;
    }
    printf("%d", count);
    return 1;
}

int calcSAction(char *errorMes){
    int count = 0;
    char command[10];
    scanf("%s", command);
    if(!strcmp(command, "total")) {
        if(!getTaskTotalCount(&count, errorMes)){
            return 0;
        }
    }else if(!strcmp(command, "id")){
        int id = -1;
        struct Cource cource;
        scanf("%d", &id);
        if(getCource(&cource, id, errorMes)){
            count = cource.taskCount;
        }else{
            return 0;
        }
    }
    else{
        strcpy(errorMes, "Command is not valid");
        return 0;
    }
    printf("%d", count);
    return 1;
}

int utMAction(struct Cource *cources, char *errorMes){
    int courcesCount = 0;
    if(!systemCourceGetAll(cources, &courcesCount, errorMes)){
        return 0;
    }
    printf("%3s%30s%30s%20s%10s%20s\n", "id", "name", "description",
           "firstTaskAddress", "taskCount", "isExist");

    for (int i = 0; i < courcesCount; ++i) {
        printf("%3d%30s%30s%20ld%10d%20d\n", cources[i].id, cources[i].name, cources[i].description,
               cources[i].firstTaskAddress, cources[i].taskCount, cources[i].isExist);
    }
    return 1;
}

int utSAction(struct Task *tasks, char *errorMes){
    int taskCount = 0;
    if(!systemTaskGetAll(tasks, &taskCount, errorMes)){
        return 0;
    }

    printf("%3s%10s%30s%30s%20s\n", "id", "courceId",
           "name", "content", "nextTaskAddress");

    for (int i = 0; i < taskCount; ++i) {
        printf("%3d%10d%30s%30s%20ld\n", tasks[i].id, tasks[i].courceId,
               tasks[i].name, tasks[i].content, tasks[i].nextTaskAddress);
    }
    return 1;
}

#endif //DATABASEEXAMPLE_CONSOLEACTIONS_H

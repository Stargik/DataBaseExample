
#ifndef DATABASEEXAMPLE_STRUCTURES_H
#define DATABASEEXAMPLE_STRUCTURES_H


struct Cource{
    int id;
    char name[50];
    char description[256];
    long firstTaskAddress;
    int taskCount;
    int isExist;
};

struct Task{
    int id;
    int courceId;
    char name[50];
    char content[256];
    long nextTaskAddress;
};

struct Indexer{
    int id;
    long address;
};

#endif //DATABASEEXAMPLE_STRUCTURES_H
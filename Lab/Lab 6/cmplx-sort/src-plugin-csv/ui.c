/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief user interface for csv-plugin
 * @cdate 2020-11-08
 */

#include <string.h>
#include "ui.h"
#include "../src-kernel/plugin-api.h"
#include "../src-kernel/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_COLUMN 36

void getType(Type *type, char *argv){
    type->dataType = DATA_NULL;
    type->sortType = SORT_NULL;
    char tmpArg[MAX_STR];
    // memset(tmpArg,0,MAX_STR);
    strcpy(tmpArg, argv);
    char *token = strtok(tmpArg, "_ \n");
    if(!strcmp(token, "rand")) type->sortType = SORT_RAND;
    else if(!strcmp(token, "inc")) type->sortType = SORT_INC;
    else if(!strcmp(token, "dec")) type->sortType = SORT_DEC;
    token = strtok(NULL, "_ \n");
    if(!strcmp(token, "int")) type->dataType = DATA_INT;
    else if(!strcmp(token, "char")) type->dataType = DATA_CHAR;
    else if(!strcmp(token, "double")) type->dataType = DATA_DOUBLE;

    assert(type->dataType!=DATA_NULL && type->sortType!=SORT_NULL);
}

void insert(List *list, char *key, void *data){
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->data = data;
    node->next = NULL;
    listAdd(list, node);
}

// void print(Type *type, const List *list){

// }

/**
 * @param argv[1]: input file name
 * @param argv[2]: sorting type
 */
void ui_cmd(char *argv[]){
    Type type[MAX_COLUMN];
    // getType(&type, argv);
    List list[MAX_COLUMN];
    // listConstructor(&list);
    printf("reading %s\n",argv[1]);
    // printf("Z ");fflush(NULL);
    char *raw = malloc(sizeof(char)*MAX_STR);
    char FIRST[MAX_STR];
    memset(raw,0,MAX_STR);
    char *tmpKey, *tmpData;
    char *tmpKeyI;
    int lineCnt=-1,columnCnt=0;
    
    FILE *fin = fopen(argv[1], "r");
    while(fgets(raw,MAX_STR,fin)!=NULL){
        char *raww = malloc(sizeof(char)*MAX_STR);
        char *rawww = raww;
        strcpy(raww,raw);
        // char *praww = &raww[0];
        if(lineCnt==-1){ // first line, get type
            strcpy(FIRST,raw);
            // printf("A "); fflush(NULL);
            tmpKeyI = strsep(&rawww,",\n"); // strtok, fix memory leak [credit: three_cats]
            while(raww != NULL && tmpKeyI != "\0"){
                // printf("%d\n",tmpKeyI[0]);fflush(NULL);
                getType(&(type[columnCnt]),tmpKeyI);
                listConstructor(&(list[columnCnt]));
                columnCnt++;
                tmpKeyI = strsep(&raww, ",\n");
            }
            // printf("A "); fflush(NULL);
        }
        else{ // next lines, get data
            // printf("A "); fflush(NULL);
            int columnTmp=0;
            tmpKeyI = strsep(&raww,",\n");
            while(raww != NULL && tmpKeyI != "\0"){
                tmpKey = strtok(tmpKeyI,"=");
                tmpData = strtok(NULL, " \n");
                void *data;
                if(type[columnTmp].dataType==DATA_INT){
                    int value = atoi(tmpData);
                    // printf("%s|%s|%d\n",tmpKey,tmpData,value);
                    // fscanf(fin, "%d", &value);
                    data = (void *)malloc(sizeof(int));
                    *(int *)data = value;
                }
                else if(type[columnTmp].dataType==DATA_DOUBLE){
                    double value = atof(tmpData);
                    // fscanf(fin, "%lf", &value);
                    data = (void *)malloc(sizeof(double));
                    *(double *)data = value;
                }
                else{
                    // char value[MAX_STR];
                    // fscanf(fin, "%s", value);
                    data = (void *)malloc(sizeof(char)*MAX_STR);
                    memset(data,0,MAX_STR);
                    strcpy((char *)data, tmpData);
                }
                char *key = (char *)malloc(sizeof(char)*MAX_KEY);
                memset(key,0,MAX_KEY);
                strcpy(key,tmpKey);
                insert(&(list[columnTmp]), key, data);
                // memset(raw,0,MAX_STR);
                columnTmp++;
                tmpKeyI = strsep(&raww, ",\n");
            }
        }

        lineCnt++;
        // printf("%s|%d|",raw,(int)sizeof(raw));
    }
    free(raw);
    // Node*tmpnode=list.head;
    // for(int i=0;i<3;i++){
    //     printf("%d ",*(int *)(tmpnode->data));
    //     tmpnode=tmpnode->next;
    // }
    printf("sorting elements\n");
    for(int i=0;i<columnCnt;i++){
        sort(&(type[i]), &(list[i]));
    }
    // print(&type, &list);
    // char outFileName[100];
    // for(int i=0;i<100;i++)outFileName[i]=0;
    // // memset(outFileName,0,100);
    // if(type->sortType==SORT_INC) strcat(outFileName,"inc_");
    // else if(type->sortType==SORT_DEC) strcat(outFileName, "dec_");
    // else strcat(outFileName, "rand_");
    // if(type->dataType==DATA_INT) strcat(outFileName, "int.txt");
    // else if(type->dataType==DATA_DOUBLE) strcat(outFileName, "double.txt");
    // else strcat(outFileName, "char.txt");
    FILE *fout = fopen("export.csv", "w");
    printf("writing %s\n","export.csv");
    
    fprintf(fout,"%s",FIRST);
    Node *nodeIndex[columnCnt];
    for(int i=0;i<columnCnt;i++) nodeIndex[i]=list[i].head;
    for(int dpt=0;dpt<lineCnt;dpt++){
        for(int i=0;i<columnCnt;i++){
            // if(list[i].length==0) break;
            if(type[i].dataType==DATA_INT) fprintf(fout, "%s=%d",nodeIndex[i]->key, *(int*)(nodeIndex[i]->data));
            else if(type[i].dataType==DATA_DOUBLE) fprintf(fout, "%s=%lf",nodeIndex[i]->key, *(double*)(nodeIndex[i]->data));
            else fprintf(fout, "%s=%s",nodeIndex[i]->key, (char*)(nodeIndex[i]->data));
            if(i!=columnCnt-1) fprintf(fout, ",");
            nodeIndex[i]=nodeIndex[i]->next;
        }
        fprintf(fout,"\n");
    }


    // NOTE: test function for listSearch
    // void *tmpp=NULL;
    // tmpp = listSearch(&list,"a");
    // if(tmpp!=NULL) printf("%d\n",*(int*)tmpp);
    for(int i=0;i<columnCnt;i++) listDestructor(&(list[i]));
    fclose(fin);
    // printf("B ");fflush(NULL);
}

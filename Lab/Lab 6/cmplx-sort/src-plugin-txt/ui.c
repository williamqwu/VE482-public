/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief user interface for txt-plugin
 * @cdate 2020-10-27
 * @mdate 2020-11-08
 */

#include "ui.h"
#include "../src-kernel/plugin-api.h"
#include "../src-kernel/list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void getType(Type *type, char *argv[]){
    type->dataType = DATA_NULL;
    type->sortType = SORT_NULL;
    char tmpArg[MAX_STR];
    // memset(tmpArg,0,MAX_STR);
    strcpy(tmpArg, argv[1]);
    char *token = strtok(tmpArg, "_.");
    token = strtok(NULL, "_.");
    if(!strcmp(token, "int")) type->dataType = DATA_INT;
    else if(!strcmp(token, "char")) type->dataType = DATA_CHAR;
    else if(!strcmp(token, "double")) type->dataType = DATA_DOUBLE;
    
    if(!strcmp(argv[2], "rand")) type->sortType = SORT_RAND;
    else if(!strcmp(argv[2], "inc")) type->sortType = SORT_INC;
    else if(!strcmp(argv[2], "dec")) type->sortType = SORT_DEC;

    assert(type->dataType!=DATA_NULL && type->sortType!=SORT_NULL);
}

void insert(List *list, char *key, void *data){
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->data = data;
    node->next = NULL;
    listAdd(list, node);
}

void print(Type *type, const List *list){
    char outFileName[100];
    for(int i=0;i<100;i++)outFileName[i]=0;
    // memset(outFileName,0,100);
    if(type->sortType==SORT_INC) strcat(outFileName,"inc_");
    else if(type->sortType==SORT_DEC) strcat(outFileName, "dec_");
    else strcat(outFileName, "rand_");
    if(type->dataType==DATA_INT) strcat(outFileName, "int.txt");
    else if(type->dataType==DATA_DOUBLE) strcat(outFileName, "double.txt");
    else strcat(outFileName, "char.txt");
    FILE *fout = fopen(outFileName, "w");
    printf("writing %s\n",outFileName);
    if(list->length==0) return;
    Node *nodeIndex;
    for(nodeIndex=list->head;nodeIndex!=NULL;nodeIndex=nodeIndex->next){
        // printf("A ");fflush(NULL);
        if(type->dataType==DATA_INT) fprintf(fout, "%s=%d\n",nodeIndex->key, *(int*)(nodeIndex->data));
        else if(type->dataType==DATA_DOUBLE) fprintf(fout, "%s=%lf\n",nodeIndex->key, *(double*)(nodeIndex->data));
        else fprintf(fout, "%s=%s\n",nodeIndex->key, (char*)(nodeIndex->data));
    }
}

/**
 * @param argv[1]: input file name
 * @param argv[2]: sorting type
 */
void ui_cmd(char *argv[]){
    Type type;
    getType(&type, argv);
    List list;
    listConstructor(&list);
    printf("reading %s\n",argv[1]);
    char *raw = malloc(sizeof(char)*MAX_STR);
    memset(raw,0,MAX_STR);
    char *tmpKey, *tmpData;
    FILE *fin = fopen(argv[1], "r");
    while(fgets(raw,MAX_STR,fin)!=NULL){
        // printf("%s|%d|",raw,(int)sizeof(raw));
        tmpKey = strtok(raw,"=");
        tmpData = strtok(NULL, " \n");
        void *data;
        if(type.dataType==DATA_INT){
            int value = atoi(tmpData);
            // printf("%s|%s|%d\n",tmpKey,tmpData,value);
            // fscanf(fin, "%d", &value);
            data = (void *)malloc(sizeof(int));
            *(int *)data = value;
        }
        else if(type.dataType==DATA_DOUBLE){
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
        insert(&list, key, data);
        memset(raw,0,MAX_STR);
    }
    free(raw);
    // Node*tmpnode=list.head;
    // for(int i=0;i<3;i++){
    //     printf("%d ",*(int *)(tmpnode->data));
    //     tmpnode=tmpnode->next;
    // }
    printf("sorting elements\n");
    sort(&type, &list);
    print(&type, &list);
    // NOTE: test function for listSearch
    // void *tmpp=NULL;
    // tmpp = listSearch(&list,"a");
    // if(tmpp!=NULL) printf("%d\n",*(int*)tmpp);
    listDestructor(&list);
    fclose(fin);
    // printf("B ");fflush(NULL);
}

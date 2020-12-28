/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief sorting utilities for csv-plugin
 * @cdate 2020-11-08
 */

#include "sort.h"
#include "../src-kernel/plugin-api.h"
#include "../src-kernel/list.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

static int rnd(const void *a, const void *b){
    assert(a!=NULL);
    assert(b!=NULL);
    return (((rand())%2)*2-1);
}

static int int_inc(const void *a, const void *b){
    return *((int *)(((Node*)a)->data)) - *((int *)(((Node*)b)->data));
}
static int int_dec(const void *a, const void *b){
    return *(int *)(((Node*)b)->data) - *(int *)(((Node*)a)->data);
}
static int double_inc(const void *a, const void *b){
    return *(double *)(((Node*)a)->data) - *(double *)(((Node*)b)->data);
}
static int double_dec(const void *a, const void *b){
    return *(double *)(((Node*)b)->data) - *(double *)(((Node*)a)->data);
}
static int char_inc(const void *a, const void *b){
    return strcmp((char *)(((Node*)a)->data), (char *)(((Node*)b)->data));
}
static int char_dec(const void *a, const void *b){
    return strcmp((char *)(((Node*)b)->data), (char *)(((Node*)a)->data));
}

int (*cmp[3][3]) (const void *, const void *) = { // TODO: static
    {rnd, char_inc, char_dec},
    {rnd, int_inc, int_dec},
    {rnd, double_inc, double_dec}
};

void sort(Type *type, List *list){
    if(list->length==0) return;
    Node *src[list->length];
    Node *nodeIndex = list->head;
    int index = 0;
    while(nodeIndex!=NULL){
        src[index++]=nodeIndex;
        nodeIndex=nodeIndex->next;
    }
    // qsort(src, list->length, sizeof(Node *), (cmp[type->dataType][type->sortType]));
    Node *flag;
    int j;
    for(int i=1;i<list->length;i++){
        flag = src[i];
        j = i-1;
        while(j>=0 && (cmp[type->dataType][type->sortType](src[j], flag) >= 0)){
            src[j+1]=src[j];
            j--;
        }
        src[j+1]=flag;
    }
    list->head = src[0];
    for(int i=0;i<list->length-1;i++) src[i]->next=src[i+1];
    src[list->length-1]->next = NULL;
    // for(int i=0;i<list->length;i++){
    //     printf("%d ",*(int *)(src[i]->data));
    // }
    // printf("\n");
}

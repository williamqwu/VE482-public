#include "lab5_dlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Node{
    char *key;
    dlistValue value;
    struct _Node *next;
} Node;

typedef struct _List{
    Node *head;
    dlistValueType type;
    int length;
} List;

dlist createDlist(dlistValueType type){
    dlist list = (List *)malloc(sizeof(List));
    ((List *)list)->type = type;
    ((List *)list)->head = NULL;
    ((List *)list)->length = 0;
    if(type==DLIST_UNKOWN) return NULL;
    return list;
}

int dlistIsEmpty(dlist_const this){
    if(((List *)this)->length==0) return 1;
    else return 0;
}

void dlistAppend(dlist this, const char *key, dlistValue value){
    Node *node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->key = key;
    node->next = NULL;
    if(dlistIsEmpty(this)){
        ((List *)this)->head=node;
    }
    else{
        Node *indexNode = ((List *)this)->head;
        while(indexNode->next!=NULL) indexNode=indexNode->next;
        indexNode->next=node;
    }
    ((List *)this)->length++;
}


static int rnd(const void *a, const void *b){
    assert(a!=NULL);
    assert(b!=NULL);
    return (((rand())%2)*2-1);
}

int (*cmp[3][3]) (const void *, const void *) = {
    {rnd, rnd, rnd},
    {rnd, rnd, rnd},
    {rnd, rnd, rnd}
};

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method){
    if(((List *)this)->length==0) return;
    Node *src[((List *)this)->length];
    Node *nodeIndex = ((List *)this)->head;
    int index = 0;
    while(nodeIndex!=NULL){
        src[index++]=nodeIndex;
        nodeIndex=nodeIndex->next;
    }
    Node *flag;
    int j;
    for(int i=1;i<((List *)this)->length;i++){
        flag = src[i];
        j = i-1;
        while(j>=0 && (cmp[((List *)this)->type][((List *)this)->type](src[j], flag) >= 0)){
            src[j+1]=src[j];
            j--;
        }
        src[j+1]=flag;
    }
    ((List *)this)->head = src[0];
    for(int i=0;i<((List *)this)->length-1;i++) src[i]->next=src[i+1];
    src[((List *)this)->length-1]->next = NULL;
}

void dlistPrint(dlist_const this){
    if(((List *)this)->length==0) return;
    Node *nodeIndex;
    for(nodeIndex=((List *)this)->head;nodeIndex!=NULL;nodeIndex=nodeIndex->next){
        if(((List *)this)->type==DLIST_INT) printf("%s=%d\n",nodeIndex->key, (nodeIndex->value));
        else if(((List *)this)->type==DLIST_DOUBLE) printf("%s=%lf\n",nodeIndex->key, (nodeIndex->value));
        else printf("%s=%s\n",nodeIndex->key, (nodeIndex->value));
    }
}

void dlistFree(dlist this){
    if(((List *)this)->length==0) return;
    Node *nodeIndex;
    for(nodeIndex=((List *)this)->head; nodeIndex!=NULL;){
        Node *nodeOld = nodeIndex;
        nodeIndex=nodeIndex->next;
        free(nodeOld);
    }
}

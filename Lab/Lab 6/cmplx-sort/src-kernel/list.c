/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief list utilities
 * @cdate 2020-10-27
 * @mdate 2020-11-08
 */

#include "list.h"

void listConstructor(List *list){
    list->head = NULL;
    list->length = 0;
}

void listAdd(List *list, Node *node){
    if(list->head==NULL) list->head=node;
    else{
        Node *indexNode = list->head;
        while(indexNode->next!=NULL) indexNode=indexNode->next;
        indexNode->next=node;
    }
    list->length++;
}

void *listSearch(List *list, const char *key){
    if(list->length==0) return NULL;
    Node *nodeIndex;
    for(nodeIndex=list->head; nodeIndex!=NULL; nodeIndex=nodeIndex->next){
        if(!strcmp(key, nodeIndex->key)) return nodeIndex->data;
    }
    return NULL;
}

void listDestructor(List *list){
    if(list->length==0) return;
    Node *nodeIndex;
    for(nodeIndex=list->head; nodeIndex!=NULL;){
        Node *nodeOld = nodeIndex;
        free(nodeIndex->key);
        free(nodeIndex->data);
        nodeIndex=nodeIndex->next;
        free(nodeOld);
    }
}

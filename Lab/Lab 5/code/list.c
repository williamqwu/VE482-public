/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief VE482 l5 ex1
 * @cdate 2020-10-27
 */

#include "list.h"

/**
 * @brief initialize the list
 * 
 * @param list 
 */
void listConstructor(List *list){
    list->head = NULL;
    list->length = 0;
}

/**
 * @brief append a new node at the end of the list
 * 
 * @param list 
 * @param node 
 */
void listAdd(List *list, Node *node){
    if(list->head==NULL) list->head=node;
    else{
        Node *indexNode = list->head;
        while(indexNode->next!=NULL) indexNode=indexNode->next;
        indexNode->next=node;
    }
    list->length++;
}

/**
 * @brief search for an element in the list with key;
 *      return NULL if the list is empty or key doesn't match
 * 
 * @param list 
 * @param key 
 * @return void* 
 */
void *listSearch(List *list, const char *key){
    if(list->length==0) return NULL;
    Node *nodeIndex;
    for(nodeIndex=list->head; nodeIndex!=NULL; nodeIndex=nodeIndex->next){
        if(!strcmp(key, nodeIndex->key)) return nodeIndex->data;
    }
    return NULL;
}

/**
 * @brief free the list
 * 
 * @param list 
 */
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
